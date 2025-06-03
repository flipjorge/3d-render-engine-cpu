#ifndef CLIPPING
#define CLIPPING

#include "vector.h"

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

void initFrustumPlane(plane_t* frustumPlanes, float fov, float zNear, float zFar);

#endif