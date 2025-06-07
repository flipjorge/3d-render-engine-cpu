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

typedef struct {
    vector3_t point;
    vector3_t normal;
} plane_t;

typedef struct {
    vector3_t vertices[MAX_NUM_POLY_VERTICES];
    texture_t uvCoords[MAX_NUM_POLY_VERTICES];
    int numVertices;
} polygon_t;

void initFrustumPlane(plane_t* frustumPlanes, float fovX, float fovY, float zNear, float zFar);
polygon_t createPolygonFromTriangle(vector3_t v0, vector3_t v1, vector3_t v2, texture_t uv0, texture_t uv1, texture_t uv2);
void clipPolygon(polygon_t* polygon, const plane_t* frustumPlanes);
void trianglesFromPolygon(const polygon_t* polygon, triangle_t* triangles, int* numberTriangles);

#endif