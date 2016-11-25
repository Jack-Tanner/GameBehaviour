#include "BaseShader.h"
#include "Debug.h"
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>


// Returns the size of a GL type...
GLsizei GetGLTypeSize( GLenum type )
{
	switch ( type )
	{
	case GL_FLOAT:
		return sizeof( GLfloat );
		break;
	case GL_FLOAT_VEC2:
		return sizeof( GLfloat ) * 2;
		break;
	case GL_FLOAT_VEC3:
		return sizeof( GLfloat ) * 3;
		break;
	case GL_FLOAT_VEC4:
		return sizeof( GLfloat ) * 4;
		break;
	case GL_FLOAT_MAT4:
		return sizeof( GLfloat ) * 16;
		break;
	default:
		GASSERT( false && "Non implemented type!" );
	}

	return 0;
}

// Returns a valid type for glVertexAttribPointer...
void GetGLConcreteType( GLenum inputType, GLenum& outputType, GLuint& outputCount )
{
	switch ( inputType )
	{
	case GL_FLOAT:
		outputType = GL_FLOAT;
		outputCount = 1;
		return;
		break;
	case GL_FLOAT_VEC2:
		outputType = GL_FLOAT;
		outputCount = 2;
		return;
		break;
	case GL_FLOAT_VEC3:
		outputType = GL_FLOAT;
		outputCount = 3;
		return;
		break;
	case GL_FLOAT_VEC4:
		outputType = GL_FLOAT;
		outputCount = 4;
		return;
		break;
	default:
		GASSERT( false && "Non implemented type!" );
	}
}

ShaderAttribute AttributeFromString( std::string strAttributeName )
{
	if ( strAttributeName == "position" )
		return ShaderAttribute::ePosition;
	else if ( strAttributeName == "texcoord" || strAttributeName == "uv" )
		return ShaderAttribute::eTexcoord;
	else if ( strAttributeName == "colour" )
		return ShaderAttribute::eVertexColour;
	else if ( strAttributeName == "normal" )
		return ShaderAttribute::eNormal;
	else if ( strAttributeName == "tangent" )
		return ShaderAttribute::eTangent;
	else if ( strAttributeName == "binormal" )
		return ShaderAttribute::eBinormal;
	else
		return ShaderAttribute::eAttributeMax;
}


BaseShader::BaseShader() : m_bInitialised( false ), m_ShaderProgram( 0 ), m_VertexShader( 0 ), m_FragmentShader( 0 ), m_bInUse( false )
{

}

BaseShader::~BaseShader()
{
	if ( m_bInitialised )
		Destroy();
}

