#include <math.h>
#include "clipping.h"
#include "triangle.h"

// Initializes the six planes of the view frustum in camera space.
// This is part of the setup for the clipping stage. The frustum is a pyramid-like shape
// that defines the visible volume of the scene.
//
// The math involves using the horizontal (fovX) and vertical (fovY) fields of view to
// calculate the normal vectors for the left, right, top, and bottom planes. These normals
// are calculated using sine and cosine of half the FOV angles to define planes that pass
// through the origin (the camera's position) and are angled correctly. The near and far
// planes are simpler, being parallel to the XY plane at zNear and zFar respectively.
// All normals point *inside* the frustum volume.
void initFrustumPlane(plane_t* frustumPlanes, float fovX, float fovY, float zNear, float zFar)
{
    float cosHalfFovX = cos(fovX / 2);
    float sinHalfFovX = sin(fovX / 2);

    float cosHalfFovY = cos(fovY / 2);
    float sinHalfFovY = sin(fovY / 2);

    frustumPlanes[LEFT_FRUSTUM_PLANE].point = (vector3_t){ 0, 0, 0 };
    frustumPlanes[LEFT_FRUSTUM_PLANE].normal.x = cosHalfFovX;
    frustumPlanes[LEFT_FRUSTUM_PLANE].normal.y = 0;
    frustumPlanes[LEFT_FRUSTUM_PLANE].normal.z = sinHalfFovX;

    frustumPlanes[RIGHT_FRUSTUM_PLANE].point = (vector3_t){ 0, 0, 0 };
    frustumPlanes[RIGHT_FRUSTUM_PLANE].normal.x = -cosHalfFovX;
    frustumPlanes[RIGHT_FRUSTUM_PLANE].normal.y = 0;
    frustumPlanes[RIGHT_FRUSTUM_PLANE].normal.z = sinHalfFovX;

    frustumPlanes[TOP_FRUSTUM_PLANE].point = (vector3_t){ 0, 0, 0 };
    frustumPlanes[TOP_FRUSTUM_PLANE].normal.x = 0;
    frustumPlanes[TOP_FRUSTUM_PLANE].normal.y = -cosHalfFovY;
    frustumPlanes[TOP_FRUSTUM_PLANE].normal.z = sinHalfFovY;

    frustumPlanes[BOTTOM_FRUSTUM_PLANE].point = (vector3_t){ 0, 0, 0 };
    frustumPlanes[BOTTOM_FRUSTUM_PLANE].normal.x = 0;
    frustumPlanes[BOTTOM_FRUSTUM_PLANE].normal.y = cosHalfFovY;
    frustumPlanes[BOTTOM_FRUSTUM_PLANE].normal.z = sinHalfFovY;

    frustumPlanes[NEAR_FRUSTUM_PLANE].point = (vector3_t){ 0, 0, zNear };
    frustumPlanes[NEAR_FRUSTUM_PLANE].normal.x = 0;
    frustumPlanes[NEAR_FRUSTUM_PLANE].normal.y = 0;
    frustumPlanes[NEAR_FRUSTUM_PLANE].normal.z = 1;

    frustumPlanes[FAR_FRUSTUM_PLANE].point = (vector3_t){ 0, 0, zFar };
    frustumPlanes[FAR_FRUSTUM_PLANE].normal.x = 0;
    frustumPlanes[FAR_FRUSTUM_PLANE].normal.y = 0;
    frustumPlanes[FAR_FRUSTUM_PLANE].normal.z = -1;
}

// Converts a triangle into a polygon structure.
// This is the first step in the clipping pipeline for a given triangle. The polygon
// structure is more flexible than a triangle, as the number of vertices can change
// during the clipping process.
polygon_t createPolygonFromTriangle(
    vector3_t v0, vector3_t v1, vector3_t v2,
    texture_t uv0, texture_t uv1, texture_t uv2)
{
    polygon_t polygon = {
        .vertices = { v0, v1, v2 },
        .uvCoords = { uv0, uv1, uv2 },
        .numVertices = 3
    };
    return polygon;
}

// Linearly interpolates between two float values.
// This is a helper function used during clipping to find the exact intersection point
// of a polygon edge with a frustum plane. It's used for both vertex coordinates and
// texture coordinates.
// Math: result = a + t * (b - a)
float floatLerp(float a, float b, float t) {
    return a + t * (b - a);
}

