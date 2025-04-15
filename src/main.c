#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
uint32_t* colorBuffer = NULL;
SDL_Texture* colorBufferTexture = NULL;

const int windowWidth = 800;
const int windowHeight = 600;
bool isRunning = false;

void initializeSDL()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(
        "My engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        0);

    renderer = SDL_CreateRenderer(window, -1, 0);

    isRunning = true;
}

void setup() {
    colorBuffer = malloc(sizeof(uint32_t) * windowWidth * windowHeight);

    colorBufferTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        windowWidth,
        windowHeight);
}

void clear() {
    free(colorBuffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
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

void clearColorBuffer(uint32_t color) {
    for (size_t i = 0; i < windowWidth * windowHeight; i++)
    {
        colorBuffer[i] = color;
    }
}

void renderColorBuffer() {
    SDL_UpdateTexture(
        colorBufferTexture,
        NULL,
        colorBuffer,
        sizeof(uint32_t) * windowWidth);

    SDL_RenderCopy(
        renderer,
        colorBufferTexture,
        NULL,
        NULL);
}

void drawGrid(uint8_t cellSize, uint32_t color) {
    for (size_t y = 0; y < windowHeight; y++) {
        for (size_t x = 0; x < windowWidth; x++) {
            if (x % cellSize == 0 || y % cellSize == 0) {
                colorBuffer[y * windowWidth + x] = color;
            }
        }
    }
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0 ,0, 255);
    SDL_RenderClear(renderer);

    drawGrid(40, 0xFFFFFFFF);

    renderColorBuffer();
    clearColorBuffer(0x000000FF);
    
    SDL_RenderPresent(renderer);
}

int main()
{
    initializeSDL();
    setup();

    while (isRunning)
    {
        processInput();
        render();
    }

    clear();

    return 0;
}