#include "OOBBShape.h"

#define M_PI 3.14159265358979323846

OOBBShape::OOBBShape( const glm::vec2 & vCenter, const glm::vec2 & vSize, float fAngle )
{
	m_eType = Type::eOOBB;
	m_vCenter = vCenter;
	m_vSize = vSize * 0.5f;
	m_fAngle = fAngle;
	Reshape();
}

OOBBShape::~OOBBShape()
{
}

void OOBBShape::ComputeAxes()
{
	// Find 2D axis.
	m_Axis[0] = m_Corners[1] - m_Corners[0];
	m_Axis[1] = m_Corners[3] - m_Corners[0];

	auto a = m_Axis[0].length();
		
	// Normalize.
	m_Axis[0] = glm::normalize( m_Axis[0] );
	m_Axis[1] = glm::normalize( m_Axis[1] );

}

void OOBBShape::SetPositionRotation( const glm::vec2 & vPosition, float fAngle )
{
	m_vCenter = vPosition;
	m_fAngle = fAngle;
	Reshape();
}

void OOBBShape::SetPosition( const glm::vec2 & vCenter )
{
	m_vCenter = vCenter;
	
	// Translate our corners.
	glm::vec2 vCentroid = ( m_Corners[0] + m_Corners[1] + m_Corners[2] + m_Corners[3] ) * 0.25f;
	glm::vec2 vTranslation = vCenter - vCentroid;

	for ( unsigned int it = 0; it < 4; it++ )
	{
		m_Corners[it] += vTranslation;
	}

	ComputeAxes();
}

void OOBBShape::SetSize( const glm::vec2 & vSize )
{
	m_vSize = vSize * 0.5f;
	Reshape();
}

void OOBBShape::SetRotation( float fAngle )
{
	m_fAngle = fAngle;
	Reshape();
}

void OOBBShape::Reshape()
{
	// Apply rotation.
	glm::vec2 xAxis( cosf( m_fAngle ), sinf( m_fAngle ) );
	glm::vec2 yAxis( -sinf( m_fAngle ), cos( m_fAngle ) );

	xAxis *= m_vSize.x;
	yAxis *= m_vSize.y;

	// Precompute corners
	m_Corners[0] = m_vCenter - xAxis - yAxis;
	m_Corners[1] = m_vCenter + xAxis - yAxis;
	m_Corners[2] = m_vCenter + xAxis + yAxis;
	m_Corners[3] = m_vCenter - xAxis + yAxis;

	ComputeAxes();
}

