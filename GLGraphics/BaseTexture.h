#pragma once

#include <GL/glew.h>

#include <string>

class BaseTexture
{
public:
	BaseTexture();
	virtual ~BaseTexture();

	virtual bool	Initialise( const std::string& strFilePath );
	virtual void	BindTexture( GLuint uLocation );
	virtual void	Destroy();


	GLuint	GetTexHandle() const
	{
		return	m_TexHandle;
	}

	bool	IsInitialised() const
	{
		return m_bInitialised;
	}

	GLsizei	GetWidth() const
	{
		return m_Width;
	}

	GLsizei	GetHeight() const
	{
		return m_Height;
	}

	const std::string& GetFileName() const
	{
		return m_FileName;
	}


protected:
	GLuint	m_TexHandle;
	GLsizei	m_Width, m_Height;

private:
	bool	m_bInitialised;
	std::string	m_FileName;

};

