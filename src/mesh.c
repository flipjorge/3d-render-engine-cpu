#include <stddef.h>
#include <stdio.h>
#include "mesh.h"
#include "matrix.h"
#include "obj.h"

#define MAX_MESHES 10

static mesh_t meshes[MAX_MESHES];
static int meshCount = 0;

// Loads a mesh from a file, initializes its transformation properties, and adds it to the scene.
// This function is part of the asset loading stage, preparing geometric data before rendering.
// It sets the mesh's initial position, rotation, and scale to default values, which correspond
// to an identity transformation (no change in position, orientation, or size).
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

// Returns the total number of meshes currently loaded in the scene.
// This is a utility function used in the main rendering loop to iterate through all meshes
// that need to be processed and drawn in each frame.
int getNumberMeshes()
{
    return meshCount;
}

// Retrieves a specific mesh from the scene's list by its index.
// This is used within the main rendering loop to get a handle to each mesh individually
// for transformation, culling, and rasterization.
mesh_t* getMesh(int index)
{
    return &meshes[index];
}

// Computes the model-to-world transformation matrix for a given mesh.
// This matrix is used in the vertex processing stage of the rendering pipeline to transform
// a mesh's vertices from its local model space into global world space.
//
// The final transformation matrix is a combination of Scale (S), Rotation (R), and Translation (T) matrices.
// The order of multiplication is T * R * S. This means a vertex is first scaled, then rotated,
// and finally translated to its final position in the world.
// M_world = M_translation * M_rotation * M_scale
matrix4_t getMeshTransformMatrix(const mesh_t* mesh)
{
    matrix4_t scaleMatrix = matrix4MakeScale(&mesh->scale);
    matrix4_t rotationMatrix = matrix4MakeRotation(&mesh->rotation);
    matrix4_t translationMatrix = matrix4MakeTranslation(&mesh->position);
    matrix4_t transformMatrix = matrix4TRS(&scaleMatrix, &rotationMatrix, &translationMatrix);

    return transformMatrix;
}

// Frees the memory allocated for the vertex and face arrays of all loaded meshes.
// This is a cleanup function called at the end of the program's execution to prevent memory leaks
// by releasing the dynamically allocated memory used by the mesh data.
void freeAllMeshes()
{
    for (size_t i = 0; i < meshCount; i++)
    {
        array_free(meshes[i].vertices);
        array_free(meshes[i].faces);
    }
}