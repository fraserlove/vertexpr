#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

std::string readShaderFile(const char* filePath);

class Shader {
private:
    GLuint id;

public:
    Shader(const char* vertexPath, const char* fragmentPath);

    GLuint getId() const;

    void activate();
    void cleanup();
};