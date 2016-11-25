#include "PhysicsManager.h"
#include <Debug.h>
#include <algorithm>

PhysicsManager* PhysicsManager::m_pInstance = nullptr;


PhysicsManager::PhysicsManager() : m_Gravity( 0.0f, -98.0f ), m_uIDCounter( 0 )
{
	GASSERT( m_pInstance == nullptr );
	m_pInstance = this;

	m_EvaluateFunctions[BaseShape::Type::eAABB][BaseShape::Type::eAABB] = &AABBvsAABB;
	m_EvaluateFunctions[BaseShape::Type::eOOBB][BaseShape::Type::eOOBB] = &OOBBvsOOBB;
}


PhysicsManager::~PhysicsManager()
{
	m_pInstance = nullptr;
}

bool SortManifolds( const Manifold& lhs, const Manifold& rhs )
{
	if ( lhs.pA->GetShape()->GetID() < rhs.pA->GetShape()->GetID() )
		return true;
	if ( lhs.pA->GetShape()->GetID() == rhs.pA->GetShape()->GetID() )
		return lhs.pB->GetShape()->GetID() < rhs.pB->GetShape()->GetID();

	return false;
}

void PhysicsManager::Update( float fFrameDelta )
{
	for ( float dt = 0.0f; dt < fFrameDelta; dt += FIXED_DT )
	{
		// Integrate position & rotation
		for ( auto& body : m_Bodies )
		{
			body->Step( FIXED_DT );
			body->SetColliding( false );
		}

		// Check for collisions
		for ( auto& bodyA : m_Bodies )
		{
			for ( auto& bodyB : m_Bodies )
			{
				if ( bodyA == bodyB )
					continue;

				bool bDoCollision = bodyA->GetBodyType() != RigidBodyType::eStatic || bodyB->GetBodyType() != RigidBodyType::eStatic;

				BaseShape* pShapeA = bodyA->GetShape();
				BaseShape* pShapeB = bodyB->GetShape();

				if ( bDoCollision && pShapeA && pShapeB )
				{
					BaseShape::Type eTypeA = pShapeA->GetType();
					BaseShape::Type eTypeB = pShapeB->GetType();
					Manifold manifold;

					if ( m_EvaluateFunctions[eTypeA][eTypeB]( pShapeA, pShapeB, manifold ) )
					{
						if ( pShapeA->GetID() < pShapeB->GetID() )
						{
							manifold.pA = bodyA;
							manifold.pB = bodyB;
						}
						else
						{
							manifold.pA = bodyB;
							manifold.pB = bodyA;
						}

						m_Manifolds.push_back( std::move( manifold ) );
						bodyA->SetColliding( true );
						bodyB->SetColliding( true );
					}
				}
			}
		}

		// Cull out pairs.
		std::sort( m_Manifolds.begin(), m_Manifolds.end(), SortManifolds );
		
		auto& it = m_Manifolds.begin();
		while( it != m_Manifolds.end() )
		{
			Manifold* pManifold = &( *it );
			m_UniqueManifolds.push_back( &( *it ) );
			it++;

			while( it != m_Manifolds.end() )
			{
				if ( it->pA->GetShape()->GetID() != pManifold->pA->GetShape()->GetID() || it->pB->GetShape()->GetID() != pManifold->pB->GetShape()->GetID() )
				{
					break;
				}
				it++;
			}
		}

		// Impulse collision.
		for ( auto& manifold : m_UniqueManifolds )
		{
			float m1 = manifold->pA->m_fMass;
			float m2 = manifold->pB->m_fMass;

			if ( manifold->pA->GetBodyType() == RigidBodyType::eStatic )
				m1 = FLT_MAX;
			if ( manifold->pB->GetBodyType() == RigidBodyType::eStatic )
				m2 = FLT_MAX;

			float e = 0.45f;
			float m = ( 1.0f / m1 ) + ( 1.0f / m2 );
			glm::vec2 v12 = manifold->pB->m_Velocity - manifold->pA->m_Velocity;
			float vdn = glm::dot( v12, manifold->vNormal );

			if ( vdn > 0.0f )
				continue;

			float j = ( -( 1.0f + e ) * vdn ) / m;
			
			manifold->pA->m_Velocity -= manifold->vNormal * ( j / m1 );
			manifold->pB->m_Velocity += manifold->vNormal * ( j / m2 );

			manifold->pA->m_Velocity += -( glm::normalize( manifold->pA->m_Velocity ) * 1.5f );
			manifold->pB->m_Velocity += -( glm::normalize( manifold->pB->m_Velocity ) * 1.5f );

			if ( manifold->pA->GetBodyType() == RigidBodyType::eDynamic )
			{
				manifold->pA->GetTransform()->m_Position -= manifold->vNormal * manifold->m_fPenetration;
			}
			if ( manifold->pB->GetBodyType() == RigidBodyType::eDynamic )
			{
				manifold->pB->GetTransform()->m_Position += manifold->vNormal * manifold->m_fPenetration;
			}

		}

		m_Manifolds.clear();
		m_UniqueManifolds.clear();

	}
}

void PhysicsManager::AddRigidBody( RigidBody * pRigidBody )
{
	if ( pRigidBody )
	{
		m_Bodies.push_back( pRigidBody );
	}
}

void PhysicsManager::RemoveRigidBody( RigidBody * pRigidBody )
{
	if ( pRigidBody )
	{
		for ( auto it = m_Bodies.begin(); it != m_Bodies.end(); it++ )
		{
			if ( ( *it ) == pRigidBody )
			{
				m_Bodies.erase( it );
				break;
			}
		}
	}
}
