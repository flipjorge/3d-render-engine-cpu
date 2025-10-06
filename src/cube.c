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
    { 1, 2, 3, {0, 1}, {0, 0}, {1, 0} },
    { 3, 4, 1, {1, 0}, {1, 1}, {0, 1} },
    // right
    { 4, 3, 6, {0, 1}, {0, 0}, {1, 0} },
    { 6, 5, 4, {1, 0}, {1, 1}, {0, 1} },
    // back
    { 5, 6, 7, {0, 1}, {0, 0}, {1, 0} },
    { 7, 8, 5, {1, 0}, {1, 1}, {0, 1} },
    // left
    { 8, 7, 2, {0, 1}, {0, 0}, {1, 0} },
    { 2, 1, 8, {1, 0}, {1, 1}, {0, 1} },
    // top
    { 2, 7, 6, {0, 1}, {0, 0}, {1, 0} },
    { 6, 3, 2, {1, 0}, {1, 1}, {0, 1} },
    //bottom
    { 8, 1, 4, {0, 1}, {0, 0}, {1, 0} },
    { 4, 5, 8, {1, 0}, {1, 1}, {0, 1} },
};

// Procedurally generates a cube mesh and initializes its properties.
// This function provides a way to create a basic geometric primitive without needing to load a file.
// It's part of the asset creation stage, populating a `mesh_t` structure with hardcoded vertex and face data.
//
// The process involves:
// 1. Scaling the 8 vertices of a unit cube template by the given `size`.
// 2. Copying the 12 predefined triangular faces into the mesh's face array. These faces include
//    hardcoded UV coordinates for texture mapping.
// 3. Setting the mesh's initial position in world space.
// 4. Initializing the mesh's rotation to zero and scale to one.
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