#include "mesh.h"
#include <stddef.h>

void getMeshTransformedVertices(const mesh_t* mesh, vector3_t** tranformedVertices)
{
    const int numberVertices = array_length(mesh->vertices);

    for (size_t i = 0; i < numberVertices; i++)
    {
        vector3_t vertice;

        vertice = vector3RotateX(mesh->vertices[i], mesh->rotation.x);
        vertice = vector3RotateY(vertice, mesh->rotation.y);
        vertice = vector3RotateZ(vertice, mesh->rotation.z);
        vertice = vector3Sum(vertice, mesh->position);

        array_push(*tranformedVertices, vertice);
    }
}

void freeMesh(mesh_t* mesh)
{
    if(mesh->vertices) array_free(mesh->vertices);
    if(mesh->faces) array_free(mesh->faces);
}