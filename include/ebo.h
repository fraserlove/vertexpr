#pragma once

#include <glad/glad.h>
#include <vector>

class EBO {
private:
    GLuint id;

public:
    EBO(std::vector<GLuint>& indices);

    GLuint getId() const;
    
    void bind();
    void unbind();
    void cleanup();
};