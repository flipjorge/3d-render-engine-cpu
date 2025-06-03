#ifndef DISPLAY
#define DISPLAY

#include <SDL2/SDL.h>

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture* colorBufferTexture;

const int windowWidth;
const int windowHeight;

uint32_t* colorBuffer;
float* depthBuffer;

bool initializeSDL();
void clearColorBuffer(uint32_t color);
void clearDepthBuffer();
void renderColorBuffer();
void clear();

void drawPixel(int x, int y, uint32_t color);
void drawGrid(uint8_t cellSize, uint32_t color);
void drawRectangle(int x, int y, int width, int height, uint32_t color);
void drawLine(int x0, int y0, int x1, int y1, uint32_t color);
void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void drawFilledTriangle(
    int x0, int y0, int z0, int w0,
    int x1, int y1, int z1, int w1,
    int x2, int y2, int z2, int w2,
    const uint32_t color);
void drawTexturedTriangle(
    int x0, int y0, int z0, int w0, float u0, float v0,
    int x1, int y1, int z1, int w1, float u1, float v1,
    int x2, int y2, int z2, int w2, float u2, float v2,
    uint32_t* texture
);

#endif