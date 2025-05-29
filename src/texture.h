#ifndef TEXTURE
#define TEXTURE

#include <stdint.h>

#define TEXTURE_WIDTH 64
#define TEXTURE_HEIGHT 64

typedef struct {
    float u;
    float v;
} texture_t;

uint32_t* convertARGBtoRGBATexture(const uint32_t* src, int numPixels);

extern const uint8_t sampleTexture[];

#endif