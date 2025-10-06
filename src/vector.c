#include "vector.h"
#include <math.h>

// Performs vector addition: c = a + b
// (cx, cy) = (ax + bx, ay + by)
vector2_t vector2Sum(vector2_t a, vector2_t b)
{
    return (vector2_t){ a.x + b.x, a.y + b.y };
}

// Performs vector subtraction: c = a - b
// (cx, cy) = (ax - bx, ay - by)
vector2_t vector2Sub(vector2_t a, vector2_t b)
{
    return (vector2_t){ a.x - b.x, a.y - b.y };
}

// Performs component-wise multiplication (Hadamard product): c = a * b
// (cx, cy) = (ax * bx, ay * by)
vector2_t vector2Multiple(vector2_t a, vector2_t b)
{
    return (vector2_t){ a.x * b.x, a.y * b.y };
}

// Performs 3D vector addition: c = a + b
// (cx, cy, cz) = (ax + bx, ay + by, az + bz)
vector3_t vector3Sum(vector3_t a, vector3_t b)
{
    return (vector3_t){ a.x + b.x, a.y + b.y, a.z + b.z };
}

// Performs 3D vector subtraction: c = a - b
// (cx, cy, cz) = (ax - bx, ay - by, az - bz)
vector3_t vector3Sub(vector3_t a, vector3_t b)
{
    return (vector3_t){ a.x - b.x, a.y - b.y, a.z - b.z };
}

// Performs 3D component-wise multiplication (Hadamard product): c = a * b
// (cx, cy, cz) = (ax * bx, ay * by, az * bz)
vector3_t vector3Multiple(vector3_t a, vector3_t b)
{
    return (vector3_t){ a.x * b.x, a.y * b.y, a.z * b.z };
}

// Rotates a 3D vector around the X-axis by a given angle.
// This is a 2D rotation in the YZ-plane.
// x' = x
// y' = y*cos(angle) - z*sin(angle)
// z' = y*sin(angle) + z*cos(angle)
vector3_t vector3RotateX(vector3_t vector, float angle)
{
    vector3_t rotatedVector = {
        .x = vector.x,
        .y = vector.y * cos(angle) - vector.z * sin(angle),
        .z = vector.y * sin(angle) + vector.z * cos(angle)
    };

    return rotatedVector;
}

// Rotates a 3D vector around the Y-axis by a given angle.
// This is a 2D rotation in the XZ-plane.
// x' = x*cos(angle) - z*sin(angle)
// y' = y
// z' = x*sin(angle) + z*cos(angle)
vector3_t vector3RotateY(vector3_t vector, float angle)
{
    vector3_t rotatedVector = {
        .x = vector.x * cos(angle) - vector.z * sin(angle),
        .y = vector.y,
        .z = vector.x * sin(angle) + vector.z * cos(angle)
    };

    return rotatedVector;
}

// Rotates a 3D vector around the Z-axis by a given angle.
// This is a 2D rotation in the XY-plane.
// x' = x*cos(angle) - y*sin(angle)
// y' = x*sin(angle) + y*cos(angle)
// z' = z
vector3_t vector3RotateZ(vector3_t vector, float angle)
{
    vector3_t rotatedVector = {
        .x = vector.x * cos(angle) - vector.y * sin(angle),
        .y = vector.x * sin(angle) + vector.y * cos(angle),
        .z = vector.z
    };

    return rotatedVector;
}

// Calculates the magnitude (length) of a 2D vector using the Pythagorean theorem.
// ||v|| = sqrt(x^2 + y^2)
float vector2Magnitude(const vector2_t vector)
{
    return sqrt(vector.x * vector.x + vector.y * vector.y);
}

// Calculates the magnitude (length) of a 3D vector using the Pythagorean theorem in 3D.
// ||v|| = sqrt(x^2 + y^2 + z^2)
float vector3Magnitude(const vector3_t vector)
{
    return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

// Calculates the cross product of two 3D vectors, a x b.
// The resulting vector is perpendicular to both a and b.
// cx = ay*bz - az*by
// cy = az*bx - ax*bz
// cz = ax*by - ay*bx
vector3_t vector3CrossProduct(vector3_t a, vector3_t b)
{
    vector3_t crossProduct = {
        .x = a.y * b.z - a.z * b.y,
        .y = a.z * b.x - a.x * b.z,
        .z = a.x * b.y - a.y * b.x
    };

    return crossProduct;
}

// Calculates the dot product of two 3D vectors, a . b.
// The result is a scalar value.
// a . b = ax*bx + ay*by + az*bz
float vector3DotProduct(vector3_t a, vector3_t b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Normalizes a 2D vector, resulting in a unit vector (length of 1).
// v_normalized = v / ||v||
vector2_t vector2Normalized(vector2_t vector)
{
    float magnitude = vector2Magnitude(vector);
    if (magnitude == 0) return vector;

    return (vector2_t){ vector.x / magnitude, vector.y / magnitude };
}

// Normalizes a 3D vector, resulting in a unit vector (length of 1).
// v_normalized = v / ||v||
vector3_t vector3Normalized(vector3_t vector)
{
    float magnitude = vector3Magnitude(vector);
    if (magnitude == 0) return vector;

    return (vector3_t){ vector.x / magnitude, vector.y / magnitude, vector.z / magnitude };
}

// Converts a 3D vector to a 4D vector (homogeneous coordinates).
// The w-component is set to 1.0, indicating it's a point in 3D space.
vector4_t vector3to4(vector3_t vector)
{
    return (vector4_t){ vector.x, vector.y, vector.z, 1 };
}

// Converts a 4D vector (homogeneous coordinates) to a 3D vector.
// The w-component is discarded.
vector3_t vector4to3(vector4_t vector)
{
    return (vector3_t){ vector.x, vector.y, vector.z };
}

// Converts a 4D vector to a 2D vector.
// The z and w components are discarded.
vector2_t vector4to2(vector4_t vector)
{
    return (vector2_t){ vector.x, vector.y };
}

// Creates a new 3D zero vector.
// v = (0, 0, 0)
vector3_t vector3New()
{
    return (vector3_t){ 0, 0, 0 };
}

// Creates a copy of a 3D vector.
vector3_t vector3Clone(vector3_t vector)
{
    return (vector3_t){ vector.x, vector.y, vector.z };
}