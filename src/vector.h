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

vector2_t vector2Sum(vector2_t a, vector2_t b);
vector2_t vector2Multiple(vector2_t a, vector2_t b);

vector3_t vector3Sum(vector3_t a, vector3_t b);
vector3_t vector3Multiple(vector3_t a, vector3_t b);

#endif