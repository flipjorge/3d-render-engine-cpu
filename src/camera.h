#ifndef CAMERA
#define CAMERA

#include "vector.h"

// Represents the camera in the 3D scene, defining the viewpoint for rendering.
// The view matrix, which transforms world coordinates into camera space, is derived
// from the camera's properties.
typedef struct {
    vector3_t position;
    vector3_t direction;
} camera_t;

#endif