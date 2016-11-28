#pragma once

#include <World.h>

enum class eGameState
{
	eNone,
	eBoot,
	eMenu,
	eLoading,
	eGame,
	eMax
};

class BaseGameState
{
public:
	BaseGameState();
	virtual ~BaseGameState();

	virtual bool Load() = 0;
	virtual void Destroy() = 0;

	virtual void OnStart() = 0;
	virtual void Update( float fDelta ) = 0;
	virtual void OnEnd() = 0;

	eGameState GetStateType() const
	{
		return m_eStateType;
	}

	World* GetWorld()
	{
		return &m_World;
	}

protected:
	eGameState	m_eStateType;

	World		m_World;
};

