#include "light.h"

float lightIntensityFactor(const vector3_t lightDirection, const vector3_t vertices[3])
{
    vector3_t ab = vector3Sub(vertices[1], vertices[0]);
    vector3_t ac = vector3Sub(vertices[2], vertices[0]);
    vector3_t normal = vector3CrossProduct(ab, ac);
    normal = vector3Normalized(normal);
    float dot = -vector3DotProduct(normal, lightDirection);

    return dot;
}

uint32_t lightApplyIntensity(uint32_t color, float factor)
{
    if (factor < 0) factor = 0;
    if (factor > 1) factor = 1;

    uint32_t r = (color & 0xFF000000) * factor;
    uint32_t g = (color & 0x00FF0000) * factor;
    uint32_t b = (color & 0x0000FF00) * factor;
    uint32_t a = (color & 0x000000FF);

    return (r & 0xFF000000) | (g & 0x00FF0000) | (b & 0x0000FF00) | a;
}