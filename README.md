# CPU based 3d rendering engine

This project was created as a learning exercise in 3D graphics rendering. It is a CPU/software-based renderer, developed by following the [Pikuma 3D Computer Graphics Programming course](https://pikuma.com/courses/learn-3d-computer-graphics-programming).

## How to run the engine example

```
make run
```

## Rendering pipeline structure

The engine processes and renders 3D objects in a series of steps, executed for every frame. This sequence is known as the rendering pipeline. Below is an overview of the pipeline as implemented in this project.

### 1. Setup
Before the main loop begins, the scene is prepared:
- **Asset Loading**: 3D models (`.obj` files) and textures (`.png` files) are loaded into memory.
- **Matrix Setup**: The `projectionMatrix` is created based on the desired field of view (FOV) and screen aspect ratio.
- **Camera & Light**: The camera's initial position and the scene's light source direction are defined.

### 2. The `update()` Loop (Geometry Stage)
This is the core of the pipeline, where 3D data is processed. It runs for every triangle of every mesh in the scene.

- **Object & Camera Transformation**:
  - The object's `worldMatrix` (combining its scale, rotation, and translation) is computed.
  - The camera's `viewMatrix` is computed based on its position and target direction.

- **Vertex Transformation**: Each vertex of a triangle is transformed from its local model space into camera space.
  - **Model Space → World Space**: Vertex is multiplied by the `worldMatrix`.
  - **World Space → Camera Space**: Vertex is then multiplied by the `viewMatrix`.

- **Back-face Culling**: Triangles that are facing away from the camera are discarded. This is an optimization that prevents the renderer from processing geometry that wouldn't be visible anyway. It works by checking the dot product of the triangle's normal and a vector to the camera.

- **Clipping**: Triangles that are partially or fully outside the camera's view volume (the "frustum") are clipped.
  - The triangle is converted to a polygon.
  - The polygon is clipped against the 6 planes of the frustum using the Sutherland-Hodgman algorithm.
  - The resulting polygon is triangulated back into one or more renderable triangles.

- **Projection Transformation**: The vertices of the clipped triangles are projected from 3D camera space into 2D screen space.
  - **Projection**: Vertices are multiplied by the `projectionMatrix`.
  - **Perspective Division**: The `x`, `y`, and `z` components are divided by the `w` component. This crucial step creates the illusion of depth, making distant objects appear smaller.
  - **Viewport Transformation**: The coordinates, which are now in a normalized range [-1, 1], are mapped to the actual pixel coordinates of the window.

- **Lighting**: The color of the triangle is calculated based on its orientation relative to the scene's light source (flat shading).

### 3. The `render()` Loop (Rasterization Stage)
After the `update()` function has produced a list of 2D triangles ready to be drawn, the `render()` function takes over.

- **Clear Buffers**: The `colorBuffer` and `depthBuffer` are cleared at the start of the frame.

- **Triangle Rasterization**: Each triangle is "drawn" into the color buffer, pixel by pixel.
  - The renderer iterates over every pixel that the triangle covers.
  - **Depth Testing (Z-buffering)**: For each pixel, its depth is compared to the value already in the `depthBuffer`. The pixel is only drawn if it is closer to the camera than what was previously drawn at that location.
  - **Attribute Interpolation**: For textured triangles, the UV coordinates are interpolated across the surface of the triangle for each pixel. This interpolation is "perspective-correct" (using the `w` component) to prevent texture distortion.
  - **Texture Sampling**: The final color for a pixel is sampled from the texture using the interpolated UV coordinates.

- **Present Frame**: The final image in the `colorBuffer` is copied to the screen to be displayed.

