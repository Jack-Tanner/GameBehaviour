#pragma once
#include "BaseGameState.h"
class MenuState :
	public BaseGameState
{
public:
	MenuState();
	virtual ~MenuState();

	virtual bool Load();
	virtual void Destroy();

	virtual void OnStart();
	virtual void Update( float fDelta );
	virtual void OnEnd();
};

