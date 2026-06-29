#pragma once
#include "Prerequisites.h"
#include "ECS/System.h"
#include "ECS/Registry.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Render.h"
#include "ECS/Components/Camera.h"

// ============================================================
//  ECS :: Systems/UISystem.h
//
//  UISystem — LÓGICA de los paneles de ImGui.
//
//  IMPORTANTE: este sistema contiene SOLO los ImGui::Begin/End
//  y los widgets. El ciclo de frame de ImGui
//  (ImGui_ImplXXX_NewFrame + ImGui::NewFrame() al inicio, y
//   ImGui::Render() + RenderDrawData al final) NO va aquí:
//  vive en el game loop / en tu Window.
//
//  Por eso este sistema debe registrarse EL ÚLTIMO
//  (después del RenderSystem), para que la UI quede encima.
// ============================================================

namespace ECS {

	class UISystem final : public System {
	public:
		UISystem() = default;

    void OnStart(Registry& /*registry*/) override {
      // Habilitar docking.
      ImGuiIO& io = ImGui::GetIO();
      io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    }

		void 
    OnUpdate(Registry& registry, float deltaTime) override {
      ImGuiDockNodeFlags dockspaceFlags =
        ImGuiDockNodeFlags_PassthruCentralNode;

      ImGui::DockSpaceOverViewport(
        0,
        ImGui::GetMainViewport(),
        dockspaceFlags
      );

			DrawOutliner(registry);
			DrawDetails(registry);
		}

		void 
    DrawOutliner(Registry& registry) {
			ImGui::Begin("Outliner");
			{
				registry.GetView<ECS::Transform>().Each(
					[this](ECS::EntityID id, ECS::Transform& /*transform*/)	{
						const std::string label = "Entity " + std::to_string(id);
						const bool isSelected = (id == selectedEntity);

						if (ImGui::Selectable(label.c_str(), isSelected))
							selectedEntity = id;
					});
			}
			ImGui::End();
		}

		void 
    DrawDetails(Registry& registry) {
      ImGui::Begin("Details");
      {
        if (selectedEntity != ECS::NULL_ENTITY && registry.IsAlive(selectedEntity)) {
          ImGui::Text("Entity %llu", static_cast<unsigned long long>(selectedEntity));
          ImGui::Separator();

          if (auto* t = registry.TryGetComponent<ECS::Transform>(selectedEntity)) {
            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
              ImGui::DragFloat2("Position", &t->position.x, 1.f);
              ImGui::DragFloat("Rotation", &t->rotation, 1.f);
              ImGui::DragFloat2("Scale", &t->scale.x, 0.01f);
            }
          }

          if (auto* r = registry.TryGetComponent<ECS::Render>(selectedEntity)) {
            if (ImGui::CollapsingHeader("Render", ImGuiTreeNodeFlags_DefaultOpen)) {
              ImGui::Checkbox("Visible", &r->visible);

              // sf::Color es 0..255; ImGui::ColorEdit4 trabaja en 0..1.
              float color[4] = {
                r->fillColor.r / 255.f,
                r->fillColor.g / 255.f,
                r->fillColor.b / 255.f,
                r->fillColor.a / 255.f
              };
              if (ImGui::ColorEdit4("Fill Color", color)) {
                r->fillColor = sf::Color(
                  static_cast<std::uint8_t>(color[0] * 255.f),
                  static_cast<std::uint8_t>(color[1] * 255.f),
                  static_cast<std::uint8_t>(color[2] * 255.f),
                  static_cast<std::uint8_t>(color[3] * 255.f));
              }
            }
          }

          if (auto* cam = registry.TryGetComponent<ECS::Camera>(selectedEntity)) {
            if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
              // La posición y la rotación de la cámara se editan arriba,
              // en el bloque Transform. Aquí solo lo propio de la cámara.
              ImGui::Checkbox("Active", &cam->active);
              ImGui::DragFloat("Zoom", &cam->zoom, 0.01f, 0.05f, 10.f);
              ImGui::DragFloat("Follow Speed", &cam->followSpeed, 0.1f, 0.f, 50.f);

              // Objetivo a seguir (solo lectura aquí; muestra el ID).
              if (cam->followTarget == ECS::NULL_ENTITY)
                ImGui::Text("Follow Target: (ninguno)");
              else
                ImGui::Text("Follow Target: %llu",
                  static_cast<unsigned long long>(cam->followTarget));
            }
          }
        }
        else {
          ImGui::TextDisabled("Selecciona una entidad en el outliner.");
        }
      }
      ImGui::End();
		}

	private:
		// Entidad seleccionada en el outliner (NULL_ENTITY = ninguna).
		ECS::EntityID selectedEntity = ECS::NULL_ENTITY;
	};

};