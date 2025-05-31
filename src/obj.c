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
    
    texture_t* textures = NULL;

    while(fgets(line, 256, file))
    {
        if(strncmp(line, "v ", 2) == 0)
        {
            vector3_t vertice;
            sscanf(line, "v %f %f %f", &vertice.x, &vertice.y, &vertice.z);
            array_push(mesh->vertices, vertice);
        }

        if(strncmp(line, "vt ", 2) == 0)
        {
            texture_t texture;
            sscanf(line, "vt %f %f", &texture.u, &texture.v);
            array_push(textures, texture);
        }

        if(strncmp(line, "f ", 2) == 0)
        {
            int verticeIndices[3];
            int textureIndices[3];
            
            sscanf(line, "f %i/%i %i/%i %i/%i",
                &verticeIndices[0], &textureIndices[0],
                &verticeIndices[1], &textureIndices[1],
                &verticeIndices[2], &textureIndices[2]
            );
            
            face_t face = {
                .a = verticeIndices[0],
                .b = verticeIndices[1],
                .c = verticeIndices[2],
                .aUV = textures[textureIndices[0] - 1],
                .bUV = textures[textureIndices[1] - 1],
                .cUV = textures[textureIndices[2] - 1]
            };

            array_push(mesh->faces, face);
        }
    }
}