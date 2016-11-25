#pragma once
#include <SDL.h>

#include <glm/glm.hpp>

class BaseInputManager
{
public:

	void Update();

	bool IsKeyPressed( unsigned int key ) const;

	bool IsKeyHeld( unsigned int key ) const;

	bool IsKeyReleased( unsigned int key ) const;

	bool IsMouseButtonPressed( Uint32 button ) const;
	bool IsMouseButtonHeld( Uint32 button ) const;
	bool IsMouseButtonReleased( Uint32 button ) const;
	

	glm::vec2 GetMousePosition() const
	{
		return m_CurrentMousePosition;
	}

	glm::vec2 GetMouseDelta() const
	{
		return m_PreviousMousePosition - m_CurrentMousePosition;
	}

	static BaseInputManager* GetInstance();

private:
	BaseInputManager();
	~BaseInputManager();

	static	BaseInputManager*	m_pInstance;


	Uint8*	m_CurrentKeyStates;
	Uint8*	m_PreviousKeyStates;
	int		m_KeyStateLength;

	glm::vec2 m_PreviousMousePosition;
	glm::vec2 m_CurrentMousePosition;

	Uint32 m_CurrentMouseButtonState;
	Uint32 m_PreviousMouseButtonState;


};

