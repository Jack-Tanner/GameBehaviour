#pragma once

#include "BaseShape.h"
#include "Manifold.h"

typedef bool( *EvaluateFunction )( BaseShape*, BaseShape*, Manifold& vOutManifold );

bool AABBvsAABB( BaseShape* A, BaseShape* B, Manifold& vOutManifold );
bool OOBBvsOOBB( BaseShape* A, BaseShape* B, Manifold& vOutManifold );