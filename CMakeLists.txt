cmake_minimum_required(VERSION 3.10)

project(opengl-expr)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create build directory if it doesn't exist
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/deps)

# Compiled libraries
link_directories(${PROJECT_SOURCE_DIR}/lib)

# Source files
file(GLOB SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp" "${PROJECT_SOURCE_DIR}/deps/glad/glad.c")

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

# Create the executable
add_executable(opengl ${SOURCE_FILES})

# Link the libraries
target_link_libraries(opengl ${PROJECT_SOURCE_DIR}/lib/libglfw.3.dylib)