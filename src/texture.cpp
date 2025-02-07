#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const char* image, const char* type, GLuint unit) : image(image), type(type), unit(unit) {
    int width, height, nChannels;
    unsigned char* data = stbi_load(image, &width, &height, &nChannels, 0);
    
    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set format based on number of channels
    GLenum format;
    switch (nChannels) {
        case 4: format = GL_RGBA; break;
        case 3: format = GL_RGB; break;
        case 1: format = GL_RED; break;
        default: throw std::invalid_argument("Unsupported number of channels in texture");
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::getId() const {
    return id;
}

const char* Texture::getType() const {
    return type;
}

const char* Texture::getImage() const {
    return image;
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit) {
    GLuint texUniform = glGetUniformLocation(shader.getId(), uniform);
    shader.activate();
    glUniform1i(texUniform, unit);
}

void Texture::bind() {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::cleanup() {
    glDeleteTextures(1, &id);
}