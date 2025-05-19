#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "array/array.h"
#include "display.h"
#include "vector.h"
#include "projection.h"
#include "cube.h"
#include "obj.h"
#include "face.h"
#include "sort.h"

#define TARGET_FRAME_RATE 60
#define TARGET_FRAME_TIME (1000 / TARGET_FRAME_RATE)

bool isRunning = false;

mesh_t** meshes = NULL;
triangle_t* trianglesToRender = NULL;

mesh_t cube;
mesh_t piramid;

Uint32 previousFrameTicks;

bool renderVertices;
bool renderLines;
bool renderFaces;
bool backCulling;

void setup()
{
    colorBuffer = malloc(sizeof(uint32_t) * windowWidth * windowHeight);

    colorBufferTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        windowWidth,
        windowHeight);

    createCube(&cube, 10, (vector3_t){ 0, 0, 0 });
    loadMeshFromObj(&piramid, "./assets/piramid.obj");

    array_push(meshes, &cube);
    array_push(meshes, &piramid);

    previousFrameTicks = SDL_GetTicks();

    renderVertices = true;
    renderLines = true;
    renderFaces = true;
    backCulling = true;
}

void processInput()
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        break;

    case SDL_KEYDOWN:
        if(event.key.keysym.sym == SDLK_v)
        {
            renderVertices = !renderVertices;
        }
        if(event.key.keysym.sym == SDLK_l)
        {
            renderLines = !renderLines;
        }
        if(event.key.keysym.sym == SDLK_f)
        {
            renderFaces = !renderFaces;
        }
        if(event.key.keysym.sym == SDLK_b)
        {
            backCulling = !backCulling;
        }
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            isRunning = false;
        }
        break;
    }
}

void update()
{
    int frameTime = SDL_GetTicks() - previousFrameTicks;
    
    if(frameTime < TARGET_FRAME_TIME)
    {
        SDL_Delay(TARGET_FRAME_TIME - frameTime);
        frameTime = SDL_GetTicks() - previousFrameTicks;
    }

    float frameTimeSeconds = frameTime / 1000.0f;
    previousFrameTicks = SDL_GetTicks();

    float rotationIncrement = 1 * frameTimeSeconds;

    cube.position = (vector3_t){ 0, -10, 30 };
    cube.rotation = vector3Sum( cube.rotation, (vector3_t){ rotationIncrement, rotationIncrement, rotationIncrement } );

    piramid.position = (vector3_t){ 0, 20, 30 };
    piramid.rotation = vector3Sum( piramid.rotation, (vector3_t){ 0, rotationIncrement, 0 } );
    piramid.scale = (vector3_t){ 10, -10, 10 };

    const int numMeshes = array_length(meshes);
    trianglesToRender = NULL;

    for (size_t i = 0; i < numMeshes; i++)
    {
        mesh_t* mesh = meshes[i];

        vector3_t* transformedVertices = NULL;
        getMeshTransformedVertices(mesh, &transformedVertices);

        const int numFaces = array_length(mesh->faces);

        for (size_t j = 0; j < numFaces; j++)
        {
            face_t face = mesh->faces[j];
            vector3_t faceVertices[3];
            
            int numVertices = array_length(transformedVertices);
            
            faceVertices[0] = transformedVertices[face.a - 1];
            faceVertices[1] = transformedVertices[face.b - 1];
            faceVertices[2] = transformedVertices[face.c - 1];
            
            triangle_t triangle;
            
            for (size_t l = 0; l < 3; l++)
            {
                vector3_t vertex = faceVertices[l];
                
                // triangle.points[l] = projectOrtographic(vertex);
                triangle.points[l] = projectPerspective(180, vertex);
                triangle.points[l] = vector2Sum(triangle.points[l], (vector2_t){ windowWidth / 2, windowHeight / 2 });
            }
            
            if(backCulling && !isFaceFacingCamera((vector3_t){ 0, 0, 0 }, faceVertices)) continue;
            
            triangle.depth = (faceVertices[0].z + faceVertices[1].z + faceVertices[2].z)/3;
            
            array_push(trianglesToRender, triangle);
        }
    }
    
    sortTrianglesByDepth(trianglesToRender);
}

void render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0 ,0, 255);
    SDL_RenderClear(renderer);

    drawGrid(40, 0x333333FF);

    const int numberTriangles = array_length(trianglesToRender);

    for (size_t i = 0; i < numberTriangles; i++)
    {
        triangle_t triangle = trianglesToRender[i];

        if(renderVertices)
        {
            for (size_t j = 0; j < 3; j++)
            {
                vector2_t vertex = triangle.points[j];
                drawRectangle(vertex.x - 2, vertex.y - 2, 4, 4, 0xFFFFFFFF);
            }
        }

        if(renderFaces)
        {
            drawFilledTriangle(
                triangle.points[0].x,
                triangle.points[0].y,
                triangle.points[1].x,
                triangle.points[1].y,
                triangle.points[2].x,
                triangle.points[2].y,
                0xFF0000FF
            );
        }

        if(renderLines)
        {
            drawTriangle(
                triangle.points[0].x,
                triangle.points[0].y,
                triangle.points[1].x,
                triangle.points[1].y,
                triangle.points[2].x,
                triangle.points[2].y,
                0xFFFFFF
            );
        }
    }
    
    array_free(trianglesToRender);

    renderColorBuffer();
    clearColorBuffer(0x000000FF);
    
    SDL_RenderPresent(renderer);
}

int main()
{
    isRunning = initializeSDL();
    setup();

    while (isRunning)
    {
        processInput();
        update();
        render();
    }

    clear();
    
    freeMesh(&cube);
    if(trianglesToRender) array_free(trianglesToRender);

    return 0;
}