#include "GameStateManager.h"
#include <Debug.h>

#include "BootState.h"
#include "MenuState.h"
#include "GameState.h"

GameStateManager* GameStateManager::m_pInstance = nullptr;

GameStateManager::GameStateManager() : m_pCurrentGameState( nullptr )
{
	m_pInstance = this;
}


GameStateManager::~GameStateManager()
{
	if ( m_pCurrentGameState )
	{
		m_pCurrentGameState->Destroy();
		delete m_pCurrentGameState;
	}

	m_pInstance = nullptr;
}

bool GameStateManager::SetGameState( eGameState eState )
{
	BaseGameState* pNewState = nullptr;
	switch ( eState )
	{
	case eGameState::eBoot:
		pNewState = new BootState();
		break;
	case eGameState::eMenu:
		pNewState = new MenuState();
		break;
	case eGameState::eLoading:
		break;
	case eGameState::eGame:
		pNewState = new GameState();
		break;
	case eGameState::eMax:
	case eGameState::eNone:
	default:
		if ( m_pCurrentGameState )
		{
			m_pCurrentGameState->Destroy();
			delete m_pCurrentGameState;
			m_pCurrentGameState = nullptr;
		}
		return true;
	}

	bool bResult = pNewState->Load();
	GASSERT( bResult );

	if ( m_pCurrentGameState )
	{
		m_pCurrentGameState->OnEnd();
		m_pCurrentGameState->Destroy();
	}

	GASSERT( pNewState );
	m_pCurrentGameState = pNewState;
	m_pCurrentGameState->OnStart();

	return true;
}

void GameStateManager::Update( float fDelta )
{
	if ( m_pCurrentGameState )
	{
		m_pCurrentGameState->Update( fDelta );
	}
}
