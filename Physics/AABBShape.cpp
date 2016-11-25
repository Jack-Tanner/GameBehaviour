#include "AABBShape.h"
#include <Debug.h>

AABBShape::AABBShape( glm::vec2 vPosition, glm::vec2 vSize )
{
	m_eType = Type::eAABB;
	m_Position = vPosition;
	m_Size = vSize * 0.5f;
}

AABBShape::~AABBShape()
{
}

void AABBShape::SetPositionRotation( const glm::vec2 & vPosition, float fAngle )
{
	m_Position = vPosition;
}

void AABBShape::SetPosition( const glm::vec2 & vPosition )
{
	m_Position = vPosition;
}

void AABBShape::SetSize( const glm::vec2 & vSize )
{
	m_Size = vSize * 0.5f;
}

void AABBShape::SetRotation( float fAngle )
{
	// Do nothing.
}
