#pragma once

#include <glad/glad.h>
#include "shader.h"

class Texture {
public:
    GLuint ID;
    const char* type;
    GLuint unit;
    Texture(const char* image, const char* type, GLuint slot);
    void texUnit(Shader& shader, const char* uniform, GLuint unit);
    void bind();
    void unbind();
    void cleanup();
};