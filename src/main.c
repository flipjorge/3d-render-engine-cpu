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
#include "matrix.h"
#include "light.h"
#include "texture.h"

#define TARGET_FRAME_RATE 60
#define TARGET_FRAME_TIME (1000 / TARGET_FRAME_RATE)

bool isRunning = false;

mesh_t** meshes = NULL;
triangle_t* trianglesToRender = NULL;

mesh_t cube;
mesh_t piramid;

matrix4_t projectionMatrix;

Uint32 previousFrameTicks;

bool renderVertices;
bool renderLines;
bool renderFaces;
bool renderTextures;
bool backCulling;

light_t light;

uint32_t* texture = NULL;
upng_t* png = NULL;

void setup()
{
    colorBuffer = malloc(sizeof(uint32_t) * windowWidth * windowHeight);

    colorBufferTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        windowWidth,
        windowHeight);

    createCube(&cube, 2, (vector3_t){ 0, 0, 0 });
    loadMeshFromObj(&piramid, "./assets/piramid.obj"); 

    array_push(meshes, &cube);
    array_push(meshes, &piramid);

    projectionMatrix = matrix4MakePerspective( M_PI / 3, (float)windowHeight / (float)windowWidth, 0.01, 100 );
    
    previousFrameTicks = SDL_GetTicks();

    renderVertices = false;
    renderLines = false;
    renderFaces = false;
    renderTextures = true;
    backCulling = true;

    light = (light_t){
        (vector3_t){ 0, 0, 1 }
    };

    // int numberPixels = TEXTURE_WIDTH * TEXTURE_HEIGHT;
    // texture = convertARGBtoRGBATexture((const uint32_t*)sampleTexture, numberPixels);

    loadTextureFromPng("./assets/cube.png", &png, &texture);
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
        if(event.key.keysym.sym == SDLK_t)
        {
            renderTextures = !renderTextures;
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

    cube.position = (vector3_t){ 0, 0, 30 };
    cube.rotation = vector3Sum( cube.rotation, (vector3_t){ rotationIncrement, rotationIncrement, rotationIncrement } );
    cube.scale = (vector3_t){ 2, 2, 2};

    piramid.position = (vector3_t){ 0, 10, 30 };
    piramid.rotation = vector3Sum( piramid.rotation, (vector3_t){ 0, rotationIncrement, 0 } );
    piramid.scale = (vector3_t){ 2, 2, 2 };

    const int numMeshes = array_length(meshes);
    trianglesToRender = NULL;

    for (size_t m = 0; m < numMeshes; m++)
    {
        mesh_t* mesh = meshes[m];
        matrix4_t transformMatrix = getMeshTransformMatrix(mesh);

        const int numFaces = array_length(mesh->faces);

        for (size_t f = 0; f < numFaces; f++)
        {
            face_t face = mesh->faces[f];
            vector3_t faceVertices[3];
            
            faceVertices[0] = mesh->vertices[face.a - 1];
            faceVertices[1] = mesh->vertices[face.b - 1];
            faceVertices[2] = mesh->vertices[face.c - 1];
            
            vector4_t transformedVertices[3];

            for (size_t v = 0; v < 3; v++)
            {
                vector4_t transformedVertice = vector3to4(faceVertices[v]);
                transformedVertice = matrix4MultiplyVector4(&transformMatrix, &transformedVertice);
                transformedVertices[v] = transformedVertice;
            }

            triangle_t triangle;

            for (size_t v = 0; v < 3; v++)
            {
                vector4_t projectedVertex = matrix4MultiplyVector4Project(&projectionMatrix, &transformedVertices[v]);
                
                projectedVertex.x *= windowWidth / 2.0;
                projectedVertex.y *= windowHeight / 2.0;

                projectedVertex.y *= -1;
                
                projectedVertex.x += windowWidth / 2.0;
                projectedVertex.y += windowHeight / 2.0;

                triangle.points[v].x = projectedVertex.x;
                triangle.points[v].y = projectedVertex.y;
                triangle.points[v].z = projectedVertex.z;
                triangle.points[v].w = projectedVertex.w;
            }

            vector3_t verticesForBackCulling[3] = {
                vector4to3(transformedVertices[0]),
                vector4to3(transformedVertices[1]),
                vector4to3(transformedVertices[2])
            };

            if(backCulling && !isFaceFacingCamera((vector3_t){ 0, 0, 0 }, verticesForBackCulling)) continue;
            
            triangle.depth = (faceVertices[0].z + faceVertices[1].z + faceVertices[2].z)/3;
            
            const float intensityFactor = lightIntensityFactor(light.direction, verticesForBackCulling);
            triangle.color = lightApplyIntensity(0xFFFFFFFF, intensityFactor);

            triangle.textureCoordinates[0] = face.aUV;
            triangle.textureCoordinates[1] = face.bUV;
            triangle.textureCoordinates[2] = face.cUV;

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
                vector4_t vertex = triangle.points[j];
                drawRectangle(vertex.x - 2, vertex.y - 2, 4, 4, 0xFF0000FF);
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
                triangle.color
            );
        }

        if(renderTextures)
        {
            drawTexturedTriangle(
                triangle.points[0].x,
                triangle.points[0].y,
                triangle.points[0].z,
                triangle.points[0].w,
                triangle.textureCoordinates[0].u,
                triangle.textureCoordinates[0].v,
                triangle.points[1].x,
                triangle.points[1].y,
                triangle.points[1].z,
                triangle.points[1].w,
                triangle.textureCoordinates[1].u,
                triangle.textureCoordinates[1].v,
                triangle.points[2].x,
                triangle.points[2].y,
                triangle.points[2].z,
                triangle.points[2].w,
                triangle.textureCoordinates[2].u,
                triangle.textureCoordinates[2].v,
                texture
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
                0x0000000
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
    freeMesh(&piramid);
    if(meshes) array_free(meshes);
    if(trianglesToRender) array_free(trianglesToRender);

    return 0;
}