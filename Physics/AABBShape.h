#pragma once
#include "BaseShape.h"

class AABBShape : public BaseShape
{
public:
	AABBShape( glm::vec2 vPosition, glm::vec2 vSize );
	virtual ~AABBShape();

	virtual void SetPositionRotation( const glm::vec2& vPosition, float fAngle );
	virtual void SetPosition( const glm::vec2& vPosition );
	virtual void SetSize( const glm::vec2& vSize );
	virtual void SetRotation( float fAngle );

	const glm::vec2& GetPosition() const
	{
		return m_Position;
	}

	const glm::vec2& GetSize() const
	{
		return m_Size;	
	}

private:
	glm::vec2 m_Position;
	glm::vec2 m_Size;
};

