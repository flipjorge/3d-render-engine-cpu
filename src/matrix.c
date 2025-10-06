#include <stddef.h>
#include <math.h>
#include <stdio.h>
#include "matrix.h"

// Creates a 4x4 identity matrix:
// |1 0 0 0|
// |0 1 0 0|
// |0 0 1 0|
// |0 0 0 1|
matrix4_t matrix4Identity()
{
    return (matrix4_t){{
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    }};
}

// Creates a scale matrix by placing scale factors on the diagonal:
// |sx  0  0  0|
// |0  sy  0  0|
// |0   0 sz  0|
// |0   0  0  1|
matrix4_t matrix4MakeScale(const vector3_t* scale)
{
    matrix4_t matrix = matrix4Identity();

    matrix.m[0][0] = scale->x;
    matrix.m[1][1] = scale->y;
    matrix.m[2][2] = scale->z;
    
    return matrix;
}

// Creates a translation matrix by placing translation values in the fourth column:
// |1  0  0  tx|
// |0  1  0  ty|
// |0  0  1  tz|
// |0  0  0   1|
matrix4_t matrix4MakeTranslation(const vector3_t* translation)
{
    matrix4_t matrix = matrix4Identity();

    matrix.m[0][3] = translation->x;
    matrix.m[1][3] = translation->y;
    matrix.m[2][3] = translation->z;

    return matrix;
}

// Creates a rotation matrix around X axis using trigonometry:
// |1   0    0   0|
// |0  cos -sin  0|
// |0  sin  cos  0|
// |0   0    0   1|
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

// Creates a rotation matrix around Y axis using trigonometry:
// | cos  0  sin  0|
// |  0   1   0   0|
// |-sin  0  cos  0|
// |  0   0   0   1|
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

// Creates a rotation matrix around Z axis using trigonometry:
// |cos -sin  0  0|
// |sin  cos  0  0|
// | 0    0   1  0|
// | 0    0   0  1|
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

// Creates a combined rotation matrix from Euler angles (XYZ order)
// Final matrix = Rz * Ry * Rx (multiplication order matters!)
// This creates a rotation matrix that applies X rotation first,
// then Y rotation, and finally Z rotation
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

// Creates a perspective projection matrix:
// [aspect/tan(fov/2)    0             0              0    ]
// [     0          1/tan(fov/2)       0              0    ]
// [     0               0        far/(far-near)  -far*near/(far-near)]
// [     0               0             1              0    ]
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

// Multiplies a matrix by a vector using dot product:
// For each row of the matrix, compute:
// result[i] = row[i].x * vector.x + row[i].y * vector.y + 
//             row[i].z * vector.z + row[i].w * vector.w
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

// Multiplies two matrices using row-column multiplication:
// For each element [i][j] in result matrix:
// result[i][j] = sum(matrix1[i][k] * matrix2[k][j]) for k = 0 to 3
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

// Performs perspective projection and divides by w component
// This creates normalized device coordinates (-1 to 1)
// The w-divide is what creates the perspective effect
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

// Combines scale, rotation, and translation matrices in TRS order
// Final matrix = Translation * Rotation * Scale
// This order ensures proper transformation hierarchy
matrix4_t matrix4TRS(const matrix4_t* scale, const matrix4_t* rotation, const matrix4_t* translation)
{
    matrix4_t matrix = matrix4MultiplyMatrix4(rotation, scale);
    matrix = matrix4MultiplyMatrix4(translation, &matrix);
    return matrix;
}

// Creates a view matrix for camera positioning:
// 1. Calculate forward (z) vector from target to eye
// 2. Calculate right (x) vector from up and forward
// 3. Calculate up (y) vector from forward and right
// 4. Create orthonormal basis matrix and include translation
// Final matrix transforms world space to view space
matrix4_t matrix4LookAt(const vector3_t* eye, const vector3_t* target, const vector3_t* up)
{
    vector3_t z = vector3Sub(*target, *eye);
    z = vector3Normalized(z);

    vector3_t x = vector3CrossProduct(*up, z);
    x = vector3Normalized(x);
    
    vector3_t y = vector3CrossProduct(z, x);
    
    matrix4_t viewMatrix = {{
        { x.x, x.y, x.z, -vector3DotProduct(x, *eye) },
        { y.x, y.y, y.z, -vector3DotProduct(y, *eye) },
        { z.x, z.y, z.z, -vector3DotProduct(z, *eye) },
        { 0, 0, 0, 1 }
    }};

    return viewMatrix;
}
