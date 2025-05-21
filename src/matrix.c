#include <stddef.h>
#include <math.h>
#include "matrix.h"

matrix4_t matrix4Identity()
{
    return (matrix4_t){{
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    }};
}

matrix4_t matrix4MakeScale(const vector3_t* scale)
{
    matrix4_t matrix = matrix4Identity();

    matrix.m[0][0] = scale->x;
    matrix.m[1][1] = scale->y;
    matrix.m[2][2] = scale->z;
    
    return matrix;
}

matrix4_t matrix4MakeTranslation(const vector3_t* translation)
{
    matrix4_t matrix = matrix4Identity();

    matrix.m[0][3] = translation->x;
    matrix.m[1][3] = translation->y;
    matrix.m[2][3] = translation->z;

    return matrix;
}

matrix4_t matrix4MakeRotationX(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    
    matrix4_t m = matrix4Identity();
    m.m[1][1] = c;
    m.m[1][2] = -s;
    m.m[2][1] = s;
    m.m[2][2] = c;
    return m;
}

matrix4_t matrix4MakeRotationY(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    
    matrix4_t m = matrix4Identity();
    m.m[0][0] = c;
    m.m[0][2] = s;
    m.m[2][0] = -s;
    m.m[2][2] = c;
    return m;
}

matrix4_t matrix4MakeRotationZ(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    
    matrix4_t m = matrix4Identity();
    m.m[0][0] = c;
    m.m[0][1] = -s;
    m.m[1][0] = s;
    m.m[1][1] = c;
    return m;
}

matrix4_t matrix4MakeRotation(const vector3_t* rotation)
{
    matrix4_t matrix = matrix4Identity();

    float cx = cosf(rotation->x);
    float sx = sinf(rotation->x);
    float cy = cosf(rotation->y);
    float sy = sinf(rotation->y);
    float cz = cosf(rotation->z);
    float sz = sinf(rotation->z);

    matrix4_t rx = matrix4Identity();
    rx.m[1][1] = cx;
    rx.m[1][2] = -sx;
    rx.m[2][1] = sx;
    rx.m[2][2] = cx;

    matrix4_t ry = matrix4Identity();
    ry.m[0][0] = cy;
    ry.m[0][2] = sy;
    ry.m[2][0] = -sy;
    ry.m[2][2] = cy;

    matrix4_t rz = matrix4Identity();
    rz.m[0][0] = cz;
    rz.m[0][1] = -sz;
    rz.m[1][0] = sz;
    rz.m[1][1] = cz;

    // Combine rotations: R = Rz * Ry * Rx
    matrix = matrix4MultiplyMatrix4(&rz, &ry);
    matrix = matrix4MultiplyMatrix4(&matrix, &rx);
    
    return matrix;
}

matrix4_t matrix4MakePerspective(float fov, float aspect, float near, float far)
{
    matrix4_t matrix = {{{ 0 }}};
    matrix.m[0][0] = aspect * (1 / tan(fov / 2));
    matrix.m[1][1] = 1 / tan(fov / 2);
    matrix.m[2][2] = far /  (far - near);
    matrix.m[2][3] = (-far * near) / (far - near);
    matrix.m[3][2] = 1.0;

    return matrix;
}

vector4_t matrix4MultiplyVector4(const matrix4_t* matrix, const vector4_t* vector)
{
    vector4_t result = {0};

    result.x = matrix->m[0][0] * vector->x;
    result.x += matrix->m[0][1] * vector->y;
    result.x += matrix->m[0][2] * vector->z;
    result.x += matrix->m[0][3] * vector->w;

    result.y = matrix->m[1][0] * vector->x;
    result.y += matrix->m[1][1] * vector->y;
    result.y += matrix->m[1][2] * vector->z;
    result.y += matrix->m[1][3] * vector->w;

    result.z = matrix->m[2][0] * vector->x;
    result.z += matrix->m[2][1] * vector->y;
    result.z += matrix->m[2][2] * vector->z;
    result.z += matrix->m[2][3] * vector->w;

    result.w = matrix->m[3][0] * vector->x;
    result.w += matrix->m[3][1] * vector->y;
    result.w += matrix->m[3][2] * vector->z;
    result.w += matrix->m[3][3] * vector->w;
    
    return result;
}

matrix4_t matrix4MultiplyMatrix4(const matrix4_t* matrix1, const matrix4_t* matrix2)
{
    matrix4_t matrix = matrix4Identity();

    for (size_t row = 0; row < 4; row++)
    {
        for (size_t column = 0; column < 4; column++)
        {
            float value = matrix1->m[row][0] * matrix2->m[0][column];
            value += matrix1->m[row][1] * matrix2->m[1][column];
            value += matrix1->m[row][2] * matrix2->m[2][column];
            value += matrix1->m[row][3] * matrix2->m[3][column];

            matrix.m[row][column] = value;
        }
    }
    
    return matrix;
}

vector4_t matrix4MultiplyVector4Project(const matrix4_t* projection, const vector4_t* vector)
{
    vector4_t result = matrix4MultiplyVector4(projection, vector);

    if (result.w != 0.0f) {
        result.x /= result.w;
        result.y /= result.w;
        result.z /= result.w;
    }

    return result;
}

matrix4_t matrix4TRS(const matrix4_t* scale, const matrix4_t* rotation, const matrix4_t* translation)
{
    matrix4_t matrix = matrix4MultiplyMatrix4(rotation, scale);
    matrix = matrix4MultiplyMatrix4(translation, &matrix);
    return matrix;
}
