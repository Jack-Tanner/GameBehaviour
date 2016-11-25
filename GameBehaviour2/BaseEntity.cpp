#include "BaseEntity.h"

#include <PhysicsManager.h>

#include "World.h"

BaseEntity::BaseEntity() : m_bInitialised( false ), m_ID( INVALID_ENTITY_ID ), m_pOwnerWorld( nullptr ), m_Type( EntityType::eNone )
{
}

BaseEntity::~BaseEntity()
{
	if ( m_bInitialised )
	{
		Destroy();
	}
}

bool BaseEntity::Initialise(BaseInitialiser * pInitialiser)
{
	if (pInitialiser != nullptr)
	{
		m_Transform = pInitialiser->m_Transform;
		return true;
	}

	return false;
}

void BaseEntity::SerializeOut( json::json & jsonOutValue )
{
	jsonOutValue["type"] = ( unsigned int ) m_Type;
	jsonOutValue["id"] = m_ID;

	jsonOutValue["transform"]["position"]["x"] = m_Transform.m_Position.x;
	jsonOutValue["transform"]["position"]["y"] = m_Transform.m_Position.y;

	jsonOutValue["transform"]["scale"]["x"] = m_Transform.m_Scale.x;
	jsonOutValue["transform"]["scale"]["y"] = m_Transform.m_Scale.y;
	jsonOutValue["transform"]["rotation"] = m_Transform.m_Rotation;

}

void BaseEntity::SerializeIn( json::json & jsonValue )
{
	m_ID = jsonValue["id"];
	m_Transform.m_Position.x = jsonValue["transform"]["position"]["x"];
	m_Transform.m_Position.y = jsonValue["transform"]["position"]["y"];

	m_Transform.m_Scale.x = jsonValue["transform"]["scale"]["x"];
	m_Transform.m_Scale.y = jsonValue["transform"]["scale"]["y"];

	m_Transform.m_Rotation = jsonValue["transform"]["rotation"];

	m_bInitialised = true;
	
}

void BaseEntity::SetID( unsigned int uID )
{
	m_ID = uID;
}

void BaseEntity::Destroy()
{
	m_bInitialised = false;
	m_ID = INVALID_ENTITY_ID;
}
