#pragma once
#include "SpriteEntity.h"

#include <RigidBody.h>

struct CollidableSpriteInitialiser : public SpriteInitialiser
{
	bool				m_bGravity;
	float				m_fMass;
	float				m_fFriction;
	RigidBodyType		m_BodyType;
	BaseShape::Type		m_ShapeType;
	glm::vec2			m_vShapeSize;

	CollidableSpriteInitialiser()
	{
		m_Type = EntityType::eCollidableSprite;

		m_bGravity = false;
		m_fMass = 10.0f;
		m_fFriction = 0.5f;
		m_BodyType = RigidBodyType::eStatic;
		m_ShapeType = BaseShape::Type::eNone;
		m_vShapeSize = glm::vec2( 1.0f, 1.0f );
	}
};

class CollidableSpriteEntity : public SpriteEntity
{
public:
	CollidableSpriteEntity();
	virtual ~CollidableSpriteEntity();

	virtual bool	Initialise( BaseInitialiser* pInitialiser );
	virtual void	Update( float fDelta = 1.0f );

	virtual void	SerializeIn( json::json& jsonValue );
	virtual	void	SerializeOut( json::json& jsonOutValue );

protected:
	virtual void	Destroy();
	RigidBody		m_Body;



};

