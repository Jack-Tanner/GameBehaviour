#pragma once

#include <Transform.h>
#include "EntityTypes.h"
#include <json.hpp>


class World;

struct BaseInitialiser
{
	EntityType	m_Type;
	Transform	m_Transform;
};

class BaseEntity
{
public:
	BaseEntity();
	virtual ~BaseEntity();

	friend class World;

	virtual bool	Initialise( BaseInitialiser* pInitialiser );
	virtual void	Update( float fDelta = 1.0f ) = 0;

	virtual	void	SerializeOut( json::json& jsonOutValue );
	virtual void	SerializeIn( json::json& jsonValue );

	bool			IsInitialised() const
	{
		return m_bInitialised;
	}

	unsigned int	GetID() const
	{
		return m_ID;
	}

	Transform&		GetTransform()
	{
		return m_Transform;
	}

protected:
	Transform		m_Transform;
	bool			m_bInitialised;
	World*			m_pOwnerWorld;

	EntityType		m_Type;
	
	virtual void	Destroy() = 0;

private:
	// ONLY to be called by World.
	void			SetID( unsigned int uID );
	

	unsigned int	m_ID;
};

