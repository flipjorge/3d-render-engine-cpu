#include <stddef.h>
#include "mesh.h"
#include "matrix.h"

void getMeshTransformedVertices(const mesh_t* mesh, vector3_t** tranformedVertices)
{
    const int numberVertices = array_length(mesh->vertices);

    matrix4_t scale = matrix4Scale(&mesh->scale);
    matrix4_t rotation = matrix4Rotation(&mesh->rotation);
    matrix4_t translation = matrix4Translation(&mesh->position);
    matrix4_t transform = matrix4TRS(&scale, &rotation, &translation);

    for (size_t i = 0; i < numberVertices; i++)
    {
        vector3_t vertice = matrix4MultiplyVector(&transform, &mesh->vertices[i]);
        array_push(*tranformedVertices, vertice);
    }
}

void freeMesh(mesh_t* mesh)
{
    if(mesh->vertices) array_free(mesh->vertices);
    if(mesh->faces) array_free(mesh->faces);
}