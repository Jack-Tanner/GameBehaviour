#pragma once

#include <GL/glew.h>

enum class BufferType : unsigned int
{
	eNone = 0,
	eVertexBuffer, 
	eIndexBuffer,
	eUniformBuffer,
	eMax
};

class GLBuffer
{
public:
	GLBuffer();
	~GLBuffer();

	bool	Initialise( BufferType eBufferType, GLenum eUsage, void* pInitialData = nullptr, GLsizei iInitialSize = 0 );
	bool	BindBuffer();
	void	Destroy();

	void	UpdateSubData( void* pData, GLsizeiptr iSize, GLintptr iOffset = 0 );

	BufferType GetBufferType() const
	{
		return m_eBufferType;
	}

private:
	
	BufferType		m_eBufferType;
	GLuint			m_uHandle;
};

