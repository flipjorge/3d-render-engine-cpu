#ifndef TEXTURE
#define TEXTURE

#include <stdint.h>
#include "png/upng.h"

#define TEXTURE_WIDTH 64
#define TEXTURE_HEIGHT 64

// Represents a 2D texture coordinate (UV mapping).
// This struct is used to map a point on a 2D texture to a vertex on a 3D model.
// - 'u' is the horizontal coordinate (equivalent to X).
// - 'v' is the vertical coordinate (equivalent to Y).
// These values are typically in the range [0, 1]. During rasterization, they are
// interpolated across the surface of a triangle to determine which texel (texture pixel)
// to apply to each screen pixel.
typedef struct {
    float u;
    float v;
} texture_t;

uint32_t* convertARGBtoRGBATexture(const uint32_t* src, int numPixels);
void loadTextureFromPng(const char* filename, upng_t** png, uint32_t** texture);

extern const uint8_t sampleTexture[];

#endif