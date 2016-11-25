#include "RigidBody.h"
#include "PhysicsManager.h"

#include "OOBBShape.h"
#include "AABBShape.h"


RigidBody::RigidBody() : m_pTransform( nullptr ), m_bGravity( false ), m_Velocity( 0.0f, 0.0f ), m_Acceleration( 0.0f, 0.0f ), m_AngularAcceleration( 0.0f ),
		m_AngularVelocity( 0.0f ), m_eType( eStatic ), m_fMass( 0.0f ), m_fFriction( 0.3f ), m_pShape( nullptr ), m_bColliding( false )
{
	auto pPhysicsMan = PhysicsManager::GetInstance();
	if( pPhysicsMan != nullptr )
		pPhysicsMan->AddRigidBody( this );
}


RigidBody::~RigidBody()
{
	auto pPhysicsMan = PhysicsManager::GetInstance();
	if ( pPhysicsMan != nullptr )
		pPhysicsMan->RemoveRigidBody( this );
}

void RigidBody::Step( float fDelta )
{
	if ( IsInitialised() )
	{
		if ( m_eType == eDynamic )
		{
			auto pPhysicsMan = PhysicsManager::GetInstance();

			glm::vec2 vGravity = glm::vec2( 0.0f, 0.0f );
			if ( m_bGravity )
				vGravity = pPhysicsMan->GetGravity();

			// Step velocities.
			m_Velocity += ( m_Acceleration + vGravity ) * fDelta;
			m_AngularVelocity += m_AngularAcceleration * fDelta;

			// Update Transform
			m_pTransform->m_Position += m_Velocity * fDelta;
			m_pTransform->m_Rotation += m_AngularVelocity * fDelta;
		}
		else
		{
			m_Velocity = glm::vec2( 0.0f, 0.0f );
		}

		// Update collider.
		if ( m_pShape )
		{
			m_pShape->SetPositionRotation( m_pTransform->m_Position, m_pTransform->m_Rotation );
		}
	}
}

void RigidBody::Initialise( Transform * pTransform, RigidBodyType eType, float fMass, float fFriction, bool bGravity )
{
	if ( pTransform )
	{
		m_pTransform = pTransform;
		m_eType = eType;
		m_fMass = fMass;
		m_fFriction = fFriction;
		m_bGravity = bGravity;
	}
}

void RigidBody::AddOOBB( const glm::vec2 & vSize )
{
	GASSERT( IsInitialised() );
	if ( m_pShape != nullptr )
		delete m_pShape;

	m_pShape = new OOBBShape( m_pTransform->m_Position, vSize, m_pTransform->m_Rotation );
	m_pShape->m_ID = PhysicsManager::GetInstance()->GetNextShapeID();
}

void RigidBody::AddAABB( const glm::vec2& vSize )
{
	GASSERT( IsInitialised() );
	if ( m_pShape != nullptr )
		delete m_pShape;

	m_pShape = new AABBShape( m_pTransform->m_Position, vSize );
	m_pShape->m_ID = PhysicsManager::GetInstance()->GetNextShapeID();
}
