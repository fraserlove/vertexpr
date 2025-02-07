#pragma once

#include <json.h>
#include <map>

#include "mesh.h"

class Model {
private:
    const char* path;
    std::vector<unsigned char> data;
    nlohmann::json json;

    std::vector<Mesh> meshes;

    // Keep track of loaded textures to avoid loading the same texture multiple times
    std::vector<Texture> loadedTextures;

public:
    Model(const char* path);

    void draw(Shader& shader, Camera& camera);

    void rotate(glm::quat rotation);
    void translate(glm::vec3 translation);
    void scale(glm::vec3 scale);

private:
    void loadMesh(unsigned int index, glm::mat4 transform);

    void traverseNode(unsigned int nextNode, glm::mat4 transform = glm::mat4(1.0f));

    void import(const char* path);
    std::vector<float> getFloats(nlohmann::json accessor);
    std::vector<GLuint> getIndices(nlohmann::json accessor);
    std::vector<Texture> getTextures();

    std::vector<Vertex> assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texCoords);

    std::vector<glm::vec4> getVec4s(std::vector<float> floatVec);
    std::vector<glm::vec3> getVec3s(std::vector<float> floatVec);
    std::vector<glm::vec2> getVec2s(std::vector<float> floatVec);
};