#include <stdbool.h>
#include "display.h"
#include "vector.h"
#include "texture.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* colorBufferTexture = NULL;

static const int windowWidth = 800;
static const int windowHeight = 600;

static uint32_t* colorBuffer = NULL;
static float* depthBuffer = NULL;

void initializeWindow(bool* isRunning)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        *isRunning = false;
        return;
    }

    window = SDL_CreateWindow(
        "3d Software Rendering Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        0);

    if (!window) {
        *isRunning = false;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer) {
        *isRunning = false;
        return;
    }

    colorBufferTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        windowWidth,
        windowHeight);

    colorBuffer = malloc(sizeof(uint32_t) * windowWidth * windowHeight);
    depthBuffer = malloc(sizeof(float) * windowWidth * windowHeight);

    *isRunning = true;
}

void destroyWindow() {
    free(colorBuffer);
    free(depthBuffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int getWindowWidth()
{
    return windowWidth;
}

int getWindowHeight()
{
    return windowHeight;
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

    SDL_RenderPresent(renderer);
}

void clearDepthBuffer()
{
    for (size_t i = 0; i < windowWidth * windowHeight; i++)
    {
        depthBuffer[i] = 1.0;
    }
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

void floatSwap(float* a, float* b)
{
    float temp = *a;
    *a = *b;
    *b = temp;
}

vector3_t barycentricWeights(vector2_t a, vector2_t b, vector2_t c, vector2_t p) {
    
    vector2_t ac = vector2Sub(c, a);
    vector2_t ab = vector2Sub(b, a);
    vector2_t ap = vector2Sub(p, a);
    vector2_t pc = vector2Sub(c, p);
    vector2_t pb = vector2Sub(b, p);

    float areaParallelogramABC = (ac.x * ab.y - ac.y * ab.x);

    float alpha = (pc.x * pb.y - pc.y * pb.x) / areaParallelogramABC;
    float beta = (ac.x * ap.y - ac.y * ap.x) / areaParallelogramABC;
    float gamma = 1 - alpha - beta;

    vector3_t weights = { alpha, beta, gamma };
    return weights;
}

void drawTrianglePixel(
    int x, int y, uint32_t color,
    vector4_t pointA, vector4_t pointB, vector4_t pointC
) {
    vector2_t p = { x, y };
    vector2_t a = vector4to2(pointA);
    vector2_t b = vector4to2(pointB);
    vector2_t c = vector4to2(pointC);

    vector3_t weights = barycentricWeights(a, b, c, p);

    float alpha = weights.x;
    float beta = weights.y;
    float gamma = weights.z;

    float interpolatedW = (1 / pointA.w) * alpha + (1 / pointB.w) * beta + (1 / pointC.w) * gamma;

    int pixelIndex = windowWidth * y + x;

    interpolatedW = 1 - interpolatedW;

    if(interpolatedW < depthBuffer[pixelIndex]) {
        drawPixel(x, y, color);
        depthBuffer[pixelIndex] = interpolatedW;
    }
}

void drawTexel(
    int x, int y, uint32_t* texture,
    vector4_t pointA, vector4_t pointB, vector4_t pointC,
    texture_t textureA, texture_t textureB, texture_t textureC
) {
    vector2_t p = { x, y };
    vector2_t a = vector4to2(pointA);
    vector2_t b = vector4to2(pointB);
    vector2_t c = vector4to2(pointC);

    vector3_t weights = barycentricWeights(a, b, c, p);

    float alpha = weights.x;
    float beta = weights.y;
    float gamma = weights.z;

    float interpolatedU = (textureA.u / pointA.w) * alpha + (textureB.u / pointB.w) * beta + (textureC.u / pointC.w) * gamma;
    float interpolatedV = (textureA.v / pointA.w) * alpha + (textureB.v / pointB.w) * beta + (textureC.v / pointC.w) * gamma;
    float interpolatedW = (1 / pointA.w) * alpha + (1 / pointB.w) * beta + (1 / pointC.w) * gamma;

    interpolatedU /= interpolatedW;
    interpolatedV /= interpolatedW;

    int textureX = abs((int)(interpolatedU * TEXTURE_WIDTH)) % TEXTURE_WIDTH;
    int textureY = abs((int)(interpolatedV * TEXTURE_HEIGHT)) % TEXTURE_HEIGHT;

    int pixelIndex = windowWidth * y + x;

    interpolatedW = 1 - interpolatedW;

    if(interpolatedW < depthBuffer[pixelIndex]) {
        drawPixel(x, y, texture[(64 * textureY) + textureX]);
        depthBuffer[pixelIndex] = interpolatedW;
    }
}

void drawFilledTriangle(
    int x0, int y0, int z0, int w0,
    int x1, int y1, int z1, int w1,
    int x2, int y2, int z2, int w2,
    const uint32_t color)
{
    if(y0 > y1)
    {
        intSwap(&x0, &x1);
        intSwap(&y0, &y1);
        intSwap(&z0, &z1);
        intSwap(&w0, &w1);
    }

    if(y1 > y2)
    {
        intSwap(&x1, &x2);
        intSwap(&y1, &y2);
        intSwap(&z1, &z2);
        intSwap(&w1, &w2);
    }

    if(y0 > y1)
    {
        intSwap(&x0, &x1);
        intSwap(&y0, &y1);
        intSwap(&z0, &z1);
        intSwap(&w0, &w1);
    }

    vector4_t pointA = { x0, y0, z0, w0 };
    vector4_t pointB = { x1, y1, z1, w1 };
    vector4_t pointC = { x2, y2, z2, w2 };

    float invertedSlopeLeft = 0;
    float invertedSlopeRight = 0;

    if (y1 - y0 != 0) invertedSlopeLeft = (float)(x1 - x0) / abs(y1 - y0);
    if (y2 - y0 != 0) invertedSlopeRight = (float)(x2 - x0) / abs(y2 - y0);

    if (y1 - y0 != 0) {
        for (int y = y0; y <= y1; y++) {
            int xStart = x1 + (y - y1) * invertedSlopeLeft;
            int xEnd = x0 + (y - y0) * invertedSlopeRight;

            if (xEnd < xStart) {
                intSwap(&xStart, &xEnd);
            }

            for (int x = xStart; x < xEnd; x++) {
                drawPixel(x, y, color);
            }
        }
    }

    invertedSlopeLeft = 0;
    invertedSlopeRight = 0;

    if (y2 - y1 != 0) invertedSlopeLeft = (float)(x2 - x1) / abs(y2 - y1);
    if (y2 - y0 != 0) invertedSlopeRight = (float)(x2 - x0) / abs(y2 - y0);

    if (y2 - y1 != 0) {
        for (int y = y1; y <= y2; y++) {
            int xStart = x1 + (y - y1) * invertedSlopeLeft;
            int xEnd = x0 + (y - y0) * invertedSlopeRight;

            if (xEnd < xStart) {
                intSwap(&xStart, &xEnd);
            }

            for (int x = xStart; x < xEnd; x++) {
                drawTrianglePixel(x, y, color, pointA, pointB, pointC);
            }
        }
    }
}

void drawTexturedTriangle(
    int x0, int y0, int z0, int w0, float u0, float v0,
    int x1, int y1, int z1, int w1, float u1, float v1,
    int x2, int y2, int z2, int w2, float u2, float v2,
    uint32_t* texture
)
{
    if(y0 > y1)
    {
        intSwap(&x0, &x1);
        intSwap(&y0, &y1);
        intSwap(&z0, &z1);
        intSwap(&w0, &w1);
        floatSwap(&u0, &u1);
        floatSwap(&v0, &v1);
    }

    if(y1 > y2)
    {
        intSwap(&x1, &x2);
        intSwap(&y1, &y2);
        intSwap(&z1, &z2);
        intSwap(&w1, &w2);
        floatSwap(&u1, &u2);
        floatSwap(&v1, &v2);
    }

    if(y0 > y1)
    {
        intSwap(&x0, &x1);
        intSwap(&y0, &y1);
        intSwap(&z0, &z1);
        intSwap(&w0, &w1);
        floatSwap(&u0, &u1);
        floatSwap(&v0, &v1);
    }

    vector4_t pointA = { x0, y0, z0, w0 };
    vector4_t pointB = { x1, y1, z1, w1 };
    vector4_t pointC = { x2, y2, z2, w2 };
    texture_t textureA = { u0, v0 };
    texture_t textureB = { u1, v1 };
    texture_t textureC = { u2, v2 };

    float invertedSlopeLeft = 0;
    float invertedSlopeRight = 0;

    if (y1 - y0 != 0) invertedSlopeLeft = (float)(x1 - x0) / abs(y1 - y0);
    if (y2 - y0 != 0) invertedSlopeRight = (float)(x2 - x0) / abs(y2 - y0);

    if (y1 - y0 != 0) {
        for (int y = y0; y <= y1; y++) {
            int xStart = x1 + (y - y1) * invertedSlopeLeft;
            int xEnd = x0 + (y - y0) * invertedSlopeRight;

            if (xEnd < xStart) {
                intSwap(&xStart, &xEnd);
            }

            for (int x = xStart; x < xEnd; x++) {
                drawTexel(x, y, texture, pointA, pointB, pointC, textureA, textureB, textureC);
            }
        }
    }

    invertedSlopeLeft = 0;
    invertedSlopeRight = 0;

    if (y2 - y1 != 0) invertedSlopeLeft = (float)(x2 - x1) / abs(y2 - y1);
    if (y2 - y0 != 0) invertedSlopeRight = (float)(x2 - x0) / abs(y2 - y0);

    if (y2 - y1 != 0) {
        for (int y = y1; y <= y2; y++) {
            int xStart = x1 + (y - y1) * invertedSlopeLeft;
            int xEnd = x0 + (y - y0) * invertedSlopeRight;

            if (xEnd < xStart) {
                intSwap(&xStart, &xEnd);
            }

            for (int x = xStart; x < xEnd; x++) {
                drawTexel(x, y, texture, pointA, pointB, pointC, textureA, textureB, textureC);
            }
        }
    }
}