#pragma once

#include <glad/glad.h>

#include "shader.h"

class Texture {
private:
    GLuint id;
    const char* image;
    const char* type;
    GLuint unit;

public:
    Texture(const char* image, const char* type, GLuint unit);

    GLuint getId() const;
    const char* getType() const;
    const char* getImage() const;

    void texUnit(Shader& shader, const char* uniform, GLuint unit);
    void bind();
    void unbind();
    void cleanup();
};