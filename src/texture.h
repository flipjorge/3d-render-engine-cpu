#ifndef TEXTURE
#define TEXTURE

#include <stdint.h>

typedef struct {
    float u;
    float v;
} texture_t;

extern const uint8_t sampleTexture[];

#endif