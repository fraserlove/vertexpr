#pragma once

#include <glad/glad.h>
#include "vbo.h"

class VAO {
public:
    GLuint ID;
    VAO();
    void linkAttribute(VBO& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
    void bind();
    void unbind();
    void cleanup();
};