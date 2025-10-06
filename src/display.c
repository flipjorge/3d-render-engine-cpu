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

static int renderMode;
static int cullingMode;

// Initializes the SDL window, renderer, and software buffers.
// This is the entry point for the display system, setting up the main window where all
// rendering will be presented. It also allocates memory for the color and depth buffers,
// which are the core components of the software rasterizer.
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

// Frees all allocated resources and shuts down SDL.
// This is the cleanup function, called at the end of the program to release memory used
// by the color and depth buffers and to properly close the SDL window and renderer,
// preventing memory leaks.
void destroyWindow() {
    free(colorBuffer);
    free(depthBuffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Returns the width of the main application window.
// A utility function to provide the window's width to other parts of the engine,
// such as the projection matrix calculation and viewport transformation.
int getWindowWidth()
{
    return windowWidth;
}

// Returns the height of the main application window.
// A utility function to provide the window's height to other parts of the engine,
// such as the projection matrix calculation and viewport transformation.
int getWindowHeight()
{
    return windowHeight;
}

// Clears the color buffer to a specified color.
// This is called at the beginning of each frame's render loop to reset the canvas
// to a solid background color before any new geometry is drawn.
void clearColorBuffer(uint32_t color)
{
    for (size_t i = 0; i < windowWidth * windowHeight; i++)
    {
        colorBuffer[i] = color;
    }
}

// Copies the contents of the software color buffer to the screen.
// After all drawing for a frame is complete, this function uses SDL to update the
// screen with the final image stored in the `colorBuffer`, making the rendered frame visible.
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

// Resets the depth buffer to its maximum value.
// The depth buffer is crucial for correct 3D occlusion (Z-buffering). This function is
// called at the start of each frame to reset all depth values, ensuring that the first
// pixel drawn at any location is considered the closest until a closer one is found.
void clearDepthBuffer()
{
    for (size_t i = 0; i < windowWidth * windowHeight; i++)
    {
        depthBuffer[i] = 1.0;
    }
}

// Returns the current rendering mode.
// Used by the main render loop to decide which drawing functions to call (e.g., wireframe, filled, textured).
int getRenderMode()
{
    return renderMode;
}

// Sets the current rendering mode.
// Allows the user to switch between different visualization styles at runtime.
void setRenderMode(int mode)
{
    renderMode = mode;
}

// Checks if vertex rendering is enabled.
// A helper function for the main render loop.
bool shouldRenderVertex()
{
    return renderMode == RENDER_MODE_VERTEX
        || renderMode == RENDER_MODE_VERTEX_WIREFRAME;
}

// Checks if wireframe rendering is enabled.
// A helper function for the main render loop.
bool shouldRenderWireframe()
{
    return renderMode == RENDER_MODE_VERTEX_WIREFRAME
        || renderMode == RENDER_MODE_FILL_TRIANGLE_WIREFRAME
        || renderMode == RENDER_MODE_TEXTURED_WIREFRAME;
}

// Checks if filled triangle rendering is enabled.
// A helper function for the main render loop.
bool shouldRenderFillTriangles()
{
    return renderMode == RENDER_MODE_FILL_TRIANGLE
        || renderMode == RENDER_MODE_FILL_TRIANGLE_WIREFRAME;
}

// Checks if textured triangle rendering is enabled.
// A helper function for the main render loop.
bool shouldRenderTextures()
{
    return renderMode == RENDER_MODE_TEXTURED
        || renderMode == RENDER_MODE_TEXTURED_WIREFRAME;
}

// Returns the current culling mode.
// Used by the main update loop to decide whether to perform back-face culling.
int getCullingMode()
{
    return cullingMode;
}

// Sets the current culling mode.
// Allows the user to enable or disable back-face culling.
void setCullingMode(int mode)
{
    cullingMode = mode;
}

// Toggles the culling mode between enabled and disabled.
// A convenience function for user input to cycle through culling options.
void setCullingNextMode()
{
    cullingMode = (cullingMode + 1) % 2;
}

// Draws a single pixel to the color buffer at a specified screen coordinate.
// This is the most fundamental drawing primitive. All other drawing functions
// (lines, triangles, etc.) are built on top of this.
//
// The color buffer is a 1D array, so the 2D coordinates (x, y) must be converted
// to a 1D index. The formula used is: index = (y * window_width) + x.
void drawPixel(int x, int y, uint32_t color)
{
    if(x > windowWidth || y > windowHeight) return;
    
    colorBuffer[y * windowWidth + x] = color;
}

// Draws a grid pattern onto the color buffer.
// A visual aid used for debugging and to give a sense of the ground plane in the scene.
// It uses the modulo operator (%) to draw lines at regular intervals. A pixel is
// drawn if its x or y coordinate is a multiple of `cellSize`.
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

// Draws a filled rectangle to the color buffer.
// A basic 2D drawing primitive, used in this project to draw the small squares
// that represent vertices when in vertex-rendering mode. It iterates through every
// pixel from the starting (x, y) to (x + width, y + height) and calls `drawPixel`.
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

// Draws a line between two points using the Digital Differential Analyzer (DDA) algorithm.
// This is a fundamental rasterization algorithm used to draw the edges of wireframe triangles.
//
// Math:
// 1. Calculate the difference in x and y: deltaX = x1 - x0, deltaY = y1 - y0.
// 2. Determine the number of steps needed by finding the dimension with the greater change:
//    longestSideLength = max(|deltaX|, |deltaY|). This ensures the line is continuous.
// 3. Calculate the increment for x and y for each step:
//    xIncrement = deltaX / longestSideLength
//    yIncrement = deltaY / longestSideLength
//    One of these will be +/-1, and the other will be a value <= 1.
// 4. Loop `longestSideLength` times, starting from (x0, y0), adding the increments at each
//    step, and drawing a pixel at the rounded (currentX, currentY) position.
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

// Draws the outline of a triangle by calling `drawLine` for each of its three edges.
// This is used for rendering wireframe modes.
void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color);
}

