#pragma once

#include "BaseEntity.h"
#include <TexturedQuad.h>

struct SpriteInitialiser : public BaseInitialiser
{
	const char* m_pszShader;
	const char* m_pszTexture;

	SpriteInitialiser()
	{
		m_Type = EntityType::eSprite;
		m_pszShader = nullptr;
		m_pszTexture = nullptr;
	}
};

class SpriteEntity : public BaseEntity
{
public:
	SpriteEntity();
	virtual ~SpriteEntity();

	virtual bool	Initialise(BaseInitialiser* pInitialiser);
	virtual void	Update( float fDelta = 1.0f );

	void			SetTexture( const char* pszTextureName );
	void			SetTexture( BaseTexture* pTexture );
	BaseTexture*	GetTexture();

	virtual void	SerializeIn( json::json& jsonValue );
	virtual	void	SerializeOut( json::json& jsonOutValue );

protected:
	virtual void	Destroy();
	TexturedQuad	m_Quad;
};

