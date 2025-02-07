#include "camera.h"

Camera::Camera(int width, int height, glm::vec3 position, float fov, float zNear, float zFar) : width(width), height(height), position(position) {
    forward = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    projection = glm::perspective(glm::radians(fov), (float)(width / height), zNear, zFar);
    
    speed = 0.1f;
    sensitivity = 100.0f;
}

glm::mat4 Camera::getProjectionView() const {
    return projectionView;
}

glm::vec3 Camera::getPosition() const {
    return position;
}

void Camera::update() {
    glm::mat4 view = glm::lookAt(position, position + forward, up);
    projectionView = projection * view;
}

void Camera::inputs(GLFWwindow* window) {
    // Keyboard inputs
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) position += speed * forward;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) position -= speed * forward;
    
    glm::vec3 right = glm::normalize(glm::cross(forward, up));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) position -= speed * right;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) position += speed * right;
    
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) position += speed * up;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) position -= speed * up;

    // Sprint
    speed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? 0.5f : 0.1f;

    // Mouse inputs
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {

        // Hide mouse and lock to center of screen while looking
        if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_HIDDEN) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            glfwSetCursorPos(window, width / 2, height / 2);
        }

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float rotX = sensitivity * (float)(mouseY - height / 2) / height;
        float rotY = sensitivity * (float)(mouseX - width / 2) / width;

        // Vertical rotation, lock so that the camera doesn't flip upside down
        glm::vec3 newDirection = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(-rotX), right)) * forward;
        if (abs(glm::dot(newDirection, up)) < 0.99f) forward = newDirection;

        // Horizontal rotation
        forward = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(-rotY), up)) * forward;
        forward = glm::normalize(forward);

        glfwSetCursorPos(window, width / 2, height / 2);
    }
    else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}