#include "projection.h"

// Performs an orthographic projection of a 3D point onto a 2D plane.
// This is the simplest form of projection, where the z-coordinate is simply discarded.
// projected_point.x = vertex.x
// projected_point.y = vertex.y
vector2_t projectOrtographic(vector3_t vertex) {
    vector2_t projectedPoint = {
        vertex.x,
        vertex.y
    };
    
    return projectedPoint;
}

// Performs a perspective projection of a 3D point onto a 2D plane.
// This simulates how objects appear smaller as they get farther away.
// The 'fov' (field of view) factor affects the projection scale. A larger fov
// acts like a zoom-in. The division by vertex.z is what creates the perspective effect.
// projected_point.x = (vertex.x * fov) / vertex.z
// projected_point.y = (vertex.y * fov) / vertex.z
vector2_t projectPerspective(float fov, vector3_t vertex) {
    vector2_t projectedPoint = {
        vertex.x * fov / vertex.z,
        vertex.y * fov / vertex.z
    };
    
    return projectedPoint;
}