#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

class Display {
private:
    GLFWwindow* window;
    
    int width;
    int height;
    const char* title;

public:
    Display(int width, int height, const char* title);
    ~Display();

    int getWidth() const;
    int getHeight() const;
    GLFWwindow* getWindow() const;

    bool isActive() const;
    void update();
}; 