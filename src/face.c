#include <stdbool.h>
#include "face.h"

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