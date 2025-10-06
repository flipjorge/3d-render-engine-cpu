#ifndef CLIPPING
#define CLIPPING

#include "vector.h"
#include "triangle.h"
#include "triangle.h"

#define FRUSTUM_NUM_PLANES 6
#define MAX_NUM_POLY_VERTICES 10
#define MAX_NUM_POLY_TRIANGLES 10

enum {
    LEFT_FRUSTUM_PLANE,
    RIGHT_FRUSTUM_PLANE,
    TOP_FRUSTUM_PLANE,
    BOTTOM_FRUSTUM_PLANE,
    NEAR_FRUSTUM_PLANE,
    FAR_FRUSTUM_PLANE
};

// Represents a plane in 3D space, defined by a point on the plane and a normal vector.
// Used to define the six planes of the view frustum for clipping. The normal vector
// is expected to point "inward", into the visible volume of the frustum.
typedef struct {
    vector3_t point;
    vector3_t normal;
} plane_t;

// Represents a polygon, used as an intermediate format during clipping.
// A triangle is first converted to a polygon. As it's clipped against the frustum
// planes, it can gain or lose vertices. After clipping is complete, the resulting
// polygon is triangulated back into one or more triangles.
typedef struct {
    vector3_t vertices[MAX_NUM_POLY_VERTICES];
    texture_t uvCoords[MAX_NUM_POLY_VERTICES];
    int numVertices;
} polygon_t;

// Initializes the six planes of the view frustum in camera space.
void initFrustumPlane(plane_t* frustumPlanes, float fovX, float fovY, float zNear, float zFar);
// Converts a triangle into a polygon to begin the clipping process.
polygon_t createPolygonFromTriangle(vector3_t v0, vector3_t v1, vector3_t v2, texture_t uv0, texture_t uv1, texture_t uv2);
// Clips a polygon against the six planes of the view frustum using the Sutherland-Hodgman algorithm.
void clipPolygon(polygon_t* polygon, const plane_t* frustumPlanes);
// Converts a clipped polygon back into one or more triangles that can be rendered.
void trianglesFromPolygon(const polygon_t* polygon, triangle_t* triangles, int* numberTriangles);

#endif