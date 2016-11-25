#pragma once

#include <glm/glm.hpp>
#include "RigidBody.h"

struct Manifold
{
	RigidBody* pA;
	RigidBody* pB;
	glm::vec2 vNormal;
	float m_fPenetration;
};