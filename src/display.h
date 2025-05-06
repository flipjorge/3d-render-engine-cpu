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

#endif