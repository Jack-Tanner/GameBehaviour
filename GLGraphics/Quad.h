#pragma once
#include "Renderable.h"
#include "GLBuffer.h"

#define NUM_QUAD_INDICES 6


class BaseShader;

class Quad : public Renderable
{
public:
	Quad();
	virtual ~Quad();

	bool	Initialise( BaseShader* pShader, float x = 0.0f, float y = 0.0f, float z = 1.0f, float fWidth = 1.0f, float fHeight = 1.0f, glm::vec4 vColour = glm::vec4( 1.0f ) );
	bool	Initialise( const char* pszShaderName, float x = 0.0f, float y = 0.0f, float z = 1.0f, float fWidth = 1.0f, float fHeight = 1.0f, glm::vec4 vColour = glm::vec4( 1.0f ) );

	virtual void	Render();
	virtual void	Destroy();

	virtual void	SetShader( BaseShader* pShader );
	BaseShader*		GetShader()
	{
		return m_pShader;
	}

protected:

	GLuint		m_VAO;
	GLBuffer	m_VBO;
	GLBuffer	m_EBO;

	BaseShader*	m_pShader;
};

