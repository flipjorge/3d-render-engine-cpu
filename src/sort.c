#include "sort.h"
#include "array/array.h"
#include <stddef.h>
#include <stdio.h>

void sortTrianglesByDepth(triangle_t* triangles)
{
    int numTriangles = array_length(triangles);

    for (size_t i = 0; i < numTriangles; i++)
    {
        for (size_t j = i + 1; j < numTriangles; j++)
        {
            if(triangles[i].depth < triangles[j].depth)
            {
                triangle_t temp = triangles[i];
                triangles[i] = triangles[j];
                triangles[j] = temp;
            }
        }
    }
}