#ifndef VECTOR
#define VECTOR

// Represents a 2D vector with x and y components.
// In 3D rendering, this is commonly used for:
// - Texture coordinates (UV mapping) to map 2D textures onto 3D models.
// - Screen-space coordinates for positioning elements on the 2D display.
typedef struct {
    float x;
    float y;
} vector2_t;

// Represents a 3D vector with x, y, and z components.
// This is a fundamental data structure in 3D graphics used for:
// - Positions of vertices in 3D space.
// - Directional vectors (e.g., surface normals, light direction).
// - Storing RGB color data.
typedef struct {
    float x;
    float y;
    float z;
} vector3_t;

// Represents a 4D vector, primarily used for homogeneous coordinates.
// Homogeneous coordinates allow affine transformations (like translation) to be
// represented by a single 4x4 matrix multiplication.
// - The 'w' component is key:
//   - w=1: Represents a point in 3D space.
//   - w=0: Represents a direction in 3D space.
//   - w is also used as the divisor in perspective projection to create depth.
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

vector3_t vector3New();
vector3_t vector3Clone(vector3_t vector);

#endif