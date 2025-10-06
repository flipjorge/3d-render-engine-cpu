#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "array/array.h"
#include "display.h"
#include "vector.h"
#include "projection.h"
#include "cube.h"
#include "face.h"
#include "matrix.h"
#include "light.h"
#include "texture.h"
#include "camera.h"
#include "clipping.h"

#define TARGET_FRAME_RATE 60
#define TARGET_FRAME_TIME (1000 / TARGET_FRAME_RATE)

#define FOV M_PI / 3
#define Z_NEAR 0.01
#define Z_FAR 100

#define MAX_TRIANGLES 10000

bool isRunning = false;

triangle_t trianglesToRender[MAX_TRIANGLES];
int numberTrianglesToRender = 0;

mesh_t* cube;
mesh_t* piramid;

matrix4_t projectionMatrix;

Uint32 previousFrameTicks;

light_t light;

uint32_t* texture = NULL;
upng_t* png = NULL;

camera_t camera;

plane_t frustumPlanes[FRUSTUM_NUM_PLANES];

// Sets up the initial state of the scene.
// This function is called once at the start of the application. It handles:
// - Loading assets like 3D models (.obj) and textures (.png).
// - Setting up the projection matrix based on window dimensions and field of view.
// - Initializing the clipping planes of the view frustum.
// - Configuring default rendering modes, lighting, and camera position.
void setupScene()
{
    cube = loadMesh("./assets/cube.obj");
    piramid = loadMesh("./assets/piramid.obj");

    float aspectY = (float)getWindowHeight() / (float)getWindowWidth();
    float aspectX = (float)getWindowWidth() / (float)getWindowHeight();
    float fovY = FOV;
    float fovX = atan(tan(fovY / 2) * aspectX) * 2;
      
    initFrustumPlane(frustumPlanes, fovX, fovY, Z_NEAR, Z_FAR);
    
    projectionMatrix = matrix4MakePerspective( FOV, aspectY, Z_NEAR, Z_FAR );
    
    previousFrameTicks = SDL_GetTicks();

    setRenderMode(RENDER_MODE_TEXTURED);
    setCullingMode(CULLING_MODE_BACK);

    light = (light_t){
        (vector3_t){ 0, 0, 1 }
    };

    loadTextureFromPng("./assets/cube.png", &png, &texture);

    camera = (camera_t){
        .position = { 0, 0, 0 },
        .direction = { 0, 0, 1 }
    };
}

// Frees all allocated resources before the application closes.
// This function is called once upon exiting to prevent memory leaks.
void clearScene() {
    upng_free(png);
    freeAllMeshes();
}

// Handles all user input for the current frame.
// It polls for SDL events and updates application state accordingly. This includes:
// - Closing the window.
// - Toggling rendering and culling modes.
// - Moving the camera based on keyboard input.
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
        if(event.key.keysym.sym == SDLK_1)
        {
            setRenderMode(RENDER_MODE_VERTEX);
        }
        if(event.key.keysym.sym == SDLK_2)
        {
            setRenderMode(RENDER_MODE_VERTEX_WIREFRAME);
        }
        if(event.key.keysym.sym == SDLK_3)
        {
            setRenderMode(RENDER_MODE_FILL_TRIANGLE);
        }
        if(event.key.keysym.sym == SDLK_4)
        {
            setRenderMode(RENDER_MODE_FILL_TRIANGLE_WIREFRAME);
        }
        if(event.key.keysym.sym == SDLK_5)
        {
            setRenderMode(RENDER_MODE_TEXTURED);
        }
        if(event.key.keysym.sym == SDLK_6)
        {
            setRenderMode(RENDER_MODE_TEXTURED_WIREFRAME);
        }
        if(event.key.keysym.sym == SDLK_c)
        {
            setCullingNextMode();
        }
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            isRunning = false;
        }
        if (event.key.keysym.sym == SDLK_w)
        {
            camera.position = vector3Sum(camera.position, (vector3_t){ 0, 0, 1 });
        }
        if (event.key.keysym.sym == SDLK_s)
        {
            camera.position = vector3Sum(camera.position, (vector3_t){ 0, 0, -1 });
        }
        if (event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_LEFT)
        {
            camera.position = vector3Sum(camera.position, (vector3_t){ -1, 0, 0 });
        }
        if (event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_RIGHT)
        {
            camera.position = vector3Sum(camera.position, (vector3_t){ 1, 0, 0 });
        }
        if (event.key.keysym.sym == SDLK_UP)
        {
            camera.position = vector3Sum(camera.position, (vector3_t){ 0, 1, 0 });
        }
        if (event.key.keysym.sym == SDLK_DOWN)
        {
            camera.position = vector3Sum(camera.position, (vector3_t){ 0, -1, 0 });
        }
        break;
    }
}

