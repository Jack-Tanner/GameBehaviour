#pragma once

#include "ShaderManager.h"
#include "Renderable.h"
#include "GLFontManager.h"

#include "Quad.h"

#include <vector>

enum DebugPrimitiveType
{
	eLine,
	eQuad
};

struct DebugPrimitive
{
	DebugPrimitiveType m_eType;
};

struct DebugLine : public DebugPrimitive
{
	glm::vec2 m_vStart;
	glm::vec2 m_vEnd;
	glm::vec4 m_vColour;

};

struct DebugQuad : public DebugPrimitive
{
	glm::vec2 m_vCorners[4];
	glm::vec4 m_vColour;
};


class Renderer
{
public:
	Renderer();
	~Renderer();

	bool	Initialise();
	void	Render();
	void	Shutdown();

	void	AddRenderable( Renderable* pRenderable );
	void	RemoveRenderable( Renderable* pRenderable );

	void	DrawDebugLine( const glm::vec2& vStart, const glm::vec2& vEnd, glm::vec4 vColour = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ) );
	void	DrawDebugQuad( glm::vec2* vCorners, glm::vec4 vColour = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ) );

	ShaderManager& GetShaderManager()
	{
		return m_ShaderManager;
	}

	GLFontManager& GetFontManager()
	{
		return m_FontManager;
	}

	static Renderer* GetInstance();

private:
	ShaderManager	m_ShaderManager;

	std::vector<Renderable*> m_Renderables;

	static Renderer*		m_pInstance;

	GLFontManager			m_FontManager;

	std::vector<DebugPrimitive*> m_DebugPrimitives;

	GLuint					m_debugVAO;
	
};

