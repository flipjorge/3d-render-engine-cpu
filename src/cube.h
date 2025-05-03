#ifndef CUBE_MESH
#define CUBE_MESH

#include "vector.h"
#include "triangle.h"

typedef struct {
    vector3_t vertices[8];
    face_t faces[12];
} cube_t;

vector3_t cubeVertices[8];
face_t cubeFaces[12];

void createCube(cube_t* cube, float size, vector3_t position);

#endif