#pragma once

#include <map>
#include <string>

#include "AnimatedTexture.h"
#include "BaseTexture.h"

class TextureManager
{
public:
	static TextureManager*	GetInstance();

	BaseTexture*		AddTexture( const std::string& strFileName );
	AnimatedTexture*	AddAnimatedTexture( const std::string& strFileName, const glm::vec2& vFrameSize );

	BaseTexture*	GetTexture( const std::string& strFileName );
	BaseTexture*	GetTexture( GLuint uTexHandle );

	void			RemoveTexture( const std::string& strFileName );
	void			RemoveTexture( GLuint uTexHandle );


	bool			DoesTextureExist( const std::string& strFileName );


private:

	TextureManager();
	~TextureManager();


	std::map< std::string, BaseTexture* > m_Textures;

	static TextureManager*	m_pInstance;
};

