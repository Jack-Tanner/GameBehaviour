#pragma once

#include <glm/glm.hpp>
struct Transform
{
	glm::vec2	m_Position;
	glm::vec2	m_Scale;
	float		m_Rotation;

	Transform()
	{
		m_Position = glm::vec2( 0.0f, 0.0f );
		m_Scale = glm::vec2( 1.0f, 1.0f );
		m_Rotation = 0.0f;
	}
};