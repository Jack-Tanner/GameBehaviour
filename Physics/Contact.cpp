#include "Contact.h"

#include "AABBShape.h"
#include "OOBBShape.h"

glm::vec2 ProjectCorners( OOBBShape* pShape, const glm::vec2& vAxis )
{
	glm::vec2 vMinMax;
	float t = glm::dot( pShape->GetCorner( 0 ), vAxis );
	vMinMax.x = t;
	vMinMax.y = t;

	bool bFirst = true;
	for ( unsigned int c = 1; c < 4; c++ )
	{
		t = glm::dot( pShape->GetCorner( c ), vAxis );

		if ( t < vMinMax.x )
			vMinMax.x = t;
		else if ( t > vMinMax.y )
			vMinMax.y = t;
	}

	return vMinMax;
}

// TODO: Find contact normal...
bool OOBBvsOOBB( BaseShape* A, BaseShape* B, Manifold& vOutManifold )
{
	if ( A->GetType() != BaseShape::Type::eOOBB || B->GetType() != BaseShape::Type::eOOBB )
	{
		GASSERT( false && "TYPE ERROR!" );
		return false;
	}

	OOBBShape* pA = (OOBBShape*) A;
	OOBBShape* pB = (OOBBShape*) B;

	// Use seperating axis theorem to test if these two boxes collide.
	for ( unsigned int x = 0; x < 2; x++ )
	{
		glm::vec2 vAExtents = ProjectCorners( pA, pA->GetAxis( x ) );
		glm::vec2 vBExtents = ProjectCorners( pB, pA->GetAxis( x ) );

		if ( vAExtents.y < vBExtents.x || vBExtents.y < vAExtents.x )
			return false;

		vAExtents = ProjectCorners( pA, pB->GetAxis( x ) );
		vBExtents = ProjectCorners( pB, pB->GetAxis( x ) );

		if ( vAExtents.y < vBExtents.x || vBExtents.y < vAExtents.x )
			return false;
	}

	return true;
}


bool AABBvsAABB( BaseShape * A, BaseShape * B, Manifold & vOutManifold )
{
	if ( A->GetType() != BaseShape::Type::eAABB || B->GetType() != BaseShape::Type::eAABB )
	{
		GASSERT( false && "TYPE ERROR!" );
		return false;
	}

	AABBShape* pA = (AABBShape*) A;
	AABBShape* pB = (AABBShape*) B;

	const glm::vec2& vAPos = pA->GetPosition();
	const glm::vec2& vASize = pA->GetSize();

	const glm::vec2& vBPos = pB->GetPosition();
	const glm::vec2& vBSize = pB->GetSize();

	vOutManifold.vNormal = pB->GetPosition() - pA->GetPosition();
	//vOutManifold.vNormal = glm::normalize( vOutManifold.vNormal );
	float fXOverlap = vASize.x + vBSize.x - std::fabsf( vOutManifold.vNormal.x );
	if ( fXOverlap > 0 )
	{
		float fYOverlap = vASize.y + vBSize.y - std::fabsf( vOutManifold.vNormal.y );
		if ( fYOverlap > 0 )
		{
			if ( fXOverlap < fYOverlap )
			{
				if ( vOutManifold.vNormal.x < 0 )
					vOutManifold.vNormal = glm::vec2( -1.0f, 0.0f );
				else
					vOutManifold.vNormal = glm::vec2( 1.0f, 0.0f );

				vOutManifold.m_fPenetration = fXOverlap;
				return true;
			}
			else
			{
				if ( vOutManifold.vNormal.y < 0 )
					vOutManifold.vNormal = glm::vec2( 0.0f, -1.0f );
				else
					vOutManifold.vNormal = glm::vec2( 0.0f, 1.0f );

				vOutManifold.m_fPenetration = fYOverlap;
				return true;
			}
		}
	}

	return false;
}
