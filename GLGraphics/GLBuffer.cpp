#include "GLBuffer.h"

#include "Debug.h"

GLenum MapBufferTypeToEnum( BufferType eBufferType )
{
	switch ( eBufferType )
	{
	case BufferType::eVertexBuffer:
		return GL_ARRAY_BUFFER;
		break;
	case BufferType::eIndexBuffer:
		return GL_ELEMENT_ARRAY_BUFFER;
		break;
	case BufferType::eUniformBuffer:
		return GL_UNIFORM_BUFFER;
		break;
	case BufferType::eMax:
	case BufferType::eNone:
	default:
		GASSERT( false && "Unknown type" );
		break;
	}

	return GL_INVALID_ENUM;
}


GLBuffer::GLBuffer() : m_eBufferType( BufferType::eNone ), m_uHandle( 0 )
{
}


GLBuffer::~GLBuffer()
{
	if ( m_uHandle != 0 )
	{
		Destroy();
	}
}

bool GLBuffer::Initialise( BufferType eBufferType, GLenum eUsage, void * pInitialData, GLsizei iInitialSize )
{
	if ( eBufferType != BufferType::eNone && eBufferType != BufferType::eMax )
	{
		glGenBuffers( 1, &m_uHandle );
		GL_CHECK();

		if ( m_uHandle != 0 )
		{
			m_eBufferType = eBufferType;

			if ( pInitialData != nullptr && iInitialSize > 0 )
			{
				BindBuffer();

				GLenum eType = MapBufferTypeToEnum( m_eBufferType );
				GASSERT( eType != GL_INVALID_ENUM );

				glBufferData( eType, iInitialSize, pInitialData, eUsage );
				GL_CHECK();
			}

			return true;
		}
	}

	GASSERT( false );
	return false;
}

bool GLBuffer::BindBuffer()
{
	if ( m_uHandle != 0 )
	{
		GLenum eType = MapBufferTypeToEnum( m_eBufferType );
		GASSERT( eType != GL_INVALID_ENUM );

		glBindBuffer( eType, m_uHandle );
		GL_CHECK();

		return true;
	}

	return false;
}

void GLBuffer::Destroy()
{
	if ( m_uHandle != 0 )
	{
		GL_CHECK();
		glDeleteBuffers( 1, &m_uHandle );
		GL_CHECK();

		m_uHandle = 0;
	}

	m_eBufferType = BufferType::eNone;
}

void GLBuffer::UpdateSubData( void * pData, GLsizeiptr iSize, GLintptr iOffset )
{
	if ( m_uHandle != 0 )
	{
		GLenum eType = MapBufferTypeToEnum( m_eBufferType );
		GASSERT( eType != GL_INVALID_ENUM );
		glBindBuffer( eType, m_uHandle );
		GL_CHECK();
		glBufferSubData( eType, iOffset, iSize, pData );
		GL_CHECK();
		glBindBuffer( eType, 0 );
		GL_CHECK();
	}
}
