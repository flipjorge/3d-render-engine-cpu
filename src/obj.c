#include "obj.h"
#include <string.h>
#include <stdio.h>

void loadMeshFromObj(mesh_t* mesh, char* filename)
{
    FILE* file;
    file = fopen(filename, "r");
    char line[256];

    mesh->vertices = NULL;
    mesh->faces = NULL;
    
    while(fgets(line, 256, file))
    {
        if(strncmp(line, "v ", 2) == 0)
        {
            vector3_t vertice;
            sscanf(line, "v %f %f %f", &vertice.x, &vertice.y, &vertice.z);
            array_push(mesh->vertices, vertice);
        }

        if(strncmp(line, "f ", 2) == 0)
        {
            face_t face;
            sscanf(line, "f %i %i %i", &face.a, &face.b, &face.c);
            array_push(mesh->faces, face);
        }
    }
}