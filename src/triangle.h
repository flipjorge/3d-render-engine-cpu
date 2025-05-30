#ifndef TRIANGLE
#define TRIANGLE

#include <SDL2/SDL.h>
#include "vector.h"
#include "texture.h"

typedef struct {
    int a, b, c;
    texture_t aUV, bUV, cUV;
} face_t;

typedef struct {
    vector4_t points[3];
    texture_t textureCoordinates[3];
    float depth;
    uint32_t color;
} triangle_t;

#endif