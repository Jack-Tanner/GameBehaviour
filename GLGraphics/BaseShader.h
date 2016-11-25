#pragma once

#include <GL\glew.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>

enum class ShaderAttribute : unsigned int
{
	ePosition = 0,
	eTexcoord,
	eNormal,
	eVertexColour,
	eTangent,
	eBinormal,

	eAttributeMax

};

struct AttributeDescriptor
{
	GLint			m_Location;
	GLint			m_Size;
	GLint			m_Count;
	GLenum			m_Type;
	std::string		m_Name;
	GLsizei			m_NameLength;
	ShaderAttribute	m_AttributeType;
};

struct UniformDescriptor
{
	GLint			m_Location;
	GLsizei			m_NameLength;
	GLenum			m_Type;
	GLint			m_Count;
	std::string		m_Name;
};

class BaseShader
{
public:

	friend class ShaderManager;
	BaseShader();
	~BaseShader();

	bool	Initialise( const char* pszShaderName, const char* pszVertexSource, const char* pszFragmentSource );

	bool	IsInitialised() const
	{
		return m_bInitialised;
	}

	void	Destroy();

	// Attribute Methods
	GLint	GetAttributeLocation( const char* pszName );
	const AttributeDescriptor*	GetAttribute( const char* pszName ) const;
	const AttributeDescriptor*	GetAttribute( GLuint uLocation ) const;
	const AttributeDescriptor*	GetAttribute( ShaderAttribute uAttribute ) const;

	// Uniform Methods
	GLint	GetUniformLocation( const char* pszName );

	void	SetUniform( GLuint uUniform, glm::mat4& matMatrix );
	void	SetUniform( GLuint uUniform, glm::vec2& vVector );
	void	SetUniform( GLuint uUniform, glm::vec4& vVector );
	void	SetUniform( GLuint uUniform, GLuint uValue );

	void	SetWorldMatrix( glm::mat4& matWorld );
	void	SetViewMatrix( glm::mat4& matView );
	void	SetProjectionMatrix( glm::mat4& matProj );

	std::string	GetName() const
	{
		return m_ShaderName;
	}

protected:

	void	Use();

	UniformDescriptor* FindUniform( const char* pszName );
	AttributeDescriptor* FindAttribute( const char* pszName );

	bool	m_bInitialised;

	GLuint	m_VertexShader;
	GLuint	m_FragmentShader;

	std::vector< AttributeDescriptor > m_Attributes;
	std::vector< UniformDescriptor > m_Uniforms;

	GLuint	m_ShaderProgram;

	std::string		m_ShaderName;

	// Cache re-used uniform locations.
	GLuint	m_WorldLocation;
	GLuint	m_ViewLocation;
	GLuint	m_ProjectionLocation;

	bool	m_bInUse;
};

