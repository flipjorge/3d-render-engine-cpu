#ifndef DISPLAY
#define DISPLAY

#include <SDL2/SDL.h>

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture* colorBufferTexture;

const int windowWidth;
const int windowHeight;

uint32_t* colorBuffer;

bool initializeSDL();
void clearColorBuffer(uint32_t color);
void renderColorBuffer();
void clear();

void drawPixel(int x, int y, uint32_t color);
void drawGrid(uint8_t cellSize, uint32_t color);
void drawRectangle(int x, int y, int width, int height, uint32_t color);
void drawLine(int x0, int y0, int x1, int y1, uint32_t color);
void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, const uint32_t color);
void drawTexturedTriangle(
    int x0, int y0, float u0, float v0,
    int x1, int y1, float u1, float v1,
    int x2, int y2, float u2, float v2
);

#endif