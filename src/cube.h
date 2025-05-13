#ifndef CUBE_MESH
#define CUBE_MESH

#include "vector.h"
#include "triangle.h"
#include "mesh.h"

void createCube(mesh_t* mesh, float size, vector3_t position);
void getCubeTransformedVertices(const mesh_t* mesh, vector3_t** tranformedVertices);

#endif