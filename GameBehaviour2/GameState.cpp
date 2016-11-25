#include "GameState.h"

#include "SpriteEntity.h"
#include <TextureManager.h>


GameState::GameState()
{
}


GameState::~GameState()
{
}

bool GameState::Load()
{
	TextureManager* pTexManager = TextureManager::GetInstance();
	BaseTexture* pTexture = pTexManager->AddTexture( "player.png" );
	SpriteInitialiser* pInit = new SpriteInitialiser();
	pInit->m_pszShader = "BasicTextured";
	pInit->m_pszTexture = "player.png";

	m_World.AddEntity<SpriteEntity>( pInit );

	return true;
}

void GameState::Destroy()
{
	m_World.Destroy();
}

void GameState::OnStart()
{

}

void GameState::Update( float fDelta )
{
	m_World.Update( fDelta );
}

void GameState::OnEnd()
{
}
