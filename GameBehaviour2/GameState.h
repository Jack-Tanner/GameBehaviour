#pragma once
#include "BaseGameState.h"
class GameState :
	public BaseGameState
{
public:
	GameState();
	virtual ~GameState();

	virtual bool Load();
	virtual void Destroy();

	virtual void OnStart();
	virtual void Update( float fDelta );
	virtual void OnEnd();
};

