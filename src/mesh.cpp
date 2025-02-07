#include "mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures, glm::mat4 transform) : vertices(vertices), indices(indices), textures(textures), transform(transform) {
    vao.bind();
    VBO vbo(vertices);
    EBO ebo(indices);

    // Position
    vao.linkAttribute(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    // Normal
    vao.linkAttribute(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
    // Color
    vao.linkAttribute(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
    // Texture coordinates
    vao.linkAttribute(vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

    vao.unbind();
    vbo.unbind();
    ebo.unbind();
}

void Mesh::draw(Shader& shader, Camera& camera) {
    shader.activate();
    vao.bind();

    // Bind textures and set uniforms
    std::unordered_map<std::string, unsigned int> textureCount;
    for (unsigned int i = 0; i < textures.size(); i++) {
        const std::string& type = textures[i].getType();
        const std::string number = std::to_string(textureCount[type]++);
        textures[i].texUnit(shader, (type + number).c_str(), i);
        textures[i].bind();
    }

    glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "cameraTransform"), 1, GL_FALSE, glm::value_ptr(camera.getProjectionView()));
    glUniform3fv(glGetUniformLocation(shader.getId(), "cameraPos"), 1, glm::value_ptr(camera.getPosition()));
    glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "transform"), 1, GL_FALSE, glm::value_ptr(transform));

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::rotate(glm::quat rotation) {
    transform = transform * glm::mat4_cast(rotation);
}

void Mesh::translate(glm::vec3 translation) {
    transform = transform * glm::translate(glm::mat4(1.0f), translation);
}

void Mesh::scale(glm::vec3 scale) {
    transform = transform * glm::scale(glm::mat4(1.0f), scale);
}