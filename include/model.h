#pragma once

#include <json.h>

#include "mesh.h"

class Model {
public:
    Model(const char* path);
    void draw(Shader& shader, Camera& camera);

private:
    const char* path;
    std::vector<unsigned char> data;
    nlohmann::json json;

    std::vector<Mesh> meshes;
    std::vector<glm::vec3> translations;
    std::vector<glm::quat> rotations;
    std::vector<glm::vec3> scales;
    std::vector<glm::mat4> matrices;

    std::vector<std::string> loadedTextureNames;
    std::vector<Texture> loadedTextures;

    void loadMesh(unsigned int index);

    void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

    std::vector<unsigned char> import();
    std::vector<float> getFloats(nlohmann::json accessor);
    std::vector<GLuint> getIndices(nlohmann::json accessor);
    std::vector<Texture> getTextures();

    std::vector<Vertex> assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texCoords);

    std::vector<glm::vec4> getVec4s(std::vector<float> floatVec);
    std::vector<glm::vec3> getVec3s(std::vector<float> floatVec);
    std::vector<glm::vec2> getVec2s(std::vector<float> floatVec);
};