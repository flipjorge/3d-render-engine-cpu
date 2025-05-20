#ifndef MATRIX
#define MATRIX


#include "vector.h"

typedef struct
{
    float m[4][4];
} matrix4_t;

matrix4_t matrix4Identity();
matrix4_t matrix4MultiplyMatrix(const matrix4_t* m1, const matrix4_t* m2);
vector3_t matrix4MultiplyVector(const matrix4_t* matrix, const vector3_t* vector);
matrix4_t matrix4Scale(const vector3_t* scale);
matrix4_t matrix4Rotation(const vector3_t* rotation);
matrix4_t matrix4Translation(const vector3_t* translation);
matrix4_t matrix4TRS(const matrix4_t* scale, const matrix4_t* rotation, const matrix4_t* translation);

#endif