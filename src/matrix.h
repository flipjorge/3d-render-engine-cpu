#ifndef MATRIX
#define MATRIX


#include "vector.h"

typedef struct
{
    float m[4][4];
} matrix4_t;

matrix4_t matrix4Identity();
matrix4_t matrix4MakeScale(const vector3_t* scale);
matrix4_t matrix4MakeTranslation(const vector3_t* translation);
matrix4_t matrix4MakeRotationX(float angle);
matrix4_t matrix4MakeRotationY(float angle);
matrix4_t matrix4MakeRotationZ(float angle);
matrix4_t matrix4MakeRotation(const vector3_t* rotation);
matrix4_t matrix4MakePerspective(float fov, float aspect, float near, float far);
vector4_t matrix4MultiplyVector4(const matrix4_t* matrix, const vector4_t* vector);
matrix4_t matrix4MultiplyMatrix4(const matrix4_t* matrix1, const matrix4_t* matrix2);
vector4_t matrix4MultiplyVector4Project(const matrix4_t* projection, const vector4_t* vector);
matrix4_t matrix4TRS(const matrix4_t* scale, const matrix4_t* rotation, const matrix4_t* translation);
matrix4_t matrix4LookAt(const vector3_t* eye, const vector3_t* target, const vector3_t* up);

#endif