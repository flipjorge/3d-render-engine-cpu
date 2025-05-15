#include "vector.h"
#include <math.h>

vector2_t vector2Sum(vector2_t a, vector2_t b)
{
    return (vector2_t){ a.x + b.x, a.y + b.y };
}

vector2_t vector2Multiple(vector2_t a, vector2_t b)
{
    return (vector2_t){ a.x * b.x, a.y * b.y };
}

vector3_t vector3Sum(vector3_t a, vector3_t b)
{
    return (vector3_t){ a.x + b.x, a.y + b.y, a.z + b.z };
}

vector3_t vector3Multiple(vector3_t a, vector3_t b)
{
    return (vector3_t){ a.x * b.x, a.y * b.y, a.z * b.z };
}

vector3_t vector3RotateX(vector3_t vector, float angle)
{
    vector3_t rotatedVector = {
        .x = vector.x,
        .y = vector.y * cos(angle) - vector.z * sin(angle),
        .z = vector.y * sin(angle) + vector.z * cos(angle)
    };

    return rotatedVector;
}

vector3_t vector3RotateY(vector3_t vector, float angle)
{
    vector3_t rotatedVector = {
        .x = vector.x * cos(angle) - vector.z * sin(angle),
        .y = vector.y,
        .z = vector.x * sin(angle) + vector.z * cos(angle)
    };

    return rotatedVector;
}

vector3_t vector3RotateZ(vector3_t vector, float angle)
{
    vector3_t rotatedVector = {
        .x = vector.x * cos(angle) - vector.y * sin(angle),
        .y = vector.x * sin(angle) + vector.y * cos(angle),
        .z = vector.z
    };

    return rotatedVector;
}

float vector2Magnitude(const vector2_t vector)
{
    return sqrt(vector.x * vector.x + vector.y * vector.y);
}

float vector3Magnitude(const vector3_t vector)
{
    return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}