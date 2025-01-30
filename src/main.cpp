#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "vao.h"
#include "vbo.h"
#include "ebo.h"

// Define vertices for a triangle centered at (0,0,0) with colors
GLfloat vertices[] = {
    -0.5f, -0.5f * float(sqrt(3)) / 3,  0.0f, 0.8f, 0.3f, 0.02f,
     0.5f, -0.5f * float(sqrt(3)) / 3,  0.0f, 0.8f, 0.3f, 0.02f,
     0.0f,  1.0f * float(sqrt(3)) / 3,  0.0f, 1.0f, 0.6f, 0.32f,
    -0.25f, 0.25f * float(sqrt(3)) / 3, 0.0f, 0.9f, 0.45f, 0.17f,
     0.25f, 0.25f * float(sqrt(3)) / 3, 0.0f, 0.9f, 0.45f, 0.17f,
     0.0f, -0.5f * float(sqrt(3)) / 3,  0.0f, 0.8f, 0.3f, 0.02f
};

GLuint indices[] = {
    0, 3, 5,
    3, 2, 4,
    5, 4, 1
};

// Callback function to handle window resize events
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    // Initialise GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialise GLFW" << std::endl;
        return -1;
    }

    // Configure GLFW window properties
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window and OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to open GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialise GLAD to load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialise GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set initial viewport size and register resize callback
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Create and compile shaders
    Shader shader("../shaders/default.vert", "../shaders/default.frag");

    // Create and bind vertex array object, vertex buffer object and element buffer object
    VAO vao;
    vao.bind();
    VBO vbo(vertices, sizeof(vertices));
    EBO ebo(indices, sizeof(indices));

    // Link VBO attributes to VAO (position and color)
    vao.linkAttribute(vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    vao.linkAttribute(vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    
    vao.unbind(); 
    vbo.unbind();
    ebo.unbind();

    // Main render loop
    while(!glfwWindowShouldClose(window)) {
        // Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Use shader program
        shader.activate();

        // Bind the vertex array object
        vao.bind();

        // Draw 9 indices as triangles
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Clean up
    vao.cleanup();
    vbo.cleanup();
    ebo.cleanup();
    shader.cleanup();

    glfwTerminate();
    return 0;
}