bool BaseShader::Initialise( const char* pszShaderName, const char * pszVertexSource, const char * pszFragmentSource )
{
	// Check inputs.
	if ( pszVertexSource == nullptr || pszFragmentSource == nullptr )
		return false;

	// Create vertex shader.
	m_VertexShader = glCreateShader( GL_VERTEX_SHADER );

	if ( m_VertexShader == 0 )
	{
		GLOG( "Failed to create vertex shader!" );
		return false;
	}

	// Compile vertex shader.
	glShaderSource( m_VertexShader, 1, &pszVertexSource, 0 );
	glCompileShader( m_VertexShader );

	GL_CHECK();

	// Create fragment shader.
	m_FragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

	if ( m_VertexShader == 0 )
	{
		GLOG( "Failed to create fragment shader!" );
		return false;
	}

	// Compile fragment shader.
	glShaderSource( m_FragmentShader, 1, &pszFragmentSource, 0 );
	glCompileShader( m_FragmentShader );

	GL_CHECK();

	// Create shader program
	m_ShaderProgram = glCreateProgram();
	if ( m_ShaderProgram == 0 )
	{
		GLOG( "Failed to create shader program!" );
	}
	
	// Assign vertex and fragment shaders then link program.
	glAttachShader( m_ShaderProgram, m_VertexShader );
	GL_CHECK();
	glAttachShader( m_ShaderProgram, m_FragmentShader );
	GL_CHECK();
	glBindFragDataLocation( m_ShaderProgram, 0, "outColor" );
	GL_CHECK();
	glLinkProgram( m_ShaderProgram );
	GL_CHECK();

	// Check for any linker errors.
	GLint isLinked = 0;
	glGetProgramiv( m_ShaderProgram, GL_LINK_STATUS, &isLinked );
	if ( isLinked == GL_FALSE )
	{
		GLint maxLength = 0;
		glGetProgramiv( m_ShaderProgram, GL_INFO_LOG_LENGTH, &maxLength );
		GLchar* pBuffer = new GLchar[maxLength];
		glGetProgramInfoLog( m_ShaderProgram, maxLength, &maxLength, &pBuffer[0] );
		GLOG( "Shader Link Log:\n" << pBuffer );
		delete[]  pBuffer;

		GASSERT( false && "Failed to link shader!" );

		glDeleteProgram( m_ShaderProgram );
		
		return false;
	}

	GLint	PreviousShader = -1;
	glGetIntegerv( GL_CURRENT_PROGRAM, &PreviousShader );

	// Use the new program
	glUseProgram( m_ShaderProgram );
	GL_CHECK();

	// Get all the attributes for this shader program.
	GLint attributeCount = 0;
	GLint attributeTotalSize = 0;

	GLchar	nameBuffer[256];

	glGetProgramiv( m_ShaderProgram, GL_ACTIVE_ATTRIBUTES, &attributeCount );
	for ( GLint i = 0; i < attributeCount; i++ )
	{
		AttributeDescriptor attribDescriptor;
		glGetActiveAttrib( m_ShaderProgram, i, 256, &attribDescriptor.m_NameLength, &attribDescriptor.m_Count, &attribDescriptor.m_Type, &nameBuffer[0] );
		attribDescriptor.m_Name = std::string( ( char* ) &nameBuffer[0], attribDescriptor.m_NameLength );
		
		GLsizei attribTypeSize = GetGLTypeSize( attribDescriptor.m_Type );
		attribTypeSize *= attribDescriptor.m_Count;
		attribDescriptor.m_Size = attribTypeSize;

		attribDescriptor.m_Location = glGetAttribLocation( m_ShaderProgram, attribDescriptor.m_Name.c_str() );

		attribDescriptor.m_AttributeType = AttributeFromString( attribDescriptor.m_Name );

		m_Attributes.push_back( attribDescriptor );

		attributeTotalSize += attribTypeSize;

	}

	// Need to sort the attributes in order of location as glGetActiveAttrib doesn't report attributes in order of location...
	std::sort( m_Attributes.begin(), m_Attributes.end(), [ ] ( const AttributeDescriptor& a, const AttributeDescriptor& b )
	{
		GLuint locationA = a.m_Location;
		GLuint locationB = b.m_Location;
		return locationA < locationB;
	} );

	GL_CHECK();

	// Find all active uniforms.
	GLint uniformCount = 0;
	glGetProgramiv( m_ShaderProgram, GL_ACTIVE_UNIFORMS, &uniformCount );

	for ( GLint i = 0; i < uniformCount; i++ )
	{
		UniformDescriptor attribDescriptor;
		glGetActiveUniform( m_ShaderProgram, i, 256, &attribDescriptor.m_NameLength, &attribDescriptor.m_Count, &attribDescriptor.m_Type, &nameBuffer[0] );
		attribDescriptor.m_Name = std::string( ( char* ) &nameBuffer[0], attribDescriptor.m_NameLength );

		attribDescriptor.m_Location = glGetUniformLocation( m_ShaderProgram, attribDescriptor.m_Name.c_str() );

		m_Uniforms.push_back( attribDescriptor );

		if ( attribDescriptor.m_Name == "matWorld" )
		{
			m_WorldLocation = attribDescriptor.m_Location;
		}
		else if( attribDescriptor.m_Name == "matView" )
		{
			m_ViewLocation = attribDescriptor.m_Location;
		}
		else if ( attribDescriptor.m_Name == "matProjection" )
		{
			m_ProjectionLocation = attribDescriptor.m_Location;
		}

		GL_CHECK();
	}

	glUseProgram( PreviousShader );

	GL_CHECK();

	m_ShaderName = pszShaderName;

	m_bInitialised = true;
	return true;
}

void BaseShader::Use()
{
	if ( m_bInitialised )
	{
		if ( m_ShaderProgram == 0 )
		{
			GLOG( "Trying to use a invalid shader!" );
			return;
		}

		glUseProgram( m_ShaderProgram );
	}
	else
	{
		GLOG( "Trying to use a non-initialised shader!" );
	}
}

