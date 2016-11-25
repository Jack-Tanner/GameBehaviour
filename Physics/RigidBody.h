#pragma once

#include <glm/glm.hpp>
#include <Debug.h>

#include "Transform.h"
#include "BaseShape.h"


enum RigidBodyType
{
	eStatic = 0,	// Never moves
	eKinematic,		// Physics not applied, only collision.
	eDynamic		// Full physics applied.
};

class RigidBody
{
public:
	RigidBody();
	~RigidBody();

	void Step( float fDelta );

	void Initialise( Transform* pTransform, RigidBodyType eType, float fMass, float fFriction, bool bGravity = true );

	bool IsInitialised() const
	{
		return ( m_pTransform != nullptr );
	}

	void SetGravityEnabled( bool bYesNo )
	{
		m_bGravity = bYesNo;
	}

	RigidBodyType GetBodyType() const
	{
		return m_eType;
	}

	void AddOOBB( const glm::vec2& vSize );
	void AddAABB( const glm::vec2& vSize );

	BaseShape* GetShape() const
	{
		return m_pShape;
	}

	void SetColliding( bool bYesNo )
	{
		m_bColliding = bYesNo;
	}

	bool IsColliding() const
	{
		return m_bColliding;
	}

	Transform*	GetTransform()
	{
		return m_pTransform;
	}

	bool		m_bGravity;
	glm::vec2	m_Velocity;
	glm::vec2	m_Acceleration;
	float		m_AngularAcceleration;
	float		m_AngularVelocity;
	float		m_fMass;
	float		m_fFriction;
private:
	Transform*	m_pTransform;
	BaseShape*	m_pShape;
	RigidBodyType	m_eType;

	bool		m_bColliding;
	
};

