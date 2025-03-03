cmake_minimum_required(VERSION 3.10)
project(verus)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/deps)

link_directories(${PROJECT_SOURCE_DIR}/lib)

file(GLOB SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp" "${PROJECT_SOURCE_DIR}/deps/glad/glad.c")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

add_executable(verus ${SOURCE_FILES})

target_link_libraries(verus ${PROJECT_SOURCE_DIR}/lib/libglfw.3.dylib)
