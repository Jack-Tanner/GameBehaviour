#pragma once

#include "BaseGameState.h"

class GameStateManager
{
public:
	GameStateManager();
	~GameStateManager();

	bool SetGameState( eGameState eState );

	void Update( float fDelta );

	BaseGameState* GetCurrentState()
	{
		return m_pCurrentGameState;
	}

	static GameStateManager* GetInstance()
	{
		return m_pInstance;
	}

private:
	BaseGameState* m_pCurrentGameState;
	static GameStateManager* m_pInstance;
	
};

