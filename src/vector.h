#ifndef VECTOR
#define VECTOR

typedef struct {
    float x;
    float y;
} vector2_t;

typedef struct {
    float x;
    float y;
    float z;
} vector3_t;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} vector4_t;

vector2_t vector2Sum(vector2_t a, vector2_t b);
vector2_t vector2Sub(vector2_t a, vector2_t b);
vector2_t vector2Multiple(vector2_t a, vector2_t b);

vector3_t vector3Sum(vector3_t a, vector3_t b);
vector3_t vector3Sub(vector3_t a, vector3_t b);
vector3_t vector3Multiple(vector3_t a, vector3_t b);

vector3_t vector3RotateX(vector3_t vector, float angle);
vector3_t vector3RotateY(vector3_t vector, float angle);
vector3_t vector3RotateZ(vector3_t vector, float angle);

float vector2Magnitude(vector2_t vector);
float vector3Magnitude(vector3_t vector);

vector3_t vector3CrossProduct(vector3_t a, vector3_t b);
float vector3DotProduct(vector3_t a, vector3_t b);

vector2_t vector2Normalized(vector2_t vector);
vector3_t vector3Normalized(vector3_t vector);

vector4_t vector3to4(vector3_t vector);
vector3_t vector4to3(vector4_t vector);
vector2_t vector4to2(vector4_t vector);

#endif