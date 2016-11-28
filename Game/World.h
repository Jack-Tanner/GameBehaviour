#pragma once

#include <vector>
#include <type_traits>
#include <functional>

#include "BaseEntity.h"

#define INVALID_ENTITY_ID ~0u

class World
{
public:
	World();
	~World();

	void	Update( float fDelta = 1.0f );
	void	Destroy();

	// Serialize in from a file (json)
	bool	SerializeOut( const std::string& strPath );
	bool	SerializeIn( const std::string& strPath );

	void	RemoveEntity( BaseEntity* pEntity );
	void	RemoveEntity( unsigned int uID );

	template< class T >
	T* AddEntity( BaseInitialiser* pInitialiser );
	void		AddEntity( BaseEntity* pEntity );

	BaseEntity*	GetEntity( unsigned int uID );
	const std::vector<BaseEntity*>& GetEntityList() const
	{
		return m_EntityList;
	}

	void SetNextID( unsigned int uNextID )
	{
		m_NextID = uNextID;
	}

	unsigned int GetNextID()
	{
		m_NextID++;
		return m_NextID - 1;
	}

private:
	unsigned int m_NextID;
	std::vector< BaseEntity* > m_EntityList;
	std::function<BaseEntity*()> m_Initialisers[EntityType::eMax];
};

template<class T>
inline T* World::AddEntity(BaseInitialiser* pInitialiser)
{
	static_assert( std::is_base_of<BaseEntity, T>::value, "T must be a BaseEntity!" );

	T* pNewEntity = new T();
	if ( pNewEntity != nullptr )
	{
		pNewEntity->m_pOwnerWorld = this;
		pNewEntity->Initialise(pInitialiser);
		pNewEntity->SetID( m_NextID++ );
		m_EntityList.push_back( static_cast< BaseEntity* >( pNewEntity ) );
	}

	return pNewEntity;
}
