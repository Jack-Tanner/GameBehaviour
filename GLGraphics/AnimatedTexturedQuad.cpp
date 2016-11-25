#include "AnimatedTexturedQuad.h"

#include <SDL.h>
#include <Debug.h>
#include "Renderer.h"
#include "ShaderManager.h"
#include "TextureManager.h"


AnimatedTexturedQuad::AnimatedTexturedQuad() : m_uCurrentFrame( 0 ), m_UVOffsetUniform( GL_INVALID_VALUE ), m_vUVOffsetPerFrame( 0.0f, 0.0f ), m_bAnimSet( false ), m_fFrameDuration( 0.0f ), m_fLastFrameTime( 0.0f )
{
}


AnimatedTexturedQuad::~AnimatedTexturedQuad()
{
}

bool AnimatedTexturedQuad::Initialise( BaseShader * pShader, BaseTexture * pTexture )
{
	GASSERT( pShader && pTexture );
	if ( pShader == nullptr || pTexture == nullptr )
		return false;

	AnimatedTexture* pAnimTexture = dynamic_cast< AnimatedTexture* >( pTexture );
	if ( pAnimTexture == nullptr )
		return false;

	glGenVertexArrays( 1, &m_VAO );
	GL_CHECK();

	if ( m_VAO == 0 )
	{
		GASSERT( false && "Failed to create VAO!" );
		m_bInitialised = false;
		return false;
	}

	GASSERT( pTexture );
	m_pTexture = pTexture;

	// TODO: Don't assume same frame size! Need to update geometry if support is needed.
	const glm::vec2& vFrameSize = pAnimTexture->GetFrameSize();
	m_vUVOffsetPerFrame.x = vFrameSize.x / pAnimTexture->GetWidth();
	m_vUVOffsetPerFrame.y = vFrameSize.y / pAnimTexture->GetHeight();

	glBindVertexArray( m_VAO );
	GL_CHECK();

	float fHalfWidth = vFrameSize.x * 0.5f;
	float fHalfHeight = vFrameSize.y * 0.5f;

	GLfloat vertices[] = {
		-fHalfWidth, fHalfHeight, 1.0f, 0.0f, 0.0f,	// Top-left
		fHalfWidth, fHalfHeight, 1.0f, m_vUVOffsetPerFrame.x, 0.0f,	// Top-right
		fHalfWidth, -fHalfHeight, 1.0f, m_vUVOffsetPerFrame.x, m_vUVOffsetPerFrame.y,	// Bottom-right
		-fHalfWidth, -fHalfHeight, 1.0f, 0.0f, m_vUVOffsetPerFrame.y	// Bottom-left
	};

	m_VBO.Initialise( BufferType::eVertexBuffer, GL_STATIC_DRAW, &vertices[0], sizeof( vertices ) );

	static GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	m_EBO.Initialise( BufferType::eIndexBuffer, GL_STATIC_DRAW, &indices[0], sizeof( indices ) );

	SetShader( pShader );
	GASSERT( pShader != nullptr );

	glBindVertexArray( 0 );
	GL_CHECK();

	m_bInitialised = true;

	return true;
}

bool AnimatedTexturedQuad::Initialise( BaseShader * pShader, const char * pszTexture )
{
	if ( m_pRenderer )
	{
		BaseTexture* pTexture = TextureManager::GetInstance()->GetTexture( pszTexture );
		if ( pShader && pTexture )
		{
			return Initialise( pShader, pTexture );
		}
	}
	return false;
}

bool AnimatedTexturedQuad::Initialise( const char * pszShader, BaseTexture * pTexture )
{
	if ( m_pRenderer )
	{
		ShaderManager& rShaderManager = m_pRenderer->GetShaderManager();
		BaseShader* pShader = rShaderManager.GetShader( pszShader );
		if ( pShader )
		{
			return Initialise( pShader, pTexture );
		}
	}
	return false;
}

