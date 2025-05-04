#include "projection.h"

vector2_t projectOrtographic(vector3_t vertex) {
    vector2_t projectedPoint = {
        vertex.x,
        vertex.y
    };
    
    return projectedPoint;
}

vector2_t projectPerspective(float fov, vector3_t vertex) {
    vector2_t projectedPoint = {
        vertex.x * fov / vertex.z,
        vertex.y * fov / vertex.z
    };
    
    return projectedPoint;
}