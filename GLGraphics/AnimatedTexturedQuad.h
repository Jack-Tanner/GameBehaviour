#pragma once
#include "TexturedQuad.h"
#include <glm\glm.hpp>
#include <vector>

#include "AnimatedTexture.h"

class AnimatedTexturedQuad :
	public TexturedQuad
{
public:
	AnimatedTexturedQuad();
	virtual ~AnimatedTexturedQuad();

	bool	Initialise( BaseShader* pShader, BaseTexture* pTexture );
	bool	Initialise( BaseShader* pShader, const char* pszTexture );
	bool	Initialise( const char* pszShader, BaseTexture* pTexture );
	bool	Initialise( const char* pszShader, const char* pszTexture );

	virtual void Render();

	virtual void SetShader( BaseShader* pShader );
	virtual void SetTexture( BaseTexture* pTexture );

	virtual void	Destroy();

	unsigned int GetCurrentFrame() const
	{
		return m_uCurrentFrame;
	}

	const AnimClip& GetCurrentAnim() const
	{
		return m_CurrentAnim;
	}

	void	PlayAnim( const char* pszName );
	void	PlayAnim( int uIndex );
	void	PlayAnim( AnimClip animClip );

protected:
	unsigned int	m_uCurrentFrame;
	GLint			m_UVOffsetUniform;
	float			m_fFrameDuration;
	float			m_fLastFrameTime;
	bool			m_bAnimSet;

	glm::vec2		m_vUVOffsetPerFrame;
	AnimClip		m_CurrentAnim;
};

