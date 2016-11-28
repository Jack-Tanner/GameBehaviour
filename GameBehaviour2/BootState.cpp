#include "BootState.h"

#include <TextureManager.h>

#include <CollidableSpriteEntity.h>
#include "BaseInputManager.h"


BootState::BootState()
{
	m_eStateType = eGameState::eBoot;
}


BootState::~BootState()
{
}

bool BootState::Load()
{
	m_World.SerializeIn( "level.json" );


	return true;
}

void BootState::Destroy()
{
	m_World.Destroy();
}

void BootState::OnStart()
{
}

void BootState::Update( float fDelta )
{
	auto pInputMan = BaseInputManager::GetInstance();
	if ( pInputMan->IsKeyPressed( SDL_SCANCODE_S ) )
	{
		m_World.SerializeOut( "test2.json" );
	}

	m_World.Update( fDelta );
}

void BootState::OnEnd()
{

}
