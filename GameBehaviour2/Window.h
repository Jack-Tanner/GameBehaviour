#pragma once
#include <SDL.h>

class Window
{
public:
	Window();
	virtual ~Window();

	bool			Create( const char* pszTitle, int x, int y, int w, int h );

	void			SwapBuffer();

	SDL_Window*		GetWindow() const
	{
		return m_pWindow;
	}


private:
	SDL_Window*			m_pWindow;

};

