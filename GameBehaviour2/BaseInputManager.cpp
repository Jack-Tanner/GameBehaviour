#include "BaseInputManager.h"

BaseInputManager* BaseInputManager::m_pInstance = nullptr;

BaseInputManager::BaseInputManager() : m_CurrentKeyStates( nullptr ), m_PreviousKeyStates( nullptr ), m_KeyStateLength( 0 ),
	m_PreviousMousePosition( 0.0f, 0.0f ), m_CurrentMousePosition( 0.0f, 0.0f ), m_PreviousMouseButtonState( 0 ), m_CurrentMouseButtonState( 0 )
{

}


BaseInputManager::~BaseInputManager()
{
}

void BaseInputManager::Update()
{
	const Uint8* pCurrentState =  SDL_GetKeyboardState( &m_KeyStateLength );

	if ( m_CurrentKeyStates == nullptr )
	{
		m_CurrentKeyStates = new Uint8[m_KeyStateLength];
	}

	if ( m_PreviousKeyStates == nullptr )
	{
		m_PreviousKeyStates = new Uint8[m_KeyStateLength];
	}

	SDL_memcpy( m_PreviousKeyStates, m_CurrentKeyStates, m_KeyStateLength );
	SDL_memcpy( m_CurrentKeyStates, pCurrentState, m_KeyStateLength );

	int x = 0;
	int y = 0;
	m_PreviousMouseButtonState = m_CurrentMouseButtonState;
	m_CurrentMouseButtonState = SDL_GetMouseState( &x, &y );
	m_PreviousMousePosition = m_CurrentMousePosition;
	m_CurrentMousePosition = glm::vec2( x, y );

}

BaseInputManager * BaseInputManager::GetInstance()
{
	if ( m_pInstance == nullptr )
		m_pInstance = new BaseInputManager();

	return m_pInstance;
}

bool BaseInputManager::IsKeyPressed( unsigned int key ) const
{
	bool bResult = m_PreviousKeyStates[key] == 0 && m_CurrentKeyStates[key] == 1;
	return bResult;
}

bool BaseInputManager::IsKeyHeld( unsigned int key ) const
{
	bool bResult = m_PreviousKeyStates[key] == 1 && m_CurrentKeyStates[key] == 1;
	return bResult;
}

bool BaseInputManager::IsKeyReleased( unsigned int key ) const
{
	bool bResult = m_PreviousKeyStates[key] == 1 && m_CurrentKeyStates[key] == 0;
	return bResult;
}

bool BaseInputManager::IsMouseButtonPressed( Uint32 button ) const
{
	return ( ( m_PreviousMouseButtonState & button ) == 0 && ( m_CurrentMouseButtonState & button ) );
}

bool BaseInputManager::IsMouseButtonHeld( Uint32 button ) const
{
	return ( ( m_PreviousMouseButtonState & button ) && ( m_CurrentMouseButtonState & button ) );
}

bool BaseInputManager::IsMouseButtonReleased( Uint32 button ) const
{
	return ( ( m_PreviousMouseButtonState & button ) && ( m_CurrentMouseButtonState & button ) == 0 );
}

