#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Renderer;

class Renderable
{
public:
	Renderable();
	virtual ~Renderable();

	virtual void			Render() = 0;
	virtual void			Destroy();

	void	SetWorldMatrix( const glm::mat4& matWorld )
	{
		m_WorldMatrix = matWorld;
	}

	const glm::mat4& GetWorldMatrix() const
	{
		return m_WorldMatrix;
	}

	bool	IsInitialised() const
	{
		return m_bInitialised;
	}

protected:
	bool	m_bInitialised;
	Renderer* m_pRenderer;

	glm::mat4	m_WorldMatrix;
};