// This is the core of the rendering pipeline, executed once per frame.
// It processes all game objects from 3D space to 2D screen space triangles.
void update()
{
    // --- 1. Frame Timing ---
    // Caps the frame rate to the target value.
    int frameTime = SDL_GetTicks() - previousFrameTicks;
    
    if(frameTime < TARGET_FRAME_TIME)
    {
        SDL_Delay(TARGET_FRAME_TIME - frameTime);
        frameTime = SDL_GetTicks() - previousFrameTicks;
    }

    float frameTimeSeconds = frameTime / 1000.0f;
    previousFrameTicks = SDL_GetTicks();

    // --- 2. Object & Camera Updates ---
    // Updates object transformations (position, rotation, scale).
    // Creates the view matrix based on the camera's current position and orientation.
    float rotationIncrement = 1 * frameTimeSeconds;

    cube->position = (vector3_t){ 0, 0, 30 };
    // cube.rotation = vector3Sum( cube.rotation, (vector3_t){ rotationIncrement, rotationIncrement, rotationIncrement } );
    cube->scale = (vector3_t){ 2, 2, 2};

    piramid->position = (vector3_t){ 0, 10, 30 };
    // piramid.rotation = vector3Sum( piramid.rotation, (vector3_t){ 0, rotationIncrement, 0 } );
    piramid->scale = (vector3_t){ 2, 2, 2 };

    vector3_t eye = camera.position;
    vector3_t target = { camera.position.x, camera.position.y, camera.position.z + 1 };
    vector3_t up = { 0, 1, 0 };
    matrix4_t viewMatrix = matrix4LookAt(&eye, &target, &up);

    const int numMeshes = getNumberMeshes();
    numberTrianglesToRender = 0;

    // --- 3. Geometry Processing Loop (per-mesh, per-face) ---
    // This loop iterates through every triangle of every mesh in the scene.
    for (size_t m = 0; m < numMeshes; m++)
    {
        mesh_t* mesh = getMesh(m);
        matrix4_t transformMatrix = getMeshTransformMatrix(mesh);

        const int numFaces = array_length(mesh->faces);

        for (size_t f = 0; f < numFaces; f++)
        {
            face_t face = mesh->faces[f];
            vector3_t faceVertices[3];
            
            faceVertices[0] = mesh->vertices[face.a - 1];
            faceVertices[1] = mesh->vertices[face.b - 1];
            faceVertices[2] = mesh->vertices[face.c - 1];
            
            // --- 3a. Model and View Transformation ---
            // Transforms vertices from model space -> world space -> camera space.
            vector4_t transformedVertices[3];

            for (size_t v = 0; v < 3; v++)
            {
                vector4_t transformedVertice = vector3to4(faceVertices[v]);
                transformedVertice = matrix4MultiplyVector4(&transformMatrix, &transformedVertice);
                transformedVertice = matrix4MultiplyVector4(&viewMatrix, &transformedVertice);
                transformedVertices[v] = transformedVertice;
            }

            // --- 3b. Back-face Culling ---
            // Checks if the triangle is facing away from the camera and discards it if so.
            vector3_t verticesForBackCulling[3] = {
                vector4to3(transformedVertices[0]),
                vector4to3(transformedVertices[1]),
                vector4to3(transformedVertices[2])
            };

            if(getCullingMode() == CULLING_MODE_NONE && !isFaceFacingCamera(camera.position, verticesForBackCulling)) continue;

            // --- 3c. Clipping ---
            // Clips the triangle against the 6 planes of the view frustum. This may result
            // in the triangle being discarded or converted into multiple new triangles.
            polygon_t polygon = createPolygonFromTriangle(
                vector4to3(transformedVertices[0]),
                vector4to3(transformedVertices[1]),
                vector4to3(transformedVertices[2]),
                face.aUV,
                face.bUV,
                face.cUV
            );
            
            clipPolygon(&polygon, frustumPlanes);

            triangle_t trianglesAfterClipping[MAX_NUM_POLY_TRIANGLES];
            int numberTrianglesAfterClipping = 0;

            trianglesFromPolygon(&polygon, trianglesAfterClipping, &numberTrianglesAfterClipping);

            // --- 3d. Projection & Screen Mapping ---
            // For each triangle that survived clipping, this block projects it to the screen.
            for (int t = 0; t < numberTrianglesAfterClipping; t++) {
                triangle_t triangleAfterClipping = trianglesAfterClipping[t];

                triangle_t triangle;

                for (size_t v = 0; v < 3; v++)
                {
                    // Applies projection matrix and performs viewport transformation to screen coordinates.
                    vector4_t projectedVertex = matrix4MultiplyVector4Project(&projectionMatrix, &triangleAfterClipping.points[v]);
                    
                    projectedVertex.x *= getWindowWidth() / 2.0;
                    projectedVertex.y *= getWindowHeight() / 2.0;

                    projectedVertex.y *= -1;
                    
                    projectedVertex.x += getWindowWidth() / 2.0;
                    projectedVertex.y += getWindowHeight() / 2.0;

                    triangle.points[v].x = projectedVertex.x;
                    triangle.points[v].y = projectedVertex.y;
                    triangle.points[v].z = projectedVertex.z;
                    triangle.points[v].w = projectedVertex.w;
                }

                // --- 3e. Lighting & Final Assembly ---
                // Calculates the triangle's color based on light intensity and assembles the
                // final triangle data to be sent to the rasterizer.
                vector3_t verticesForIntensityFactor[3] = {
                    vector4to3(transformedVertices[0]),
                    vector4to3(transformedVertices[1]),
                    vector4to3(transformedVertices[2])
                };
                
                const float intensityFactor = lightIntensityFactor(light.direction, verticesForIntensityFactor);
                triangle.color = lightApplyIntensity(0xFFFFFFFF, intensityFactor);

                triangle.textureCoordinates[0] = triangleAfterClipping.textureCoordinates[0];
                triangle.textureCoordinates[1] = triangleAfterClipping.textureCoordinates[1];
                triangle.textureCoordinates[2] = triangleAfterClipping.textureCoordinates[2];

                if(numberTrianglesToRender > MAX_TRIANGLES) break;
                
                trianglesToRender[numberTrianglesToRender] = triangle;
                numberTrianglesToRender++;
            }
        }
    }
}

