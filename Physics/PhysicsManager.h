#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Transform.h"
#include "RigidBody.h"

#include "BaseShape.h"
#include "Contact.h"

#define FIXED_DT 0.016f

class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();

	void	Update( float fFrameDelta );

	void	AddRigidBody( RigidBody* pRigidBody );
	void	RemoveRigidBody( RigidBody* pRigidBody );

	void	SetGravity( const glm::vec2& vGravity )
	{
		m_Gravity = vGravity;
	}

	const glm::vec2& GetGravity() const
	{
		return m_Gravity;
	}

	static PhysicsManager* GetInstance()
	{
		return m_pInstance;
	}

	unsigned int GetNextShapeID()
	{
		m_uIDCounter++;
		return (m_uIDCounter - 1);
	}

private:
	static PhysicsManager* m_pInstance;

	std::vector< RigidBody* > m_Bodies;
	
	std::vector< Manifold* > m_UniqueManifolds;
	std::vector< Manifold > m_Manifolds;


	glm::vec2 m_Gravity;
	unsigned int m_uIDCounter;

	EvaluateFunction m_EvaluateFunctions[BaseShape::Type::eMax][BaseShape::Type::eMax];
};

