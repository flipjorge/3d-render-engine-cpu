#ifndef TRIANGLE
#define TRIANGLE

#include <SDL2/SDL.h>
#include "vector.h"
#include "texture.h"

// Represents a single triangular face of a mesh, as defined in an .obj file.
// It connects vertices from the mesh's vertex list to form a surface and
// associates texture coordinates with each vertex.
typedef struct {
    // Indices into the mesh's vertex array, defining the three vertices of the triangle.
    int a, b, c;
    // UV texture coordinates for each vertex of the face (a, b, and c).
    texture_t aUV, bUV, cUV;
} face_t;

// Represents a triangle ready for the rasterization stage of the rendering pipeline.
// This struct holds all the necessary data for a single triangle after it has been
// transformed, clipped, and projected into screen space.
typedef struct {
    // The three vertices of the triangle in homogeneous screen-space coordinates.
    // The x and y components are pixel coordinates, z is for the depth buffer,
    // and w is used for perspective-correct interpolation of attributes.
    vector4_t points[3];
    // The UV texture coordinates corresponding to each of the three vertices.
    // These are interpolated across the triangle's surface to map the texture.
    texture_t textureCoordinates[3];
    // The color of the triangle, typically calculated from lighting computations.
    // This color is used for flat shading or can be modulated with a texture.
    uint32_t color;
} triangle_t;

#endif