// Clips a polygon against a single plane using the Sutherland-Hodgman algorithm.
// This function is the core of the clipping process. It iterates through the polygon's
// edges and outputs a new set of vertices that are all on the "inside" of the plane.
//
// For each edge (from a previous to a current vertex):
// 1. It determines if the vertices are inside or outside the plane by checking the sign
//    of the dot product between the vector from the plane's point to the vertex and the
//    plane's normal. A positive dot product means the vertex is inside.
// 2. If an edge crosses the plane boundary, it calculates the intersection point. The
//    interpolation factor 't' is found using the dot products: t = prev_dot / (prev_dot - current_dot).
// 3. This 't' value is used to linearly interpolate the vertex position and UV coordinates
//    to find the new vertex at the intersection.
// 4. A new list of "inside" vertices is generated, forming the clipped polygon.
void clipPolygonAgainstPlane(polygon_t* polygon, const plane_t* plane)
{
    vector3_t insideVertices[MAX_NUM_POLY_VERTICES];
    texture_t insideUVs[MAX_NUM_POLY_VERTICES];
    int numInsideVertices = 0;

    vector3_t* currentVertex = &polygon->vertices[0];
    vector3_t* previousVertex = &polygon->vertices[polygon->numVertices - 1];

    texture_t* currentUV = &polygon->uvCoords[0];
    texture_t* previousUV = &polygon->uvCoords[polygon->numVertices - 1];

    float currentDot = 0;
    float previousDot = vector3DotProduct(vector3Sub(*previousVertex, plane->point), plane->normal);

    while (currentVertex != &polygon->vertices[polygon->numVertices]) {
        currentDot = vector3DotProduct(vector3Sub(*currentVertex, plane->point), plane->normal);

        // If we changed from inside to outside or from outside to inside
        if (currentDot * previousDot < 0) {
            // Find the interpolation factor t
            float t = previousDot / (previousDot - currentDot);

            vector3_t intersectionPoint = (vector3_t){
                .x = floatLerp(previousVertex->x, currentVertex->x, t),
                .y = floatLerp(previousVertex->y, currentVertex->y, t),
                .z = floatLerp(previousVertex->z, currentVertex->z, t)
            };

            texture_t intersectionUV = (texture_t){
                .u = floatLerp(previousUV->u, currentUV->u, t),
                .v = floatLerp(previousUV->v, currentUV->v, t)
            };

            insideVertices[numInsideVertices] = vector3Clone(intersectionPoint);
            insideUVs[numInsideVertices] = intersectionUV;
            numInsideVertices++;
        }

        // Current vertex is inside the plane
        if (currentDot > 0) {
            insideVertices[numInsideVertices] = vector3Clone(*currentVertex);
            insideUVs[numInsideVertices] = (texture_t){ .u = currentUV->u, .v = currentUV->v };
            numInsideVertices++;
        }

        previousDot = currentDot;
        previousVertex = currentVertex;
        currentVertex++;

        previousUV = currentUV;
        currentUV++;
    }
    
    for (int i = 0; i < numInsideVertices; i++) {
        polygon->vertices[i] = vector3Clone(insideVertices[i]);
        polygon->uvCoords[i] = insideUVs[i];
    }

    polygon->numVertices = numInsideVertices;
}

// Clips a polygon against all six planes of the view frustum.
// This function orchestrates the clipping process by calling `clipPolygonAgainstPlane`
// sequentially for each of the six frustum planes. The output polygon from one clipping
// stage becomes the input for the next. If the polygon is ever reduced to fewer than
// three vertices, it is effectively discarded.
void clipPolygon(polygon_t* polygon, const plane_t* frustumPlanes)
{
    clipPolygonAgainstPlane(polygon, &frustumPlanes[LEFT_FRUSTUM_PLANE]);
    clipPolygonAgainstPlane(polygon, &frustumPlanes[RIGHT_FRUSTUM_PLANE]);
    clipPolygonAgainstPlane(polygon, &frustumPlanes[TOP_FRUSTUM_PLANE]);
    clipPolygonAgainstPlane(polygon, &frustumPlanes[BOTTOM_FRUSTUM_PLANE]);
    clipPolygonAgainstPlane(polygon, &frustumPlanes[NEAR_FRUSTUM_PLANE]);
    clipPolygonAgainstPlane(polygon, &frustumPlanes[FAR_FRUSTUM_PLANE]);
}

// Converts a final (clipped) polygon back into one or more triangles.
// After a polygon has been clipped against all frustum planes, it needs to be
// converted back into triangles to be rendered. This is done using a simple
// "triangle fan" method, where the first vertex of the polygon is used as a common
// vertex for all the new triangles.
//
// For a polygon with N vertices, it creates (N - 2) triangles.
// Triangle 1: (v0, v1, v2)
// Triangle 2: (v0, v2, v3)
// ... and so on.
void trianglesFromPolygon(const polygon_t* polygon, triangle_t* triangles, int* numberTriangles)
{
    for (int i = 0; i < polygon->numVertices - 2; i++)
    {
        int index0 = 0;
        int index1 = i + 1;
        int index2 = i + 2;

        triangles[i].points[0] = vector3to4(polygon->vertices[index0]);
        triangles[i].points[1] = vector3to4(polygon->vertices[index1]);
        triangles[i].points[2] = vector3to4(polygon->vertices[index2]);

        triangles[i].textureCoordinates[0] = polygon->uvCoords[index0];
        triangles[i].textureCoordinates[1] = polygon->uvCoords[index1];
        triangles[i].textureCoordinates[2] = polygon->uvCoords[index2];
    }

    *numberTriangles = polygon->numVertices - 2;
}