// A utility function to swap the values of two integers.
// Used internally by the triangle rasterization functions to sort vertices by their y-coordinate.
void intSwap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// A utility function to swap the values of two floats.
// Used internally by the textured triangle rasterization function to sort vertex attributes.
void floatSwap(float* a, float* b)
{
    float temp = *a;
    *a = *b;
    *b = temp;
}

// Calculates the barycentric weights (alpha, beta, gamma) for a point 'p' inside a triangle (a, b, c).
// These weights are essential for interpolating attributes (like depth and texture coordinates)
// across the surface of a triangle during rasterization.
//
// Math:
// Any point 'p' inside a triangle can be expressed as a weighted average of its vertices:
// p = α*a + β*b + γ*c, where α+β+γ=1.
// The weights (α, β, γ) are the barycentric coordinates. They represent the ratio of the areas
// of the sub-triangles formed by 'p' to the area of the main triangle 'abc'.
//   α = Area(pbc) / Area(abc)
//   β = Area(apc) / Area(abc)
//   γ = Area(abp) / Area(abc)
// The code calculates these areas using a 2D cross-product-like formula. For two vectors
// v1=(x1, y1) and v2=(x2, y2), the value (x1*y2 - x2*y1) is twice the signed area of the
// triangle they form with the origin.
// `areaParallelogramABC` is twice the area of triangle 'abc'.
// `alpha` is computed using the area of sub-triangle 'pbc'.
// `beta` is computed using the area of sub-triangle 'apc'.
// `gamma` is then found using `1 - alpha - beta`.
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

