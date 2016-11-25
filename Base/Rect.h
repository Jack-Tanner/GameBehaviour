#pragma once
#include <glm/glm.hpp>

struct Rect
{
	glm::vec3	m_Position;
	glm::vec3	m_Size;
	
	Rect() : Rect(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f))
	{
	}

	Rect(glm::vec3 vPosition, glm::vec3 vSize)
	{
		m_Position = vPosition;
		m_Size = vSize * 0.5f;
	}
};
