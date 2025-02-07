#include "shader.h"
#include "utils.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode = Utils::readFile(vertexPath);
    std::string fragmentCode = Utils::readFile(fragmentPath);

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    activate();
}

GLuint Shader::getId() const {
    return id;
}

void Shader::activate() {
    glUseProgram(id);
}

void Shader::cleanup() {
    glDeleteProgram(id);
}