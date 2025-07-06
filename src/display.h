#ifndef DISPLAY
#define DISPLAY

#include <SDL2/SDL.h>

enum RenderMode
{
    RENDER_MODE_VERTEX,
    RENDER_MODE_VERTEX_WIREFRAME,
    RENDER_MODE_FILL_TRIANGLE,
    RENDER_MODE_FILL_TRIANGLE_WIREFRAME,
    RENDER_MODE_TEXTURED,
    RENDER_MODE_TEXTURED_WIREFRAME
};

enum CullingMode
{
    CULLING_MODE_NONE,
    CULLING_MODE_BACK
};

void initializeWindow(bool* isRunning);
void destroyWindow();

int getWindowWidth();
int getWindowHeight();

void clearColorBuffer(uint32_t color);
void renderColorBuffer();

void clearDepthBuffer();

int getRenderMode();
void setRenderMode(int mode);

int getCullingMode();
void setCullingMode(int mode);
void setCullingNextMode();

bool shouldRenderVertex();
bool shouldRenderWireframe();
bool shouldRenderFillTriangles();
bool shouldRenderTextures();

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