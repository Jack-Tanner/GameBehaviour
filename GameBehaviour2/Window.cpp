#include "Window.h"



Window::Window()
{
}


Window::~Window()
{
}
bool Window::Create( const char * pszTitle, int x, int y, int w, int h )
{
	m_pWindow = SDL_CreateWindow( pszTitle, x, y, w, h, SDL_WINDOW_OPENGL );
	if ( !m_pWindow )
		return false;

	return true;
}

void Window::SwapBuffer()
{
	SDL_GL_SwapWindow( m_pWindow );
}
