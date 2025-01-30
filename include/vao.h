#pragma once

#include <glad/glad.h>
#include "vbo.h"

class VAO {
public:
    GLuint ID;
    VAO();
    void linkVBO(VBO vbo, GLuint layout);
    void bind();
    void unbind();
    void cleanup();
};