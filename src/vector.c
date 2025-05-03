#include "vector.h"

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