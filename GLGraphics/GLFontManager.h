#pragma once

#include "GLBuffer.h"
#include "GLFont.h"
#include "BaseShader.h"

#include <string>
#include <vector>

class GLFontManager
{
public:
	GLFontManager();
	~GLFontManager();

	bool	Initialise();

	GLFont* AddFont( const char* pszFont, unsigned int uSize );
	GLFont* GetFont( const char* pszFont, unsigned int uSize );

	void	PrintString( GLFont* pFont, const std::string& strText, float x, float y, glm::vec4 vColour );

	void	Shutdown();


private:
	std::vector< GLFont* > m_Fonts;

	BaseShader* m_pTextShader;

	GLuint	m_VAO;
	GLint	m_uFontColourUniform;
	GLBuffer m_VBO;
	GLBuffer m_EBO;


};

