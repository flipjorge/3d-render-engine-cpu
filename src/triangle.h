#ifndef TRIANGLE
#define TRIANGLE

#include <SDL2/SDL.h>
#include "vector.h"

typedef struct {
    int a, b, c;
} face_t;

typedef struct {
    vector2_t points[3];
    float depth;
    uint32_t color;
} triangle_t;

#endif