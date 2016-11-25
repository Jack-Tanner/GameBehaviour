#include "TextureManager.h"

#include "AnimatedTexture.h"
#include "Debug.h"

TextureManager* TextureManager::m_pInstance = nullptr;

TextureManager * TextureManager::GetInstance()
{
	if ( m_pInstance == nullptr )
	{
		m_pInstance = new TextureManager();
	}

	return m_pInstance;
}

BaseTexture * TextureManager::AddTexture( const std::string & strFileName )
{	
	if ( DoesTextureExist( strFileName ) )
	{
		return m_Textures[strFileName];
	}
	else
	{
		BaseTexture* pNewTexture = new BaseTexture();

		std::string strFullPath = "Assets/Textures/" + strFileName;
		if ( pNewTexture->Initialise( strFullPath ) )
		{
			m_Textures[strFileName] = pNewTexture;
			return pNewTexture;
		}

		GASSERT( false && "Failed to load file!" );
		delete pNewTexture;
	}

	return nullptr;
}

AnimatedTexture * TextureManager::AddAnimatedTexture( const std::string & strFileName, const glm::vec2 & vFrameSize )
{
	if ( DoesTextureExist( strFileName ) )
	{
		return ( AnimatedTexture* ) m_Textures[strFileName];
	}
	else
	{
		AnimatedTexture* pNewTexture = new AnimatedTexture();

		std::string strFullPath = "Assets/Textures/" + strFileName;
		if ( pNewTexture->Initialise( strFullPath, vFrameSize ) )
		{
			m_Textures[strFileName] = pNewTexture;
			return pNewTexture;
		}

		GASSERT( false && "Failed to load file!" );
		delete pNewTexture;
	}

	return nullptr;
}

BaseTexture * TextureManager::GetTexture( const std::string & strFileName )
{
	if ( DoesTextureExist( strFileName ) )
	{
		return m_Textures[strFileName];
	}

	return nullptr;
}

BaseTexture * TextureManager::GetTexture( GLuint uTexHandle )
{
	for ( auto& texture : m_Textures )
	{
		if ( texture.second && texture.second->GetTexHandle() == uTexHandle )
		{
			return texture.second;
		}
	}

	return nullptr;
}

void TextureManager::RemoveTexture( const std::string & strFileName )
{
	if ( DoesTextureExist( strFileName ) )
	{
		m_Textures[strFileName]->Destroy();
		delete m_Textures[strFileName];
		m_Textures.erase( strFileName );
	}
}

void TextureManager::RemoveTexture( GLuint uTexHandle )
{
	for ( auto& texture : m_Textures )
	{
		if ( texture.second && texture.second->GetTexHandle() == uTexHandle )
		{
			texture.second->Destroy();
			delete texture.second;
			m_Textures.erase( texture.first );
		}
	}
}

bool TextureManager::DoesTextureExist( const std::string & strFileName )
{
	std::map<std::string, BaseTexture*>::const_iterator it = m_Textures.find( strFileName );
	return it != m_Textures.end();
}

TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
	for ( auto& texture : m_Textures )
	{
		if ( texture.second )
		{
			texture.second->Destroy();
			delete texture.second;
		}
	}
}
