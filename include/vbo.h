#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 texCoord;
};

class VBO {
private:
    GLuint id;

public:
    VBO(std::vector<Vertex>& vertices);

    GLuint getId() const;
    
    void bind();
    void unbind();
    void cleanup();
};