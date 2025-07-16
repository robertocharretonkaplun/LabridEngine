#include "BaseApp.h"

BaseApp::~BaseApp() {
}

int
BaseApp::run() {
	if (!init()) {
		ERROR("BaseApp",
			"run",
			"Initializes result on a false statemente, check method validations");
	}

	while (m_windowPtr->isOpen()) {
		m_windowPtr->handleEvents();
		update();
		render();
	}

	destroy();
	return 0;
}

bool
BaseApp::init() {
	m_windowPtr = EngineUtilities::MakeShared<Window>(1920, 1080, "Labrid Engine");
	if (!m_windowPtr) {
		ERROR("BaseApp",
			"init",
			"Failed to create window pointer, check memory allocation");
		return false;
	}


	// Create Circle Actor
	m_ACirlce = EngineUtilities::MakeShared<Actor>("Circle Actor");
	if (m_ACirlce) {
		m_ACirlce->getComponent<CShape>()->createShape(CIRCLE);
		m_ACirlce->getComponent<CShape>()->setFillColor(sf::Color::Red);
		m_ACirlce->getComponent<Transform>()->setPosition(sf::Vector2f(200.f, 150.f));
		//m_ACirlce->setName("Circle Actor");
	}
	else {
		ERROR("BaseApp",
			"init",
			"Failed to create Circle Actor, check memory allocation");
		return false;
	}
	return true;
}

void
BaseApp::update() {
	if (!m_windowPtr.isNull()) {
		m_windowPtr->update();
	}
	// Update actors
	if (!m_ACirlce.isNull()) {
		m_ACirlce->update(m_windowPtr->deltaTime.asSeconds());

		// Posicion del destino (Punto recorrido)
		sf::Vector2f targetPos(1200.f, 150.f);

		// Llamar al seek del Transform
		m_ACirlce->getComponent<Transform>()->seek(targetPos, 200.0f, m_windowPtr->deltaTime.asSeconds(), 10.0f);
	}
}

void
BaseApp::render() {
	if (!m_windowPtr) {
		return;
	}
	m_windowPtr->clear();

	if (!m_ACirlce.isNull()) {
		m_ACirlce->getComponent<CShape>()->render(m_windowPtr);
	}
	m_windowPtr->display();
}

void
BaseApp::destroy() {
	//m_shapePtr.reset();
	//m_window->destroy();
}