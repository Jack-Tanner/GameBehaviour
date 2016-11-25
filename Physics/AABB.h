#pragma once

#include <glm/glm.hpp>

struct AABB
{
	glm::vec2 m_CenterPoint;
	glm::vec2 m_Size;

	AABB() : AABB( glm::vec2( 0.0f, 0.0f ), glm::vec2( 1.0f, 1.0f ) )
	{
	}

	AABB( glm::vec2 vPosition, glm::vec2 vSize )
	{
		m_CenterPoint = vPosition;
		m_Size = vSize * 0.5f;
	}
};

inline bool AABBvsAABB( const AABB*a, const AABB* b, glm::vec2& outNormal )
{
	//bool x = std::fabs( a->m_CenterPoint.x - b->m_CenterPoint.x ) <= ( a->m_Size.x + b->m_Size.x );
	//bool y = std::fabs( a->m_CenterPoint.y - b->m_CenterPoint.y ) <= ( a->m_Size.y + b->m_Size.y );
	//return x && y;

	glm::vec2 vNormal = b->m_CenterPoint - a->m_CenterPoint;
	float fXOverlap = a->m_Size.x + b->m_Size.x - std::fabsf( vNormal.x );
	if ( fXOverlap > 0 )
	{
		float fYOverlap = a->m_Size.y + b->m_Size.y - std::fabsf( vNormal.y );
		if ( fYOverlap > 0 )
		{
			if ( fXOverlap < fYOverlap )
			{
				if ( vNormal.x < 0 )
					outNormal = glm::vec2( -1.0f, 0.0f );
				else
					outNormal = glm::vec2( 1.0f, 0.0f );

				return true;
			}
			else
			{
				if ( vNormal.y < 0 )
					outNormal = glm::vec2( 0.0f, -1.0f );
				else
					outNormal = glm::vec2( 0.0f, 1.0f );

				return true;
			}
		}
	}

	return false;
}

