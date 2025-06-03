#include <math.h>
#include "clipping.h"

void initFrustumPlane(plane_t* frustumPlanes, float fov, float zNear, float zFar)
{
	float cos_half_fov = cos(fov / 2);
	float sin_half_fov = sin(fov / 2);

	frustumPlanes[LEFT_FRUSTUM_PLANE].point = (vector3_t){ 0, 0, 0 };
	frustumPlanes[LEFT_FRUSTUM_PLANE].normal.x = cos_half_fov;
	frustumPlanes[LEFT_FRUSTUM_PLANE].normal.y = 0;
	frustumPlanes[LEFT_FRUSTUM_PLANE].normal.z = sin_half_fov;

	frustumPlanes[RIGHT_FRUSTUM_PLANE].point = (vector3_t){ 0, 0, 0 };
	frustumPlanes[RIGHT_FRUSTUM_PLANE].normal.x = -cos_half_fov;
	frustumPlanes[RIGHT_FRUSTUM_PLANE].normal.y = 0;
	frustumPlanes[RIGHT_FRUSTUM_PLANE].normal.z = sin_half_fov;

	frustumPlanes[TOP_FRUSTUM_PLANE].point = (vector3_t){ 0, 0, 0 };
	frustumPlanes[TOP_FRUSTUM_PLANE].normal.x = 0;
	frustumPlanes[TOP_FRUSTUM_PLANE].normal.y = -cos_half_fov;
	frustumPlanes[TOP_FRUSTUM_PLANE].normal.z = sin_half_fov;

	frustumPlanes[BOTTOM_FRUSTUM_PLANE].point = (vector3_t){ 0, 0, 0 };
	frustumPlanes[BOTTOM_FRUSTUM_PLANE].normal.x = 0;
	frustumPlanes[BOTTOM_FRUSTUM_PLANE].normal.y = cos_half_fov;
	frustumPlanes[BOTTOM_FRUSTUM_PLANE].normal.z = sin_half_fov;

	frustumPlanes[NEAR_FRUSTUM_PLANE].point = (vector3_t){ 0, 0, zNear };
	frustumPlanes[NEAR_FRUSTUM_PLANE].normal.x = 0;
	frustumPlanes[NEAR_FRUSTUM_PLANE].normal.y = 0;
	frustumPlanes[NEAR_FRUSTUM_PLANE].normal.z = 1;

	frustumPlanes[FAR_FRUSTUM_PLANE].point = (vector3_t){ 0, 0, zFar };
	frustumPlanes[FAR_FRUSTUM_PLANE].normal.x = 0;
	frustumPlanes[FAR_FRUSTUM_PLANE].normal.y = 0;
	frustumPlanes[FAR_FRUSTUM_PLANE].normal.z = -1;
}
