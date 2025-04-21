#ifndef TRIANGLE
#define TRIANGLE

#include "vector.h"

typedef struct {
    int a, b, c;
} face_t;

typedef struct {
    vector2_t points;
} triangle_t;

#endif