bool AnimatedTexturedQuad::Initialise( const char * pszShader, const char * pszTexture )
{
	if ( m_pRenderer )
	{
		ShaderManager& rShaderManager = m_pRenderer->GetShaderManager();
		BaseShader* pShader = rShaderManager.GetShader( pszShader );

		BaseTexture* pTexture = TextureManager::GetInstance()->GetTexture( pszTexture );
		if ( pShader && pTexture )
		{
			return Initialise( pShader, pTexture );
		}
	}
	return false;
}

void AnimatedTexturedQuad::Render()
{
	if ( m_bInitialised && m_bAnimSet )
	{
		if ( m_pRenderer && m_pShader )
		{
			m_pRenderer->GetShaderManager().UseShader( m_pShader );
			m_pShader->SetWorldMatrix( m_WorldMatrix );

			float fCurrentTime = (float)SDL_GetTicks();
			if ( fCurrentTime >= ( m_fLastFrameTime + m_fFrameDuration ) )
			{
				m_uCurrentFrame++;
				m_fLastFrameTime = fCurrentTime;
			}

			if ( m_uCurrentFrame > m_CurrentAnim.m_uEndFrame )
				m_uCurrentFrame = m_CurrentAnim.m_uStartFrame;

			AnimatedTexture* pTexture = ( AnimatedTexture* ) m_pTexture;
			glm::vec2 uvOffset;
			uvOffset.x = m_vUVOffsetPerFrame.x * ( m_uCurrentFrame % pTexture->GetNumCols() );
			uvOffset.y = m_vUVOffsetPerFrame.y * ( m_uCurrentFrame / pTexture->GetNumCols() );

			m_pShader->SetUniform( m_UVOffsetUniform, uvOffset );

		}

		if ( m_pTexture )
			m_pTexture->BindTexture( 0 );

		glBindVertexArray( m_VAO );
		glDrawElements( GL_TRIANGLES, NUM_QUAD_INDICES, GL_UNSIGNED_INT, 0 );
		GL_CHECK();
	}
}

void AnimatedTexturedQuad::SetShader( BaseShader * pShader )
{
	if ( pShader )
	{
		TexturedQuad::SetShader( pShader );
		m_UVOffsetUniform = pShader->GetUniformLocation( "vUVOffset" );
		GASSERT( m_UVOffsetUniform != GL_INVALID_VALUE );
	}
}

void AnimatedTexturedQuad::SetTexture( BaseTexture * pTexture )
{
	if ( pTexture )
	{
		m_VBO.Destroy();
		m_EBO.Destroy();

		if ( m_VAO != 0 )
			glDeleteVertexArrays( 1, &m_VAO );

		m_VAO = 0;

		GL_CHECK();

		Initialise( m_pShader, pTexture );

	}
}

void AnimatedTexturedQuad::Destroy()
{
	TexturedQuad::Destroy();
}

void AnimatedTexturedQuad::PlayAnim( const char * pszName )
{
	AnimatedTexture* pTexture = ( AnimatedTexture* ) m_pTexture;
	m_CurrentAnim = pTexture->GetAnim( pszName );
	m_bAnimSet = true;

	m_fFrameDuration = m_CurrentAnim.m_fAnimDuration / ( float ) ( m_CurrentAnim.m_uEndFrame - m_CurrentAnim.m_uStartFrame );
	m_fLastFrameTime = ( float ) SDL_GetTicks();
}

void AnimatedTexturedQuad::PlayAnim( int uIndex )
{
	AnimatedTexture* pTexture = ( AnimatedTexture* ) m_pTexture;
	m_CurrentAnim = pTexture->GetAnim( uIndex );
	m_bAnimSet = true;
	m_fFrameDuration = m_CurrentAnim.m_fAnimDuration / ( float ) ( m_CurrentAnim.m_uEndFrame - m_CurrentAnim.m_uStartFrame );
	m_fLastFrameTime = ( float ) SDL_GetTicks();
}

void AnimatedTexturedQuad::PlayAnim( AnimClip animClip )
{
	m_CurrentAnim = animClip;
	m_bAnimSet = true;
	m_fFrameDuration = m_CurrentAnim.m_fAnimDuration / ( float ) ( m_CurrentAnim.m_uEndFrame - m_CurrentAnim.m_uStartFrame );
	m_fLastFrameTime = ( float ) SDL_GetTicks();
}
