#ifndef PROJECTION
#define PROJECTION

#include "vector.h"

vector2_t projectOrtographic(vector3_t vertex);
vector2_t projectPerspective(float fov, vector3_t vertex);

#endif