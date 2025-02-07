# opengl-expr
This is an OpenGL experiment using `glfw` for window management, `glad` for OpenGL function loading, and `glm` for mathematics and transformations.

## Build Instructions
This project requires `cmake` to build and can be built using the following command:

```sh
cmake . && make
```
Run the executable using `./build/opengl`.

## Loading Models
Currently, the project only supports loading `.gltf` models with support for diffuse and specular textures.

## Camera
Keyboard Controls:
  - `W`/`A`/`S`/`D`: Move forward/left/backward/right
  - `Space`/`Ctrl`: Move up/down
  - `Left Shift`: Sprint (increased movement speed)

Mouse Controls:
  - Left click and drag: Look around
  - Mouse is hidden and locked to screen center while looking