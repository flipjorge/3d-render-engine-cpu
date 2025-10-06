#ifndef LIGHT
#define LIGHT

#include <SDL2/SDL.h>
#include "vector.h"

// Represents a directional light source in the scene.
// A directional light is assumed to be infinitely far away, so all its rays are parallel.
// This is a simple and efficient way to simulate a distant light source like the sun.
typedef struct
{
    vector3_t direction;
} light_t;

float lightIntensityFactor(const vector3_t lightDirection, const vector3_t vertices[3]);
uint32_t lightApplyIntensity(uint32_t color, float factor);

#endif