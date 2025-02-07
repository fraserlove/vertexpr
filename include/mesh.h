#pragma once

#include "vao.h"
#include "ebo.h"
#include "camera.h"
#include "texture.h"

class Mesh {
private:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    glm::mat4 transform;

    VAO vao;

public:
    Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures, glm::mat4 transform = glm::mat4(1.0f));

    void draw(Shader& shader, Camera& camera);

    void rotate(glm::quat rotation);
    void translate(glm::vec3 translation);
    void scale(glm::vec3 scale);
};