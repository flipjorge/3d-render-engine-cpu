#ifndef LIGHT
#define LIGHT

#include <SDL2/SDL.h>
#include "vector.h"

typedef struct
{
    vector3_t direction;
} light_t;

float lightIntensityFactor(const vector3_t lightDirection, const vector3_t vertices[3]);
uint32_t lightApplyIntensity(uint32_t color, float factor);

#endif