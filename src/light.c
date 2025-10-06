#include "light.h"

// Calculates the intensity of light on a triangle face using the Lambertian reflectance model.
// This function is a core part of the shading stage in the rendering pipeline. It determines
// how bright a face should be based on its angle relative to the light source.
//
// The calculation involves:
// 1. Finding the face normal vector by taking the cross product of two edges of the triangle (ab and ac).
// 2. Calculating the dot product of the normalized face normal and the light's direction vector.
//    The result is the cosine of the angle between them.
// 3. A dot product of 1 means the face is directly facing the light (max intensity), while a value
//    of 0 means it's perpendicular, and negative values mean it's facing away.
// The light direction is negated to point towards the surface, as per the standard lighting model.
float lightIntensityFactor(const vector3_t lightDirection, const vector3_t vertices[3])
{
    vector3_t ab = vector3Sub(vertices[1], vertices[0]);
    vector3_t ac = vector3Sub(vertices[2], vertices[0]);
    vector3_t normal = vector3CrossProduct(ab, ac);
    normal = vector3Normalized(normal);
    float dot = -vector3DotProduct(normal, lightDirection);

    return dot;
}

// Applies a calculated light intensity factor to a given color.
// This function is used in the shading stage to modulate the base color of a triangle
// with the computed light intensity, effectively darkening faces that are not directly
// facing the light source.
//
// The process is:
// 1. The intensity factor is clamped between 0 (no light) and 1 (full light).
// 2. The Red, Green, and Blue components of the input color are extracted.
// 3. Each component is multiplied by the intensity factor.
// 4. The modified RGB components are recombined with the original Alpha component to produce the final shaded color.
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