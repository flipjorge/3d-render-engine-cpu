#include "mesh.h"

void freeMesh(mesh_t* mesh)
{
    if(mesh->vertices) array_free(mesh->vertices);
    if(mesh->faces) array_free(mesh->faces);
}