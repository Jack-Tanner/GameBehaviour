#pragma once
#include "BaseGameState.h"
class BootState :
	public BaseGameState
{
public:
	BootState();
	virtual ~BootState();

	virtual bool Load();
	virtual void Destroy();

	virtual void OnStart();
	virtual void Update( float fDelta );
	virtual void OnEnd();
};

