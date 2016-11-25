#pragma once

#include "Quad.h"
#include "BaseTexture.h"

class TexturedQuad : public Quad
{
public:
	TexturedQuad();
	virtual ~TexturedQuad();

	bool Initialise( BaseShader* pShader, BaseTexture* pTexture );
	bool Initialise( const char* pszShaderName, BaseTexture* pTexture );
	bool Initialise( const char* pszShaderName, const char* pszTextureName );

	virtual void Render();

	virtual void SetShader( BaseShader* pShader );
	virtual void SetTexture( BaseTexture* pTexture );

	virtual void	Destroy();

	BaseTexture* GetTexture() const
	{
		return m_pTexture;
	}

protected:
	BaseTexture* m_pTexture;

};

