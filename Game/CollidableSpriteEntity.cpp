#include "CollidableSpriteEntity.h"


CollidableSpriteEntity::CollidableSpriteEntity() 
{
	m_Type = EntityType::eCollidableSprite;
}


CollidableSpriteEntity::~CollidableSpriteEntity()
{
}

bool CollidableSpriteEntity::Initialise( BaseInitialiser * pInitialiser )
{
	if ( pInitialiser == nullptr )
		return false;

	SpriteEntity::Initialise( pInitialiser );

	m_bInitialised = true;
	CollidableSpriteInitialiser* pSpriteInit = ( CollidableSpriteInitialiser* ) pInitialiser;
	m_Body.Initialise( &m_Transform, pSpriteInit->m_BodyType, pSpriteInit->m_fMass, pSpriteInit->m_fFriction, pSpriteInit->m_bGravity );

	switch ( pSpriteInit->m_ShapeType )
	{
	case BaseShape::eAABB:
		m_Body.AddAABB( pSpriteInit->m_vShapeSize );
		break;
	case BaseShape::eOOBB:
		m_Body.AddOOBB( pSpriteInit->m_vShapeSize );
		break;
	case BaseShape::eCircle:
		GASSERT( false ); // Implement me.
		break;

	case BaseShape::eNone:
	case BaseShape::eMax:
	default:
		GASSERT( false );
		break;
	}

	return true;
}

void CollidableSpriteEntity::Update( float fDelta )
{
	SpriteEntity::Update( fDelta );
}

void CollidableSpriteEntity::SerializeIn( json::json & jsonValue )
{
	SpriteEntity::SerializeIn( jsonValue );

	unsigned int uBodyType = jsonValue["bodyType"];
	unsigned int uShapeType = jsonValue["shapeType"];
	float fMass = jsonValue["mass"];
	float fFriction = jsonValue["friction"];
	bool bGravity = jsonValue["gravity"];

	m_Body.Initialise( &m_Transform, ( RigidBodyType ) uBodyType, fMass, fFriction, bGravity );

	if ( uShapeType != 0 )
	{
		BaseShape::Type eShapeType = ( BaseShape::Type )uShapeType;
		glm::vec2 vShapeSize;
		vShapeSize.x = jsonValue["shapeSize"]["x"];
		vShapeSize.y = jsonValue["shapeSize"]["y"];

		switch ( eShapeType )
		{
		case BaseShape::eAABB:
			m_Body.AddAABB( vShapeSize );
			break;
		case BaseShape::eOOBB:
			m_Body.AddOOBB( vShapeSize );
			break;
		case BaseShape::eCircle:
			GASSERT( false ); // Implement me.
			break;

		case BaseShape::eNone:
		case BaseShape::eMax:
		default:
			GASSERT( false );
			break;

		}

	}
}

void CollidableSpriteEntity::SerializeOut( json::json & jsonOutValue )
{
	SpriteEntity::SerializeOut( jsonOutValue );

	jsonOutValue["bodyType"] = (unsigned int)m_Body.GetBodyType();
	BaseShape* pShape = m_Body.GetShape();
	if ( pShape )
	{
		jsonOutValue["shapeType"] = ( unsigned int ) pShape->GetType();
		jsonOutValue["shapeSize"]["x"] = pShape->GetSize().x * 2.0f;
		jsonOutValue["shapeSize"]["y"] = pShape->GetSize().y * 2.0f;
	}
	else
	{
		jsonOutValue["shapeType"] = 0;
		jsonOutValue["shapeSize"]["x"] = 1.0f;
		jsonOutValue["shapeSize"]["y"] = 1.0f;
	}

	jsonOutValue["mass"] = m_Body.m_fMass;
	jsonOutValue["friction"] = m_Body.m_fFriction;
	jsonOutValue["gravity"] = m_Body.m_bGravity;
	
}

void CollidableSpriteEntity::Destroy()
{
	SpriteEntity::Destroy();
}
