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
vector2_t cubeProjectedPoints[8];

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

    createCube(&cube, 5, (vector3_t){ 0, 0, 0 });

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

    for (size_t i = 0; i < 8; i++)
    {
        //cubeProjectedPoints[i] = projectOrtographic(cubeTranformedVertices[i]);
        cubeProjectedPoints[i] = projectPerspective(180, cubeTranformedVertices[i]);
        cubeProjectedPoints[i] = vector2Sum(cubeProjectedPoints[i], (vector2_t){ windowWidth / 2, windowHeight / 2 });
    }
}

void render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0 ,0, 255);
    SDL_RenderClear(renderer);

    drawGrid(40, 0x333333FF);

    for (size_t i = 0; i < 8; i++)
    {
        drawRectangle(cubeProjectedPoints[i].x, cubeProjectedPoints[i].y, 5, 5, 0xFF0000FF);
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