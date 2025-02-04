# opengl-expr
This is an OpenGL experiment using `glfw` for window management, `glad` for OpenGL function loading, and `glm` for mathematics and transformations.

## Build Instructions
This project requires `cmake` to build. The project can be built using the following commands:

```sh
mkdir build
cd build
cmake ..
make  # or "cmake --build ."
```

If you are using VSCode, you can use the following arguments in the `tasks.json` file to build the project:
```
"args": [
    "-std=c++17",
    "-g",
    "-I${workspaceFolder}/include",
    "-L${workspaceFolder}/lib",
    "-Wl,-rpath,${workspaceFolder}/lib",
    "${workspaceFolder}/src/*.cpp",
    "${workspaceFolder}/src/glad.c",
    "${workspaceFolder}/lib/libglfw.3.dylib",
    "-o",
    "${workspaceFolder}/bin/opengl"
]
```