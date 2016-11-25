#include "World.h"
#include "CollidableSpriteEntity.h"
#include <Debug.h>

#include <string>
#include <fstream>
#include <json.hpp>


World::World() : m_NextID( 0 )
{
	m_Initialisers[EntityType::eSprite] = [ ] () { return (BaseEntity*)(new SpriteEntity()); };
	m_Initialisers[EntityType::eCollidableSprite] = [ ] () { return ( BaseEntity* ) ( new CollidableSpriteEntity() ); };
}

World::~World()
{
	Destroy();
}

void World::Update( float fDelta )
{
	for ( auto& entity : m_EntityList )
	{
		if ( entity->IsInitialised() )
		{
			entity->Update( fDelta );
		}
	}
}

void World::Destroy()
{
	for ( auto& entity : m_EntityList )
	{
		if ( entity )
		{
			entity->Destroy();
			delete entity;
		}
	}

	m_EntityList.clear();
}

void World::AddEntity( BaseEntity * pEntity )
{
	GASSERT( pEntity != nullptr );
	if ( pEntity != nullptr )
	{
		pEntity->m_pOwnerWorld = this;
		m_EntityList.push_back( pEntity );
	}
}

BaseEntity * World::GetEntity( unsigned int uID )
{
	for ( auto& entity : m_EntityList )
	{
		if ( entity && entity->GetID() == uID )
		{
			return entity;
		}
	}

	return nullptr;
}

// TODO: Move somewhere else..
bool World::SerializeOut( const std::string & strPath )
{
	// TODO: Path manager please!!
	std::string strFullPath = "Assets/Levels/";
	strFullPath += strPath;

	json::json jsonRoot;
	for ( auto& it : m_EntityList )
	{
		json::json j;
		it->SerializeOut( j );

		jsonRoot["entities"].push_back( j );
	}

	std::ofstream jsonFile( strFullPath.c_str(), std::ios_base::out | std::ios_base::binary );
	if ( !jsonFile.is_open() )
	{
		GASSERT( false );
		return false;
	}

	jsonFile << jsonRoot;

	jsonFile.close();

	return true;
}

// TODO: Move somewhere else..
bool World::SerializeIn( const std::string & strPath )
{
	// TODO: Path manager please!!
	std::string strFullPath = "Assets/Levels/";
	strFullPath += strPath;

	std::ifstream jsonFile( strFullPath.c_str(), std::ios_base::in | std::ios_base::binary );
	if ( !jsonFile.is_open() )
	{
		GASSERT( false );
		return false;
	}

	jsonFile.seekg( 0, std::ios::end );
	std::size_t fileSize = (std::size_t)jsonFile.tellg();
	jsonFile.seekg( 0, std::ios::beg );

	if ( fileSize == 0 )
	{
		GASSERT( false );
		return false;
	}

	char* pBuffer = new char[fileSize];
	if ( !pBuffer )
	{
		GASSERT( false );
		return false;
	}

	jsonFile.read( pBuffer, fileSize );
	pBuffer[fileSize] = '\0';

	auto j = json::json::parse(pBuffer);
	auto entityKey = j.find( "entities" );
	auto entities = entityKey.value();
	for ( auto& it : entities )
	{
		unsigned int uType = it["type"];
		BaseEntity* pEnt = m_Initialisers[uType]();
		pEnt->SerializeIn( it );
		AddEntity( pEnt );
	}
	
	jsonFile.close();

	return true;
}