// Draws a single pixel of a filled triangle, performing a depth test.
// For each pixel inside a triangle, this function interpolates the depth (from the w-component)
// and compares it with the value in the depth buffer. The pixel is only drawn if it is
// closer to the camera than what is already in the buffer.
//
// Math:
// 1. Barycentric weights (α, β, γ) are calculated for the pixel's center.
// 2. For perspective-correct interpolation, we interpolate the reciprocal of the w-component:
//    1/w_interpolated = α * (1/w_a) + β * (1/w_b) + γ * (1/w_c)
//    This `w_interpolated` value is proportional to the true z-depth of the pixel in camera space.
// 3. The depth buffer stores values from 0 (near) to 1 (far). The interpolated value is
//    compared against the buffer. If it's smaller (closer), the pixel is drawn, and the
//    depth buffer is updated with the new, closer depth value.
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

// Draws a single pixel of a textured triangle, performing depth testing and perspective-correct interpolation.
// This function calculates the correct texture coordinate (UV) for a pixel using barycentric
// interpolation that is corrected for perspective distortion by using the w-component of the vertices.
// It then performs a depth test before sampling the texture and drawing the final pixel.
//
// Math:
// 1. Barycentric weights (α, β, γ) are calculated for the pixel's center.
// 2. To find the correct texture coordinates (u, v) for the pixel, we must perform
//    perspective-correct interpolation. Simple linear interpolation of UVs would cause
//    visual artifacts (warping). Instead, we interpolate u/w and v/w:
//    u_persp = α*(u_a/w_a) + β*(u_b/w_b) + γ*(u_c/w_c)
//    v_persp = α*(v_a/w_a) + β*(v_b/w_b) + γ*(v_c/w_c)
//    1/w_persp = α*(1/w_a) + β*(1/w_b) + γ*(1/w_c)
// 3. The final UV coordinates are found by dividing by the interpolated 1/w:
//    u_final = u_persp / (1/w_persp)
//    v_final = v_persp / (1/w_persp)
// 4. A depth test is performed using `w_persp` before the texel is sampled and drawn.
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

// Renders a flat-shaded, filled triangle using a scanline algorithm.
// The function first sorts the triangle's vertices by their y-coordinate. It then splits the
// triangle into two parts (top and bottom flat) and iterates through the scanlines (rows of pixels)
// for each part, drawing horizontal lines of pixels. Depth is checked for each pixel using `drawTrianglePixel`.
//
// Math:
// 1. The vertices are sorted vertically (y0 <= y1 <= y2). This simplifies rasterization.
// 2. The triangle is split horizontally at vertex y1, creating a "flat-bottom" triangle
//    (p0, p1, and an intermediate point on the p0-p2 edge) and a "flat-top" triangle
//    (p1, p2, and the same intermediate point).
// 3. For each part, the algorithm iterates from the top y to the bottom y (scanline by scanline).
// 4. For each scanline `y`, the start (x_start) and end (x_end) x-coordinates of the
//    triangle's intersection with that scanline are calculated using linear interpolation
//    along the triangle's edges. The inverse slopes of the edges are used for efficiency:
//    x = x_start + (y - y_start) * (dx/dy).
// 5. A horizontal line is drawn from x_start to x_end. For each pixel in this line,
//    `drawTrianglePixel` is called to perform a depth test.
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

// Renders a textured triangle using a scanline algorithm with perspective-correct texturing.
// Similar to `drawFilledTriangle`, it sorts vertices and uses a scanline approach. However, for
// each pixel, it calls `drawTexel`, which performs perspective-correct interpolation of UV
// coordinates before sampling the texture and drawing the pixel, ensuring the texture does not
// appear warped on surfaces angled away from the camera.
//
// Math:
// The overall approach is the same as `drawFilledTriangle` (vertex sorting, splitting into
// two sub-triangles, iterating scanlines). The key difference is that instead of drawing
// a solid color, it calls `drawTexel` for each pixel along the horizontal scanline.
// `drawTexel` handles the complex mathematics of perspective-correct texture coordinate
// interpolation and depth testing, as described in its own documentation block.
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