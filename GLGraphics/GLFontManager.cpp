#include "GLFontManager.h"

#include <Debug.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Renderer.h"


GLFontManager::GLFontManager() : m_pTextShader( nullptr ), m_VAO( 0 ), m_uFontColourUniform( GL_INVALID_VALUE )
{
}


GLFontManager::~GLFontManager()
{
}

bool GLFontManager::Initialise()
{
	glGenVertexArrays( 1, &m_VAO );
	glBindVertexArray( m_VAO );


	GLfloat vertices[] = {
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 0.0f, 1.0f
	};


	if ( !m_VBO.Initialise( BufferType::eVertexBuffer, GL_DYNAMIC_DRAW, &vertices[0], sizeof( vertices ) ) )
	{
		GASSERT( false );
		return false;
	}

	static GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	m_EBO.Initialise( BufferType::eIndexBuffer, GL_STATIC_DRAW, &indices[0], sizeof( indices ) );

	GL_CHECK();

	m_pTextShader = Renderer::GetInstance()->GetShaderManager().CreateShaderFromFile( "TextShader", "TextShader_Vert.glsl", "TextShader_Frag.glsl" );
	if ( m_pTextShader == nullptr )
	{
		GASSERT( m_pTextShader != nullptr );
		return false;
	}

	m_VBO.BindBuffer();
	m_EBO.BindBuffer();

	Renderer::GetInstance()->GetShaderManager().UseShader( m_pTextShader );

	const AttributeDescriptor* pPositionAttrib = m_pTextShader->GetAttribute( ShaderAttribute::ePosition );
	const AttributeDescriptor* pUVAttrib = m_pTextShader->GetAttribute( ShaderAttribute::eTexcoord );
	if ( pPositionAttrib )
	{
		// Enable position stream.
		glEnableVertexAttribArray( pPositionAttrib->m_Location );
		glVertexAttribPointer( pPositionAttrib->m_Location, 3, GL_FLOAT, GL_FALSE, sizeof( GLfloat ) * 5, 0 );

		GL_CHECK();
	}

	if ( pUVAttrib )
	{
		// Enable colour stream.
		glEnableVertexAttribArray( pUVAttrib->m_Location );
		glVertexAttribPointer( pUVAttrib->m_Location, 2, GL_FLOAT, GL_FALSE, sizeof( GLfloat ) * 5, ( void* ) ( 3 * sizeof( GLfloat ) ) );

		GL_CHECK();
	}

	m_pTextShader->SetProjectionMatrix( glm::orthoLH( -640.0f, 640.0f, -360.0f, 360.0f, 1.0f, 1000.0f ) );
	m_pTextShader->SetUniform( m_pTextShader->GetUniformLocation( "FontGlyph" ), 0 );
	m_uFontColourUniform = m_pTextShader->GetUniformLocation( "FontColour" );


	glBindVertexArray( 0 );

	return true;
}

GLFont * GLFontManager::AddFont( const char * pszFont, unsigned int uSize )
{
	GLFont* pFont = new GLFont();
	if ( pFont->Initialise( pszFont, uSize ) )
	{
		m_Fonts.push_back( pFont );

		return pFont;
	}

	return nullptr;
}

GLFont * GLFontManager::GetFont( const char * pszFont, unsigned int uSize )
{
	for ( auto& pFont : m_Fonts )
	{
		if ( pFont->GetName() == pszFont && pFont->GetSize() == uSize )
		{
			return pFont;
		}
	}

	return nullptr;
}

void GLFontManager::PrintString( GLFont * pFont, const std::string & strText, float x, float y, glm::vec4 vColour )
{
	if ( pFont != nullptr && strText.length() > 0 && m_pTextShader != nullptr )
	{
		
		Renderer::GetInstance()->GetShaderManager().UseShader( m_pTextShader );
		m_pTextShader->SetUniform( m_uFontColourUniform, vColour );

		glActiveTexture( GL_TEXTURE0 );
		glBindVertexArray( m_VAO );

		std::string::const_iterator c;
		for ( c = strText.begin(); c != strText.end(); c++ )
		{
			auto pCharacter = pFont->GetCharacter( *c );
			if ( pCharacter )
			{
				GL_CHECK();

				float xPos = x + pCharacter->vBearing.x;
				float yPos = y - ( pCharacter->vSize.y - pCharacter->vBearing.y );
				float fWidth = ( float ) pCharacter->vSize.x;
				float fHeight = ( float ) pCharacter->vSize.y;

				GLfloat vertices[] = {
					xPos,			yPos + fHeight,	1.0f, 0.0f, 0.0f,
					xPos + fWidth,	yPos + fHeight,	1.0f, 1.0f, 0.0f,
					xPos + fWidth,	yPos,			1.0f, 1.0f, 1.0f,
					xPos,			yPos,			1.0f, 0.0f, 1.0f
				};

				m_EBO.BindBuffer();
				glBindTexture( GL_TEXTURE_2D, pCharacter->uTexHandle );
				GL_CHECK();

				m_VBO.UpdateSubData( &vertices[0], sizeof( vertices ) );
				GL_CHECK();

				m_VBO.BindBuffer();
				glDrawElements( GL_TRIANGLES, NUM_QUAD_INDICES, GL_UNSIGNED_INT, 0 );
				GL_CHECK();

				x += ( pCharacter->uAdvance >> 6 );
			}
		}

		glBindVertexArray( 0 );
		glBindTexture( GL_TEXTURE_2D, 0 );

	}
}

void GLFontManager::Shutdown()
{
	GL_CHECK();
	for ( auto& pFont : m_Fonts )
	{
		delete pFont;
	}

	m_Fonts.clear();

	m_VBO.Destroy();
	m_EBO.Destroy();
	glDeleteVertexArrays( 1, &m_VAO );

}
