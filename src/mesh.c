#include <stddef.h>
#include <stdio.h>
#include "mesh.h"
#include "matrix.h"
#include "obj.h"

#define MAX_MESHES 10

static mesh_t meshes[MAX_MESHES];
static int meshCount = 0;

mesh_t* loadMesh(char* filename)
{
    loadMeshFromObj(&meshes[meshCount], filename);
    
    meshes[meshCount].position = (vector3_t){ 0, 0, 0 };
    meshes[meshCount].rotation = (vector3_t){ 0, 0, 0 };
    meshes[meshCount].scale = (vector3_t){ 1, 1, 1 };

    mesh_t* mesh = &meshes[meshCount];

    meshCount++;

    return mesh;
}

int getNumberMeshes()
{
    return meshCount;
}

mesh_t* getMesh(int index)
{
    return &meshes[index];
}

matrix4_t getMeshTransformMatrix(const mesh_t* mesh)
{
    matrix4_t scaleMatrix = matrix4MakeScale(&mesh->scale);
    matrix4_t rotationMatrix = matrix4MakeRotation(&mesh->rotation);
    matrix4_t translationMatrix = matrix4MakeTranslation(&mesh->position);
    matrix4_t transformMatrix = matrix4TRS(&scaleMatrix, &rotationMatrix, &translationMatrix);

    return transformMatrix;
}

void freeAllMeshes()
{
    for (size_t i = 0; i < meshCount; i++)
    {
        array_free(meshes[i].vertices);
        array_free(meshes[i].faces);
    }
}