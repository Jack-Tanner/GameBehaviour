#include "TexturedQuad.h"

#include "Renderer.h"
#include "Debug.h"
#include "BaseShader.h"
#include "TextureManager.h"

TexturedQuad::TexturedQuad()
{
}

TexturedQuad::~TexturedQuad()
{
}

bool TexturedQuad::Initialise( BaseShader * pShader, BaseTexture * pTexture )
{
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

	float fHalfWidth = pTexture->GetWidth() * 0.5f;
	float fHalfHeight = pTexture->GetHeight() * 0.5f;

	GLfloat vertices[] = {
		-fHalfWidth,	fHalfHeight,	1.0f, 0.0f, 0.0f,	// Top-left
		fHalfWidth,		fHalfHeight,	1.0f, 1.0f, 0.0f,	// Top-right
		fHalfWidth,		-fHalfHeight,	1.0f, 1.0f, 1.0f,	// Bottom-right
		-fHalfWidth,	-fHalfHeight,	1.0f, 0.0f, 1.0f	// Bottom-left
	};

	m_VBO.Initialise( BufferType::eVertexBuffer, GL_STATIC_DRAW, &vertices[0], sizeof( vertices ) );

	static GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	m_EBO.Initialise( BufferType::eIndexBuffer, GL_STATIC_DRAW, &indices[0], sizeof( indices ) );

	SetShader( pShader );
	GASSERT( pShader != nullptr );

	SetTexture( pTexture );
	GASSERT( pTexture );

	glBindVertexArray( 0 );
	GL_CHECK();

	m_bInitialised = true;

	return true;

}

bool TexturedQuad::Initialise( const char * pszShaderName, BaseTexture * pTexture )
{
	if ( m_pRenderer )
	{
		ShaderManager& rShaderManager = m_pRenderer->GetShaderManager();
		BaseShader* pShader = rShaderManager.GetShader( pszShaderName );
		if ( pShader )
		{
			return Initialise( pShader, pTexture );
		}
	}
	return false;
}

bool TexturedQuad::Initialise( const char * pszShaderName, const char * pszTextureName )
{
	if ( m_pRenderer )
	{
		ShaderManager& rShaderManager = m_pRenderer->GetShaderManager();
		BaseShader* pShader = rShaderManager.GetShader( pszShaderName );

		BaseTexture* pTexture = TextureManager::GetInstance()->GetTexture( pszTextureName );
		if ( pShader && pTexture )
		{
			return Initialise( pShader, pTexture );
		}
	}
	return false;
}

void TexturedQuad::Render()
{
	if ( m_bInitialised )
	{
		if ( m_pRenderer && m_pShader )
		{
			m_pRenderer->GetShaderManager().UseShader( m_pShader );
			m_pShader->SetWorldMatrix( m_WorldMatrix );
		}

		if ( m_pTexture )
			m_pTexture->BindTexture( 0 );

		glBindVertexArray( m_VAO );
		glDrawElements( GL_TRIANGLES, NUM_QUAD_INDICES, GL_UNSIGNED_INT, 0 );
		GL_CHECK();
	}
}

void TexturedQuad::SetShader( BaseShader * pShader )
{
	if ( pShader != nullptr )
	{
		m_pShader = pShader;
		m_pRenderer->GetShaderManager().UseShader( pShader );

		// TODO: Try and match up streams so we can use shaders that have mismatching inputs to data.
		// Needs to have a better idea of the data we are trying to use.
		GLuint uStreamsFound = 0;
		const AttributeDescriptor* pPositionAttrib = pShader->GetAttribute( ShaderAttribute::ePosition );
		const AttributeDescriptor* pUVAttrib = pShader->GetAttribute( ShaderAttribute::eTexcoord );
		if ( pPositionAttrib )
		{
			// Enable position stream.
			glEnableVertexAttribArray( pPositionAttrib->m_Location );
			glVertexAttribPointer( pPositionAttrib->m_Location, 3, GL_FLOAT, GL_FALSE, sizeof( GLfloat ) * 5, 0 );

			GL_CHECK();
			uStreamsFound++;
		}

		if ( pUVAttrib )
		{
			// Enable colour stream.
			glEnableVertexAttribArray( pUVAttrib->m_Location );
			glVertexAttribPointer( pUVAttrib->m_Location, 2, GL_FLOAT, GL_FALSE, sizeof( GLfloat ) * 5, ( void* ) ( 3 * sizeof( GLfloat ) ) );

			GL_CHECK();
			uStreamsFound++;
		}

		GASSERT( ( uStreamsFound > 0 ) && "Failed to match any streams!" );
	}
}

void TexturedQuad::SetTexture( BaseTexture * pTexture )
{
	if ( pTexture != nullptr )
	{
		m_pTexture = pTexture;
	}
}

void TexturedQuad::Destroy()
{
	Quad::Destroy();
}
