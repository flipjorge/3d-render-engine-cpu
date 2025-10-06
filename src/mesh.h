#ifndef MESH
#define MESH

#include "array/array.h"
#include "vector.h"
#include "triangle.h"
#include "matrix.h"

// Represents a 3D object in the scene, containing its geometry and transformation data.
// This is a central data structure for any renderable object in the project.
typedef struct {
    vector3_t* vertices;
    face_t* faces;
    vector3_t position;
    vector3_t rotation;
    vector3_t scale;
} mesh_t;

mesh_t* loadMesh(char* filename);
int getNumberMeshes();
mesh_t* getMesh(int index);
matrix4_t getMeshTransformMatrix(const mesh_t* mesh);
void freeAllMeshes();

#endif