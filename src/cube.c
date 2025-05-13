#include "cube.h"
#include <stddef.h>
#include <stdio.h>

#define NUMBER_VERTICES 8
#define NUMBER_FACES 12

vector3_t cubeVertices[NUMBER_VERTICES] = {
    { -1, -1, -1 },
    { -1, 1, -1 },
    { 1, 1, -1 },
    { 1, -1, -1 },
    { 1, -1, 1 },
    { 1, 1, 1 },
    { -1, 1, 1 },
    { -1, -1, 1 },
};

face_t cubeFaces[NUMBER_FACES] = {
    { 0, 1, 2 },
    { 2, 4, 0 },
    { 3, 2, 5 },
    { 5, 4, 3 },
    { 4, 5, 6 },
    { 6, 7, 4 },
    { 7, 6, 1 },
    { 1, 0, 7 },
    { 1, 6, 5 },
    { 5, 2, 1 },
    { 7, 0, 3 },
    { 3, 4, 7 },
};

void createCube(mesh_t* mesh, float size, vector3_t position)
{
    mesh->vertices = NULL;
    mesh->faces = NULL;

    for (size_t i = 0; i < NUMBER_VERTICES; i++)
    {
        float x = cubeVertices[i].x * size;
        float y = cubeVertices[i].y * size;
        float z = cubeVertices[i].z * size;

        vector3_t vertice = (vector3_t){ x, y, z };

        array_push(mesh->vertices, vertice);
    }

    for (size_t i = 0; i < NUMBER_FACES; i++)
    {
        array_push(mesh->faces, cubeFaces[i]);
    }
    
    mesh->position = position;
}