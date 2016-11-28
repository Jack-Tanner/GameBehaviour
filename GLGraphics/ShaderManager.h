#pragma once

#include <GL/glew.h>
#include <vector>

#include "BaseShader.h"


class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	BaseShader*	CreateShader( const char* pszShaderName, const char* pszVertexSource, const char* pszFragmentSource );
	BaseShader*	CreateShaderFromFile( const char* pszShaderName, const char* pszVertexFile, const char* pszFragmentSource );
	BaseShader*	GetShader( const char* pszShaderName );
	BaseShader*	GetShader( unsigned int uIndex );

	bool		UseShader( BaseShader* pShader );

	BaseShader* GetCurrentShader() const
	{
		return m_pCurrentShader;
	}

	unsigned int GetNumShaders() const
	{
		return m_Shaders.size();
	}

private:

	std::vector<BaseShader*> m_Shaders;
	BaseShader*				 m_pCurrentShader;

};

