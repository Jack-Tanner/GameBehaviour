#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>

struct GLCharacter
{
	GLuint		uTexHandle;
	GLuint		uAdvance;
	glm::ivec2	vSize;
	glm::ivec2	vBearing;

};

class GLFont
{
public:
	GLFont();
	~GLFont();

	bool	Initialise( const char* pszFontName, unsigned int uFontSize );
	const GLCharacter* GetCharacter( GLubyte uCharacter ) const;

	const std::string& GetName() const
	{
		return m_FontName;
	}

	unsigned int GetSize() const
	{
		return m_uSize;
	}

	unsigned int GetHeight()
	{
		return m_uHeight;
	}

private:
	std::map< GLubyte, GLCharacter > m_Characters;	
	unsigned int m_uSize;
	unsigned int m_uHeight;
	

	std::string	m_FontName;

};

