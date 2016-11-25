#pragma once
#include "SpriteEntity.h"

#include <RigidBody.h>

class CollidableSpriteEntity : public SpriteEntity
{
public:
	CollidableSpriteEntity();
	virtual ~CollidableSpriteEntity();

	virtual bool	Initialise( BaseInitialiser* pInitialiser );
	virtual void	Update( float fDelta = 1.0f );

	virtual void	SerializeIn( json::json& jsonValue );
	virtual	void	SerializeOut( json::json& jsonOutValue );

private:
	virtual void	Destroy();

protected:




};