// Renders the final 2D triangles to the screen.
// This function is called after the update loop has processed all geometry.
void render()
{
    // --- 1. Clear Buffers ---
    // Resets the color and depth buffers for the new frame.
    clearColorBuffer(0x000000FF);
    clearDepthBuffer();

    drawGrid(40, 0x333333FF);

    // --- 2. Rasterization Loop ---
    // Iterates through the array of screen-space triangles and draws them based on the
    // current rendering mode (e.g., wireframe, filled, textured).
    for (size_t i = 0; i < numberTrianglesToRender; i++)
    {
        triangle_t triangle = trianglesToRender[i];

        if(shouldRenderVertex())
        {
            for (size_t j = 0; j < 3; j++)
            {
                vector4_t vertex = triangle.points[j];
                drawRectangle(vertex.x - 2, vertex.y - 2, 4, 4, 0xFF0000FF);
            }
        }

        if(shouldRenderFillTriangles())
        {
            drawFilledTriangle(
                triangle.points[0].x,
                triangle.points[0].y,
                triangle.points[0].z,
                triangle.points[0].w,
                triangle.points[1].x,
                triangle.points[1].y,
                triangle.points[1].z,
                triangle.points[1].w,
                triangle.points[2].x,
                triangle.points[2].y,
                triangle.points[2].z,
                triangle.points[2].w,
                triangle.color
            );
        }

        if(shouldRenderTextures())
        {
            drawTexturedTriangle(
                triangle.points[0].x,
                triangle.points[0].y,
                triangle.points[0].z,
                triangle.points[0].w,
                triangle.textureCoordinates[0].u,
                triangle.textureCoordinates[0].v,
                triangle.points[1].x,
                triangle.points[1].y,
                triangle.points[1].z,
                triangle.points[1].w,
                triangle.textureCoordinates[1].u,
                triangle.textureCoordinates[1].v,
                triangle.points[2].x,
                triangle.points[2].y,
                triangle.points[2].z,
                triangle.points[2].w,
                triangle.textureCoordinates[2].u,
                triangle.textureCoordinates[2].v,
                texture
            );
        }

        if(shouldRenderWireframe())
        {
            drawTriangle(
                triangle.points[0].x,
                triangle.points[0].y,
                triangle.points[1].x,
                triangle.points[1].y,
                triangle.points[2].x,
                triangle.points[2].y,
                0xFFFFFF00
            );
        }
    }
    
    // --- 3. Present Frame ---
    // Copies the software color buffer to the screen, making the new frame visible.
    renderColorBuffer();
}


// The main entry point of the application.
// It contains the main game loop that drives the entire program.
int main()
{
    initializeWindow(&isRunning); 
    setupScene();

    while (isRunning)
    {
        processInput();
        update();
        render();
    }

    clearScene();
    destroyWindow();

    return 0;
}