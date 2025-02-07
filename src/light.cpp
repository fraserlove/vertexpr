#include "light.h"

Light::Light(Shader& shader, glm::vec3 position, glm::vec4 color) : position(position), color(color) {
    glUniform4fv(glGetUniformLocation(shader.getId(), "lightColor"), 1, glm::value_ptr(color));
    glUniform3fv(glGetUniformLocation(shader.getId(), "lightPos"), 1, glm::value_ptr(position));
}

glm::vec3 Light::getPosition() const {
    return position;
}

glm::vec4 Light::getColor() const {
    return color;
} 