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
	return false;
}

void CollidableSpriteEntity::Update( float fDelta )
{
}

void CollidableSpriteEntity::SerializeIn( json::json & jsonValue )
{
}

void CollidableSpriteEntity::SerializeOut( json::json & jsonOutValue )
{
}

void CollidableSpriteEntity::Destroy()
{
}
