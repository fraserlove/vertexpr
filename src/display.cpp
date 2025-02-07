#include <iostream>

#include "display.h"

Display::Display(int width, int height, const char* title) : width(width), height(height), title(title) {
    // Initialise GLFW
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create GLFW window
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);

    // Initialise GLAD to load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
}

Display::~Display() {
    glfwTerminate();
}

int Display::getWidth() const {
    return width;
}

int Display::getHeight() const {
    return height;
}

GLFWwindow* Display::getWindow() const {
    return window;
}

bool Display::isActive() const {
    return !glfwWindowShouldClose(window);
}

void Display::update() {
    // Swap front and back buffers
    glfwSwapBuffers(window);
    // Poll for and process events
    glfwPollEvents();
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}