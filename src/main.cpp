#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "texture.h"
#include "shader.h"
#include "vao.h"
#include "vbo.h"
#include "ebo.h"

// Define vertices for a square with colors
GLfloat vertices[] = {
    // positions        // colors            // texture coords
    -0.5f, 0.0f,  0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f,
    -0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f,
     0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f,
     0.5f, 0.0f,  0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f,
     0.0f, 0.8f,  0.0f, 0.92f, 0.86f, 0.76f, 2.5f, 5.0f
};

GLuint indices[] = {
    0, 1, 2,
    0, 2, 3,
    0, 1, 4,
    1, 2, 4,
    2, 3, 4,
    3, 0, 4
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
    GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL", NULL, NULL);
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
    vao.linkAttribute(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    vao.linkAttribute(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    vao.linkAttribute(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    vao.unbind(); 
    vbo.unbind();
    ebo.unbind();

    // Load texture
    Texture texture("../textures/grass.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    texture.texUnit(shader, "tex0", 0);

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    glEnable(GL_DEPTH_TEST);

    // Main render loop
    while(!glfwWindowShouldClose(window)) {
        // Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use shader program
        shader.activate();

        // Update camera inputs
        camera.inputs(window);

        // Update camera matrix
        camera.updateMatrix(45.0f, 0.1f, 100.0f);
        camera.matrix(shader, "camMatrix");

        // Bind the texture
        texture.bind();

        // Bind the vertex array object
        vao.bind();

        // Draw 6 indices as triangles
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

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
    texture.cleanup();

    glfwTerminate();
    return 0;
}