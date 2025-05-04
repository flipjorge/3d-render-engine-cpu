#ifndef CUBE_MESH
#define CUBE_MESH

#include "vector.h"
#include "triangle.h"

typedef struct {
    vector3_t vertices[8];
    face_t faces[12];
    vector3_t position;
    vector3_t rotation;
} cube_t;

vector3_t cubeVertices[8];
face_t cubeFaces[12];

void createCube(cube_t* cube, float size, vector3_t position);
void getCubeTransformedVertices(const cube_t* cube, vector3_t tranformedVertices[8]);

#endif