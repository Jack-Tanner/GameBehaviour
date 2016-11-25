#include "ShaderManager.h"
#include "BaseFile.h"



ShaderManager::ShaderManager() : m_pCurrentShader( nullptr )
{
}


ShaderManager::~ShaderManager()
{
	for ( auto shader : m_Shaders )
	{
		shader->Destroy();
		delete shader;
	}
}

BaseShader * ShaderManager::CreateShader( const char * pszShaderName, const char * pszVertexSource, const char * pszFragmentSource )
{
	BaseShader* newShader = new BaseShader();
	if ( newShader && newShader->Initialise( pszShaderName, pszVertexSource, pszFragmentSource ) )
	{
		m_Shaders.push_back( newShader );
		return newShader;
	}

	return nullptr;
}

BaseShader * ShaderManager::CreateShaderFromFile( const char * pszShaderName, const char * pszVertexFile, const char * pszFragmentSource )
{
	std::string strVertPath = "Assets/Shaders/";
	strVertPath += pszVertexFile;

	BaseFile file( strVertPath );
	auto vertFile = file.Read();

	std::string strFragPath = "Assets/Shaders/";
	strFragPath += pszFragmentSource;

	file.OpenFile( strFragPath );
	auto fragFile = file.Read();

	if ( vertFile && fragFile )
	{
		return CreateShader( pszShaderName, vertFile.get(), fragFile.get() );
	}

	return nullptr;
}

BaseShader * ShaderManager::GetShader( const char * pszShaderName )
{
	for ( auto shader : m_Shaders )
	{
		if ( shader && shader->GetName() == pszShaderName )
		{
			return shader;
		}
	}

	return nullptr;
}

bool ShaderManager::UseShader( BaseShader * pShader )
{
	if ( pShader != nullptr )
	{
		if ( pShader != m_pCurrentShader && pShader->IsInitialised() )
		{
			m_pCurrentShader = pShader;
			m_pCurrentShader->Use();
			return true;
		}
	}

	return false;
}
