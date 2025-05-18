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

void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color);
}

void intSwap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void fillFlatBottom(int x0, int y0, int x1, int y1, int x2, int y2, const uint32_t color)
{
    float slope1 = (float)(x1 - x0) / (float)(y1 - y0);
    float slope2 = (float)(x2 - x0) / (float)(y2 - y0);
    
    float xStart = x0;
    float xEnd = x0;

    for (size_t y = y0; y <= y2; y++)
    {
        drawLine(xStart, y, xEnd, y, color);
        xStart += slope1;
        xEnd += slope2;
    }
}

void fillFlatTop(int x0, int y0, int x1, int y1, int x2, int y2, const uint32_t color)
{
    float slope1 = (float)(x2 - x0) / (float)(y2 - y0);
    float slope2 = (float)(x2 - x1) / (float)(y2 - y1);
    
    float xStart = x2;
    float xEnd = x2;

    for (size_t y = y2; y >= y0; y--)
    {
        drawLine(xStart, y, xEnd, y, color);
        xStart -= slope1;
        xEnd -= slope2;
    }
}

void drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, const uint32_t color)
{
    if(y0 > y1)
    {
        intSwap(&x0, &x1);
        intSwap(&y0, &y1);
    }

    if(y1 > y2)
    {
        intSwap(&x1, &x2);
        intSwap(&y1, &y2);
    }

    if(y0 > y1)
    {
        intSwap(&x0, &x1);
        intSwap(&y0, &y1);
    }

    if (y1 == y2) {
        fillFlatBottom(x0, y0, x1, y1, x2, y2, color);
    } else if (y0 == y1) {
        fillFlatTop(x0, y0, x1, y1, x2, y2, color);
    } else {
        int mY = y1;
        int mX = ((float)((x2 - x0) * (y1 - y0)) / (float)(y2 - y0)) + x0;

        fillFlatBottom(x0, y0, x1, y1, mX, mY, color);
        fillFlatTop(x1, y1, mX, mY, x2, y2, color);
    }
}