void BaseShader::Destroy()
{
	m_bInitialised = false;
	glDeleteProgram( m_ShaderProgram );
	glDeleteShader( m_FragmentShader );
	glDeleteShader( m_VertexShader );
}


GLint BaseShader::GetUniformLocation( const char * pszName )
{
	GLint location = GL_INVALID_VALUE;

	UniformDescriptor* pUniform = FindUniform( pszName );
	if ( pUniform )
	{
		location = pUniform->m_Location;
	}

	GASSERT( location != GL_INVALID_VALUE );

	return location;
}

void BaseShader::SetUniform( GLuint uUniform, glm::mat4 & matMatrix )
{
	if ( uUniform != GL_INVALID_VALUE )
	{
		glUniformMatrix4fv( uUniform, 1, GL_FALSE, glm::value_ptr( matMatrix ) );
	}
}

void BaseShader::SetUniform(GLuint uUniform, glm::vec2 & vVector)
{
	if (uUniform != GL_INVALID_VALUE)
	{
		glUniform2fv(uUniform, 1, glm::value_ptr(vVector));
	}
}

void BaseShader::SetUniform( GLuint uUniform, glm::vec4 & vVector )
{
	if ( uUniform != GL_INVALID_VALUE )
	{
		glUniform4fv( uUniform, 1, glm::value_ptr( vVector ) );
	}
}

void BaseShader::SetUniform( GLuint uUniform, GLuint uValue )
{
	if ( uUniform != GL_INVALID_VALUE )
	{
		glUniform1i( uUniform, uValue );
	}
}

void BaseShader::SetWorldMatrix( glm::mat4 & matWorld )
{
	SetUniform( m_WorldLocation, matWorld );
}

void BaseShader::SetViewMatrix( glm::mat4 & matView )
{
	SetUniform( m_ViewLocation, matView );
}

void BaseShader::SetProjectionMatrix( glm::mat4 & matProj )
{
	SetUniform( m_ProjectionLocation, matProj );
}

UniformDescriptor* BaseShader::FindUniform( const char * pszName )
{
	if ( pszName != nullptr )
	{
		for ( unsigned int i = 0; i < m_Uniforms.size(); i++ )
		{
			const UniformDescriptor& uniform = m_Uniforms.at( i );
			if ( uniform.m_Name == pszName )
			{
				return &m_Uniforms[i];
			}
		}
	}

	return nullptr;
}

GLint BaseShader::GetAttributeLocation( const char* pszName )
{
	GLint location = GL_INVALID_VALUE;

	AttributeDescriptor* pAttribute = FindAttribute( pszName );
	if ( pAttribute )
	{
		location = pAttribute->m_Location;
	}

	return location;
}

const AttributeDescriptor* BaseShader::GetAttribute( const char * pszName ) const
{
	if ( pszName != nullptr )
	{
		for ( unsigned int i = 0; i < m_Attributes.size(); i++ )
		{
			const AttributeDescriptor& attribute = m_Attributes.at( i );
			if ( attribute.m_Name == pszName )
			{
				return &m_Attributes[i];
			}
		}
	}

	return nullptr;
}

const AttributeDescriptor* BaseShader::GetAttribute( GLuint uLocation ) const
{
	for ( unsigned int i = 0; i < m_Attributes.size(); i++ )
	{
		const AttributeDescriptor& attribute = m_Attributes.at( i );
		if ( attribute.m_Location == uLocation )
		{
			return &m_Attributes[i];
		}
	}
	
	return nullptr;
}

const AttributeDescriptor * BaseShader::GetAttribute( ShaderAttribute uAttribute ) const
{
	for ( unsigned int i = 0; i < m_Attributes.size(); i++ )
	{
		const AttributeDescriptor& attribute = m_Attributes.at( i );
		if ( attribute.m_AttributeType == uAttribute )
		{
			return &m_Attributes[i];
		}
	}

	return nullptr;
}

AttributeDescriptor* BaseShader::FindAttribute( const char * pszName )
{
	if ( pszName != nullptr )
	{
		for ( unsigned int i = 0; i < m_Attributes.size(); i++ )
		{
			const AttributeDescriptor& attribute = m_Attributes.at( i );
			if ( attribute.m_Name == pszName )
			{
				return &m_Attributes[i];
			}
		}
	}

	return nullptr;

}
