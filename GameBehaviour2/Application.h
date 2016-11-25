#pragma once
#include <SDL.h>
#include <PhysicsManager.h>

#include "Window.h"
#include "Renderer.h"
#include "GameStateManager.h"


class Application
{
public:
	Application();
	~Application();

	bool	Initialise();
	void	Run();
	void	Shutdown();

	void	RequestShutdown()
	{
		m_bShutdownRequested = true;
	}

private:

	bool			m_bExit;
	bool			m_bShutdownRequested;
	SDL_GLContext	m_Context;

	Window			m_Window;

	Renderer		m_Renderer;
	PhysicsManager	m_PhysicsManager;
	GameStateManager m_GameStateManager;
};

