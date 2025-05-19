#pragma once
#include "Prerequisites.h"

class 
BaseApp {
public:
	BaseApp() = default;
	~BaseApp();

	// Funcion encargada de ejecutar la aplicacion en main
	int
	run();

	// Funcion de inicializacion
	bool
	init();

	// Funcion que se actualiza por frame
	void
	update();

	// Funcion de renderizado
	void
	render();

	void
	destroy();

	void 
	handleEvents() ;

private:
	sf::RenderWindow* m_window;
	sf::CircleShape*  m_circle;
};