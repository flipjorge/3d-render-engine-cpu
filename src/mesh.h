#ifndef MESH
#define MESH

#include "vector.h"
#include "triangle.h"
#include "array/array.h"

typedef struct {
    vector3_t* vertices;
    face_t* faces;
    vector3_t position;
    vector3_t rotation;
} mesh_t;

void getMeshTransformedVertices(const mesh_t* mesh, vector3_t** tranformedVertices);
void freeMesh(mesh_t* mesh);

#endif