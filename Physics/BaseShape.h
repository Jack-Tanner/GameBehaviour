#pragma once

#include <glm/glm.hpp>

class BaseShape
{
public:
	BaseShape();
	virtual ~BaseShape();

	enum Type
	{
		eNone = 0,
		eAABB,
		eOOBB,	// OOBB
		eCircle,
		eMax
	};

	Type	GetType() const
	{
		return m_eType;
	}

	virtual void SetPositionRotation( const glm::vec2& vPosition, float fAngle ) = 0;
	virtual void SetPosition( const glm::vec2& vPosition ) = 0;
	virtual void SetSize( const glm::vec2& vSize ) = 0;
	virtual void SetRotation( float fAngle ) = 0;

	unsigned int GetID() const
	{
		return m_ID;	
	}

protected:
	Type	m_eType;

private:
	friend class RigidBody;
	unsigned int m_ID;

};

