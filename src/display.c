#include <stdbool.h>
#include "display.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture* colorBufferTexture = NULL;

const int windowWidth = 800;
const int windowHeight = 600;

uint32_t* colorBuffer = NULL;

bool initializeSDL()
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

    return true;
}

void clearColorBuffer(uint32_t color)
{
    for (size_t i = 0; i < windowWidth * windowHeight; i++)
    {
        colorBuffer[i] = color;
    }
}

void renderColorBuffer()
{
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

void clear() {
    free(colorBuffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void drawPixel(int x, int y, uint32_t color)
{
    if(x > windowWidth || y > windowHeight) return;
    
    colorBuffer[y * windowWidth + x] = color;
}

void drawGrid(uint8_t cellSize, uint32_t color)
{
    for (size_t y = 0; y < windowHeight; y++)
    {
        for (size_t x = 0; x < windowWidth; x++)
        {
            if (x % cellSize == 0 || y % cellSize == 0)
            {
                colorBuffer[y * windowWidth + x] = color;
            }
        }
    }
}

void drawRectangle(int x, int y, int width, int height, uint32_t color)
{
    for (size_t pixelY = y; pixelY < y + height; pixelY++)
    {
        for (size_t pixelX = x; pixelX < x + width; pixelX++)
        {
            colorBuffer[pixelY * windowWidth + pixelX] = color;
        }   
    }
}

void drawLine(int x0, int y0, int x1, int y1, uint32_t color)
{
    int deltaX = x1 - x0;
    int deltaY = y1 - y0;

    int longestSideLength = abs(deltaX) >= abs(deltaY) ? abs(deltaX) : abs(deltaY);

    float xIncrement = deltaX / (float)longestSideLength;
    float yIncrement = deltaY / (float)longestSideLength;

    float currentX = x0;
    float currentY = y0;

    for (size_t i = 0; i < longestSideLength; i++)
    {
        drawPixel(round(currentX), round(currentY), color);
        currentX += xIncrement;
        currentY += yIncrement;
    }
}