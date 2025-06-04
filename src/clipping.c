#include <math.h>
#include "clipping.h"
#include "triangle.h"

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

polygon_t createPolygonFromTriangle(vector3_t v0, vector3_t v1, vector3_t v2)
{
    polygon_t polygon = {
        .vertices = { v0, v1, v2 },
        .numVertices = 3
    };
    return polygon;
}

void clipPolygonAgainstPlane(polygon_t* polygon, const plane_t* plane)
{
    vector3_t insideVertices[MAX_NUM_POLY_VERTICES];
    int numInsideVertices = 0;

    vector3_t* currentVertex = &polygon->vertices[0];
    vector3_t* previousVertex = &polygon->vertices[polygon->numVertices - 1];

    float currentDot = 0;
    float previousDot = vector3DotProduct(vector3Sub(*previousVertex, plane->point), plane->normal);

    while (currentVertex != &polygon->vertices[polygon->numVertices]) {
        currentDot = vector3DotProduct(vector3Sub(*currentVertex, plane->point), plane->normal);

        // If we changed from inside to outside or from outside to inside
        if (currentDot * previousDot < 0) {
            // Find the interpolation factor t
            float t = previousDot / (previousDot - currentDot);
            
            // Calculate the intersection point I = Q1 + t(Q2-Q1)
            vector3_t intersectionPoint = vector3Clone(*currentVertex);
            intersectionPoint = vector3Sub(intersectionPoint, *previousVertex);
            intersectionPoint = vector3Multiple(intersectionPoint, (vector3_t){ t, t, t });
            intersectionPoint = vector3Sum(intersectionPoint, *previousVertex); // I = Qp + t(Qc-Qp)

            insideVertices[numInsideVertices] = vector3Clone(intersectionPoint);
            numInsideVertices++;
        }

        // Current vertex is inside the plane
        if (currentDot > 0) {
            insideVertices[numInsideVertices] = vector3Clone(*currentVertex);
            numInsideVertices++;
        }

        previousDot = currentDot;
        previousVertex = currentVertex;
        currentVertex++;
    }
    
    for (int i = 0; i < numInsideVertices; i++) {
        polygon->vertices[i] = vector3Clone(insideVertices[i]);
    }

    polygon->numVertices = numInsideVertices;
}

void clipPolygon(polygon_t* polygon, const plane_t* frustumPlanes)
{
    clipPolygonAgainstPlane(polygon, &frustumPlanes[LEFT_FRUSTUM_PLANE]);
    clipPolygonAgainstPlane(polygon, &frustumPlanes[RIGHT_FRUSTUM_PLANE]);
    clipPolygonAgainstPlane(polygon, &frustumPlanes[TOP_FRUSTUM_PLANE]);
    clipPolygonAgainstPlane(polygon, &frustumPlanes[BOTTOM_FRUSTUM_PLANE]);
    clipPolygonAgainstPlane(polygon, &frustumPlanes[NEAR_FRUSTUM_PLANE]);
    clipPolygonAgainstPlane(polygon, &frustumPlanes[FAR_FRUSTUM_PLANE]);
}

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
    }

    *numberTriangles = polygon->numVertices - 2;
}