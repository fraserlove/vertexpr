#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
private:
    glm::mat4 projectionView;
    glm::mat4 projection;
    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 up;

    int width;
    int height;
    
    float speed;
    float sensitivity;

public:
    Camera(int width, int height, glm::vec3 position, float fov, float zNear, float zFar);

    glm::mat4 getProjectionView() const;
    glm::vec3 getPosition() const;

    void update();
    void inputs(GLFWwindow* window);
};