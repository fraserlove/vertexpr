#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

class Light {
private:
    glm::vec3 position;
    glm::vec4 color;

public:
    Light(Shader& shader, glm::vec3 position, glm::vec4 color);

    glm::vec3 getPosition() const;
    glm::vec4 getColor() const;
};