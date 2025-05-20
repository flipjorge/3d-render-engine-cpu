#include <stddef.h>
#include <math.h>
#include "matrix.h"
#include "stddef.h"

matrix4_t matrix4Identity()
{
    return (matrix4_t){
        .m = {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        }
    };
}

matrix4_t matrix4MultiplyMatrix(const matrix4_t* m1, const matrix4_t* m2)
{
    matrix4_t matrix = matrix4Identity();

    for (size_t row = 0; row < 4; row++)
    {
        for (size_t column = 0; column < 4; column++)
        {
            float value = m1->m[row][0] * m2->m[0][column];
            value += m1->m[row][1] * m2->m[1][column];
            value += m1->m[row][2] * m2->m[2][column];
            value += m1->m[row][3] * m2->m[3][column];

            matrix.m[row][column] = value;
        }
    }
    
    return matrix;
}

vector3_t matrix4MultiplyVector(const matrix4_t* matrix, const vector3_t* vector)
{
    vector3_t result = {0};

    result.x = matrix->m[0][0] * vector->x;
    result.x += matrix->m[0][1] * vector->y;
    result.x += matrix->m[0][2] * vector->z;
    result.x += matrix->m[0][3] * 1;

    result.y = matrix->m[1][0] * vector->x;
    result.y += matrix->m[1][1] * vector->y;
    result.y += matrix->m[1][2] * vector->z;
    result.y += matrix->m[1][3] * 1;

    result.z = matrix->m[2][0] * vector->x;
    result.z += matrix->m[2][1] * vector->y;
    result.z += matrix->m[2][2] * vector->z;
    result.z += matrix->m[2][3] * 1;
    
    return result;
}

matrix4_t matrix4Scale(const vector3_t* scale)
{
    matrix4_t matrix = matrix4Identity();

    matrix.m[0][0] = scale->x;
    matrix.m[1][1] = scale->y;
    matrix.m[2][2] = scale->z;
    
    return matrix;
}

matrix4_t matrix4Rotation(const vector3_t* rotation)
{
    matrix4_t matrix = matrix4Identity();

    float cx = cosf(rotation->x);
    float sx = sinf(rotation->x);
    float cy = cosf(rotation->y);
    float sy = sinf(rotation->y);
    float cz = cosf(rotation->z);
    float sz = sinf(rotation->z);

    // Rotation around X axis
    matrix4_t rx = matrix4Identity();
    rx.m[1][1] = cx;
    rx.m[1][2] = -sx;
    rx.m[2][1] = sx;
    rx.m[2][2] = cx;

    // Rotation around Y axis
    matrix4_t ry = matrix4Identity();
    ry.m[0][0] = cy;
    ry.m[0][2] = sy;
    ry.m[2][0] = -sy;
    ry.m[2][2] = cy;

    // Rotation around Z axis
    matrix4_t rz = matrix4Identity();
    rz.m[0][0] = cz;
    rz.m[0][1] = -sz;
    rz.m[1][0] = sz;
    rz.m[1][1] = cz;

    // Combine rotations: R = Rz * Ry * Rx
    matrix = matrix4MultiplyMatrix(&rz, &ry);
    matrix = matrix4MultiplyMatrix(&matrix, &rx);
    
    return matrix;
}

matrix4_t matrix4Translation(const vector3_t* translation)
{
    matrix4_t matrix = matrix4Identity();

    matrix.m[0][3] = translation->x;
    matrix.m[1][3] = translation->y;
    matrix.m[2][3] = translation->z;

    return matrix;
}

matrix4_t matrix4TRS(const matrix4_t* scale, const matrix4_t* rotation, const matrix4_t* translation)
{
    matrix4_t matrix = matrix4MultiplyMatrix(rotation, scale);
    matrix = matrix4MultiplyMatrix(translation, &matrix);
    return matrix;
}