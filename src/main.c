#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"
#include "projection.h"
#include "cube.h"

#define TARGET_FRAME_RATE 60
#define TARGET_FRAME_TIME (1000 / TARGET_FRAME_RATE)

bool isRunning = false;

cube_t cube;
triangle_t trianglesToRender[12];

Uint32 previousFrameTicks;

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

    previousFrameTicks = SDL_GetTicks();
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
    vector3_t cubeTranformedVertices[8];
    getCubeTransformedVertices(&cube, cubeTranformedVertices);

    for (size_t i = 0; i < 12; i++)
    {
        face_t face = cube.faces[i];
        vector3_t faceVertices[3];
        faceVertices[0] = cubeTranformedVertices[face.a];
        faceVertices[1] = cubeTranformedVertices[face.b];
        faceVertices[2] = cubeTranformedVertices[face.c];

        triangle_t triangle;

        for (size_t j = 0; j < 3; j++)
        {
            vector3_t vertex = faceVertices[j];

            // triangle.points[j] = projectOrtographic(vertex);
            triangle.points[j] = projectPerspective(180, vertex);
            triangle.points[j] = vector2Sum(triangle.points[j], (vector2_t){ windowWidth / 2, windowHeight / 2 });
        }

        trianglesToRender[i] = triangle;
    }
}

void render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0 ,0, 255);
    SDL_RenderClear(renderer);

    drawGrid(40, 0x333333FF);

    for (size_t i = 0; i < 12; i++)
    {
        triangle_t triangle = trianglesToRender[i];

        for (size_t j = 0; j < 3; j++)
        {
            vector2_t vertex = triangle.points[j];
            drawRectangle(vertex.x - 2, vertex.y - 2, 4, 4, 0xFF0000FF);
        }

        drawTriangle(
            triangle.points[0].x,
            triangle.points[0].y,
            triangle.points[1].x,
            triangle.points[1].y,
            triangle.points[2].x,
            triangle.points[2].y,
            0xFF0000FF
        );
    }
    
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

    return 0;
}