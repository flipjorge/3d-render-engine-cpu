#ifndef MESH
#define MESH

#include "array/array.h"
#include "vector.h"
#include "triangle.h"
#include "matrix.h"

typedef struct {
    vector3_t* vertices;
    face_t* faces;
    vector3_t position;
    vector3_t rotation;
    vector3_t scale;
} mesh_t;

matrix4_t getMeshTransformMatrix(const mesh_t* mesh);
void freeMesh(mesh_t* mesh);

#endif