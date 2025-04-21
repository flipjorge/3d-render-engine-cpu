#include "projection.h"

vector2_t projectOrtographic(vector3_t vertex) {
    vector2_t projectedPoint = {
        vertex.x,
        vertex.y
    };
    
    return projectedPoint;
}