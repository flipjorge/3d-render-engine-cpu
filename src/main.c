#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"
#include "projection.h"
#include "cube.h"

bool isRunning = false;

cube_t cube;
vector2_t cubeProjectedPoints[8];

void setup()
{
    colorBuffer = malloc(sizeof(uint32_t) * windowWidth * windowHeight);

    colorBufferTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        windowWidth,
        windowHeight);

    createCube(&cube, 30, (vector3_t){ 0, 0, 0});
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
    for (size_t i = 0; i < 8; i++)
    {
        cubeProjectedPoints[i] = projectOrtographic(cube.vertices[i]);
        cubeProjectedPoints[i] = vector2Sum(cubeProjectedPoints[i], (vector2_t){60, 60});
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