#pragma once
#include "BaseShape.h"
#include <glm/glm.hpp>
#include <Debug.h>

class OOBBShape : public BaseShape
{
public:
	OOBBShape( const glm::vec2& vCenter, const glm::vec2& vSize, float fAngle );
	virtual ~OOBBShape();

	void ComputeAxes();

	virtual void SetPositionRotation( const glm::vec2& vPosition, float fAngle );
	virtual void SetPosition( const glm::vec2& vCenter );
	virtual void SetSize( const glm::vec2& vSize );
	virtual void SetRotation( float fAngle );

	const glm::vec2& GetCorner( unsigned int uIndex ) const
	{
		GASSERT( uIndex < 4 );
		return m_Corners[uIndex];
	}

	const glm::vec2& GetAxis( unsigned int uIndex ) const
	{
		GASSERT( uIndex < 2 );
		return m_Axis[uIndex];
	}

	virtual const glm::vec2& GetSize() const
	{
		return m_vSize;
	}

private:
	void Reshape();
	glm::vec2	m_vCenter;
	glm::vec2	m_vSize;
	glm::vec2	m_Corners[4];
	glm::vec2	m_Axis[2];
	float		m_fAngle;

};

