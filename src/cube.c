#include "cube.h"
#include <stddef.h>

vector3_t cubeVertices[8] = {
    { -1, -1, -1 },
    { -1, 1, -1 },
    { 1, 1, -1 },
    { 1, -1, -1 },
    { 1, -1, 1 },
    { 1, 1, 1 },
    { -1, 1, 1 },
    { -1, -1, 1 },
};

face_t cubeFaces[12] = {
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

void createCube(cube_t* cube, float size, vector3_t position)
{
    vector3_t vertices[8];

    for (size_t i = 0; i < 8; i++)
    {
        float x = cubeVertices[i].x * size;
        float y = cubeVertices[i].y * size;
        float z = cubeVertices[i].z * size;

        cube->vertices[i] = (vector3_t){ x, y, z };
    }

    for (size_t i = 0; i < 12; i++)
    {
        cube->faces[i] = cubeFaces[i];
    }
    
    cube->position = position;
}

void getCubeTransformedVertices(const cube_t* cube, vector3_t tranformedVertices[8])
{
    for (size_t i = 0; i < 8; i++)
    {
        tranformedVertices[i].x = cube->vertices[i].x + cube->position.x;
        tranformedVertices[i].y = cube->vertices[i].y + cube->position.y;
        tranformedVertices[i].z = cube->vertices[i].z + cube->position.z;
    }
}