#include "SpriteEntity.h"

#include <TexturedQuad.h>
#include <TextureManager.h>
#include <BaseShader.h>
#include <glm/gtc/matrix_transform.hpp>


SpriteEntity::SpriteEntity()
{
	m_Type = EntityType::eSprite;
}


SpriteEntity::~SpriteEntity()
{
}

bool SpriteEntity::Initialise( BaseInitialiser* pInitialiser )
{
	if ( pInitialiser == nullptr || pInitialiser->m_Type != EntityType::eSprite )
		return false;

	BaseEntity::Initialise( pInitialiser );

	m_bInitialised = true;
	SpriteInitialiser* pSpriteInit = ( SpriteInitialiser* ) pInitialiser;

	m_Quad.Initialise( pSpriteInit->m_pszShader, pSpriteInit->m_pszTexture );

	return true;
}

void SpriteEntity::Update( float fDelta )
{

	glm::mat4 matWorld( 1.0f );
	matWorld = glm::translate( matWorld, glm::vec3( m_Transform.m_Position.x, m_Transform.m_Position.y, 0.0f ) );
	matWorld = glm::rotate( matWorld, m_Transform.m_Rotation, glm::vec3( 0.0f, 0.0f, 1.0f ) );
	matWorld = glm::scale( matWorld, glm::vec3( m_Transform.m_Scale.x, m_Transform.m_Scale.y, 1.0f ) );
	
	
	m_Quad.SetWorldMatrix( matWorld );
}

void SpriteEntity::SetTexture( const char * pszTextureName )
{
	BaseTexture* pTexture = TextureManager::GetInstance()->GetTexture( pszTextureName );
	if ( pTexture )
	{
		m_Quad.SetTexture( pTexture );
	}
}

void SpriteEntity::SetTexture( BaseTexture * pTexture )
{
	m_Quad.SetTexture( pTexture );
}

void SpriteEntity::SerializeIn( json::json & jsonValue )
{
	BaseEntity::SerializeIn( jsonValue );

	std::string strTexture = jsonValue["texture"];
	std::string strShader = jsonValue["shader"];

	TextureManager::GetInstance()->AddTexture( strTexture );

	m_Quad.Initialise( strShader.c_str(), strTexture.c_str() );
}

void SpriteEntity::SerializeOut( json::json & jsonOutValue )
{
	BaseEntity::SerializeOut( jsonOutValue );

	jsonOutValue["texture"] = m_Quad.GetTexture()->GetFileName();
	jsonOutValue["shader"] = m_Quad.GetShader()->GetName();
}

void SpriteEntity::Destroy()
{
	m_Quad.Destroy();
}
