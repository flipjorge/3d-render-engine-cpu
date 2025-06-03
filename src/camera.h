#ifndef CAMERA
#define CAMERA

#include "vector.h"

typedef struct {
    vector3_t position;
    vector3_t direction;
} camera_t;

#endif