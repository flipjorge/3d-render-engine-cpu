#include <stdbool.h>
#include "face.h"

// Determines if a triangle face is visible from the camera's perspective.
// This is used for back-face culling, an optimization step in the rendering pipeline
// that discards triangles facing away from the camera, saving rendering time.
//
// The logic is based on the dot product between the face's normal vector and a
// vector pointing from the face to the camera.
// 1. Two vectors on the triangle's plane are calculated (ab, ac).
// 2. The face normal is computed using the cross product of these vectors.
// 3. A vector from the triangle to the camera is calculated.
// 4. The dot product of the normal and the camera vector is computed.
//    - If the dot product is positive, the angle between the vectors is less than 90 degrees,
//      meaning the face is pointing towards the camera and is visible.
//    - If the dot product is negative or zero, the face is pointing away from or is parallel
//      to the camera's line of sight and should be culled.
bool isFaceFacingCamera(const vector3_t cameraPosition, const vector3_t vertices[3])
{
    vector3_t ab = vector3Sub(vertices[1], vertices[0]);
    vector3_t ac = vector3Sub(vertices[2], vertices[0]);
    vector3_t normal = vector3CrossProduct(ab, ac);
    normal = vector3Normalized(normal);
    vector3_t cameraVector = vector3Sub(cameraPosition, vertices[0]);
    float dot = vector3DotProduct(normal, cameraVector);

    return dot > 0;
}