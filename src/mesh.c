#include <stddef.h>
#include <stdio.h>
#include "mesh.h"
#include "matrix.h"

matrix4_t getMeshTransformMatrix(const mesh_t* mesh)
{
    matrix4_t scaleMatrix = matrix4MakeScale(&mesh->scale);
    matrix4_t rotationMatrix = matrix4MakeRotation(&mesh->rotation);
    matrix4_t translationMatrix = matrix4MakeTranslation(&mesh->position);
    matrix4_t transformMatrix = matrix4TRS(&scaleMatrix, &rotationMatrix, &translationMatrix);

    return transformMatrix;
}

void freeMesh(mesh_t* mesh)
{
    if(mesh->vertices) array_free(mesh->vertices);
    if(mesh->faces) array_free(mesh->faces);
}