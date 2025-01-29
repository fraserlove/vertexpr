# opengl

### Build Configuration
The project uses VSCode for building. The build configuration in `tasks.json` should include the following arguments:
```
"args": [
    "-std=c++17",
    "-fdiagnostics-color=always",
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