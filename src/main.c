#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"

bool isRunning = false;

void setup()
{
    colorBuffer = malloc(sizeof(uint32_t) * windowWidth * windowHeight);

    colorBufferTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        windowWidth,
        windowHeight);
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

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0 ,0, 255);
    SDL_RenderClear(renderer);

    drawGrid(40, 0xFFFFFFFF);

    drawRectangle(80,40,360,160, 0xFF0000FF);

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
        render();
    }

    clear();

    return 0;
}