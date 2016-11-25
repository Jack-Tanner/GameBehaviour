#include "Quad.h"

#include "Renderer.h"
#include "Debug.h"
#include "BaseShader.h"

Quad::Quad() : m_VAO( 0 ), m_pShader( nullptr )
{
}

Quad::~Quad()
{
}

bool Quad::Initialise( BaseShader* pShader, float x, float y, float z,  float fWidth, float fHeight, glm::vec4 vColour )
{
	// TODO: Move this all into a base renderable class.
	glGenVertexArrays( 1, &m_VAO );
	GL_CHECK();

	if ( m_VAO == 0 )
	{
		GASSERT( false && "Failed to create VAO!" );
		m_bInitialised = false;
		return false;
	}

	glBindVertexArray( m_VAO );
	GL_CHECK();

	float fHalfWidth = fWidth * 0.5f;
	float fHalfHeight = fHeight * 0.5f;
	
	GLfloat vertices[] = {
		x - fHalfWidth, y + fHalfHeight, z, vColour.r, vColour.g, vColour.b, vColour.a,	// Top-left
		x + fHalfWidth, y + fHalfHeight, z, vColour.r, vColour.g, vColour.b, vColour.a,	// Top-right
		x + fHalfWidth, y - fHalfHeight, z, vColour.r, vColour.g, vColour.b, vColour.a,	// Bottom-right
		x - fHalfWidth,	y - fHalfHeight, z, vColour.r, vColour.g, vColour.b, vColour.a,	// Bottom-left
	};

	m_VBO.Initialise( BufferType::eVertexBuffer, GL_STATIC_DRAW, &vertices[0], sizeof( vertices ) );


	static GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	m_EBO.Initialise( BufferType::eIndexBuffer, GL_STATIC_DRAW, &indices[0], sizeof( indices ) );

	SetShader( pShader );

	glBindVertexArray( 0 );
	GL_CHECK();

	m_bInitialised = true;

	return true;
}

bool Quad::Initialise( const char * pszShaderName, float x, float y, float z, float fWidth, float fHeight, glm::vec4 vColour )
{
	if ( m_pRenderer )
	{
		ShaderManager& rShaderManager = m_pRenderer->GetShaderManager();
		BaseShader* pShader = rShaderManager.GetShader( pszShaderName );
		if ( pShader )
		{
			return Initialise( pShader, x, y, z, fWidth, fHeight, vColour );
		}
	}
	return false;
}

void Quad::Render()
{
	if ( m_bInitialised )
	{
		if ( m_pRenderer && m_pShader )
		{
			m_pRenderer->GetShaderManager().UseShader( m_pShader );
			m_pShader->SetWorldMatrix( m_WorldMatrix );
		}

		glBindVertexArray( m_VAO );
		glDrawElements( GL_TRIANGLES, NUM_QUAD_INDICES, GL_UNSIGNED_INT, 0 );
		GL_CHECK();
	}
}

void Quad::Destroy()
{
	m_VBO.Destroy();
	m_EBO.Destroy();

	if ( m_VAO != 0 )
		glDeleteVertexArrays( 1, &m_VAO );

	m_VAO = 0;

	GL_CHECK();

	Renderable::Destroy();
}

void Quad::SetShader( BaseShader * pShader )
{
	if ( pShader != nullptr )
	{
		m_pShader = pShader;
		m_pRenderer->GetShaderManager().UseShader( pShader );

		// TODO: Try and match up streams so we can use shaders that have mismatching inputs to data.
		// Needs to have a better idea of the data we are trying to use.
		GLuint uStreamsFound = 0;
		const AttributeDescriptor* pPositionAttrib = pShader->GetAttribute( ShaderAttribute::ePosition );
		const AttributeDescriptor* pVertexColourAttrib = pShader->GetAttribute( ShaderAttribute::eVertexColour );
		if ( pPositionAttrib )
		{
			// Enable position stream.
			glEnableVertexAttribArray( pPositionAttrib->m_Location );
			glVertexAttribPointer( pPositionAttrib->m_Location, 3, GL_FLOAT, GL_FALSE, sizeof( GLfloat ) * 7, 0 );

			GL_CHECK();
			uStreamsFound++;
		}

		if ( pVertexColourAttrib )
		{
			// Enable colour stream.
			glEnableVertexAttribArray( pVertexColourAttrib->m_Location );
			glVertexAttribPointer( pVertexColourAttrib->m_Location, 4, GL_FLOAT, GL_FALSE, sizeof( GLfloat ) * 7, ( void* ) ( 3 * sizeof( GLfloat ) ) );

			GL_CHECK();
			uStreamsFound++;
		}

		GASSERT( ( uStreamsFound > 0 ) && "Failed to match any streams!" );
	}
}
