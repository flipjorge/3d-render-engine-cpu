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
    // front
    { 1, 2, 3, {0, 0}, {0, 1}, {1, 1} },
    { 3, 4, 1, {1, 1}, {1, 0}, {0, 0} },
    // right
    { 4, 3, 6, {0, 0}, {0, 1}, {1, 1} },
    { 6, 5, 4, {1, 1}, {1, 0}, {0, 0} },
    // back
    { 5, 6, 7, {0, 0}, {0, 1}, {1, 1} },
    { 7, 8, 5, {1, 1}, {1, 0}, {0, 0} },
    // left
    { 8, 7, 2, {0, 0}, {0, 1}, {1, 1} },
    { 2, 1, 8, {1, 1}, {1, 0}, {0, 0} },
    // top
    { 2, 7, 6, {0, 0}, {0, 1}, {1, 1} },
    { 6, 3, 2, {1, 1}, {1, 0}, {0, 0} },
    //bottom
    { 8, 1, 4, {0, 0}, {0, 1}, {1, 1} },
    { 4, 5, 8, {1, 1}, {1, 0}, {0, 0} },
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
    mesh->rotation = (vector3_t){ 0, 0, 0 };
    mesh->scale = (vector3_t){ 1, 1, 1 };
}