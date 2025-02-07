#include "model.h"
#include "utils.h"

Model::Model(const char* path) : path(path) {
    import(path);
    traverseNode(0);
}

void Model::draw(Shader& shader, Camera& camera) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].draw(shader, camera);
    }
}

void Model::rotate(glm::quat rotation) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].rotate(rotation);
    }
}

void Model::translate(glm::vec3 translation) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].translate(translation);
    }
}

void Model::scale(glm::vec3 scale) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].scale(scale);
    }
}

void Model::import(const char* path) {
    std::string text = Utils::readFile(path);
    json = nlohmann::json::parse(text);
    
    std::string pathString(path);
    std::string filePath = pathString.substr(0, pathString.find_last_of('/') + 1);
    std::string uri = json["buffers"][0]["uri"];
    std::string bytesText = Utils::readFile((filePath + uri).c_str());
    
    data = std::vector<unsigned char>(bytesText.begin(), bytesText.end());
}

void Model::loadMesh(unsigned int indexMesh, glm::mat4 transform) {
    unsigned int positionIndex = json["meshes"][indexMesh]["primitives"][0]["attributes"]["POSITION"];
    unsigned int normalIndex = json["meshes"][indexMesh]["primitives"][0]["attributes"]["NORMAL"];
    unsigned int texCoordIndex = json["meshes"][indexMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
    unsigned int indexIndex = json["meshes"][indexMesh]["primitives"][0]["indices"];

    std::vector<glm::vec3> positions = getVec3s(getFloats(json["accessors"][positionIndex]));
    std::vector<glm::vec3> normals = getVec3s(getFloats(json["accessors"][normalIndex]));
    std::vector<glm::vec2> texCoords = getVec2s(getFloats(json["accessors"][texCoordIndex]));
    std::vector<GLuint> indices = getIndices(json["accessors"][indexIndex]);
    std::vector<Texture> textures = getTextures();

    std::vector<Vertex> vertices = assembleVertices(positions, normals, texCoords);
    meshes.push_back(Mesh(vertices, indices, textures, transform));
}

void Model::traverseNode(unsigned int nextNode, glm::mat4 prevTransform) {
    auto& node = json["nodes"][nextNode];

    // Extract translation (if it exists)
    glm::vec3 translation(0.0f);
    if (node.contains("translation")) {
        float transValues[3];
        for (unsigned int i = 0; i < node["translation"].size(); i++) {
            transValues[i] = node["translation"][i];
        }
        translation = glm::make_vec3(transValues);
    }

    // Extract rotation (if it exists)
    glm::quat rotation(1.0f, 0.0f, 0.0f, 0.0f);
    if (node.contains("rotation")) {
        float rotValues[4] = {
            node["rotation"][3],
            node["rotation"][0],
            node["rotation"][1],
            node["rotation"][2]
        };
        rotation = glm::make_quat(rotValues);
    }

    // Extract scale (if it exists)
    glm::vec3 scale(1.0f);
    if (node.contains("scale")) {
        float scaleValues[3];
        for (unsigned int i = 0; i < node["scale"].size(); i++) {
            scaleValues[i] = node["scale"][i];
        }
        scale = glm::make_vec3(scaleValues);
    }

    // Extract transform matrix (if it exists)
    glm::mat4 transform(1.0f);
    if (node.contains("matrix")) {
        float matValues[16];
        for (unsigned int i = 0; i < node["matrix"].size(); i++) {
            matValues[i] = node["matrix"][i];
        }
        transform = glm::make_mat4(matValues);
    }

	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);
	glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

    // GLTF node transforms: either a 4x4 transform, or TRS (translation, rotation, scale)
    // see https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#transformations
    transform = prevTransform * transform * translationMatrix * rotationMatrix * scaleMatrix;

    if (node.contains("mesh")) {
		loadMesh(node["mesh"], transform);
	}

    if (node.contains("children")) {
        for (unsigned int i = 0; i < node["children"].size(); i++) {
            traverseNode(node["children"][i], transform);
        }
    }
}

std::vector<float> Model::getFloats(nlohmann::json accessor) {
    std::vector<float> floatVec;

    unsigned int bufferViewIndex = accessor.value("bufferView", 1);
    unsigned int accessorByteOffset = accessor.value("byteOffset", 0);
    unsigned int count = accessor["count"];
    std::string type = accessor["type"];

    const auto& bufferView = json["bufferViews"][bufferViewIndex];
    unsigned int byteOffset = bufferView["byteOffset"];

    unsigned int nComponents;
    if (type == "SCALAR") nComponents = 1;
    else if (type == "VEC2") nComponents = 2;
    else if (type == "VEC3") nComponents = 3;
    else if (type == "VEC4") nComponents = 4;
    else throw std::invalid_argument("Type not supported (expected SCALAR, VEC2, VEC3, or VEC4)");

    unsigned int start = byteOffset + accessorByteOffset;
    unsigned int length = count * 4 * nComponents;
    for (unsigned int i = start; i < start + length; i += 4) {
        unsigned char bytes[] = { data[i], data[i + 1], data[i + 2], data[i + 3] };
        float value;
        std::memcpy(&value, bytes, sizeof(float));
        floatVec.push_back(value);
    }
    return floatVec;
}

std::vector<GLuint> Model::getIndices(nlohmann::json accessor) {
    std::vector<GLuint> indices;
    
    unsigned int bufferViewIndex = accessor.value("bufferView", 0);
    unsigned int accessorByteOffset = accessor.value("byteOffset", 0);
    unsigned int count = accessor["count"];
    unsigned int componentType = accessor["componentType"];

    const auto& bufferView = json["bufferViews"][bufferViewIndex];
    unsigned int byteOffset = bufferView["byteOffset"];

    unsigned int start = byteOffset + accessorByteOffset;
    switch (componentType) {
        case 5125: { // unsigned int
            unsigned int length = count * 4;
            for (unsigned int i = start; i < start + length; i += 4) {
                unsigned char bytes[] = { data[i], data[i + 1], data[i + 2], data[i + 3] };
                unsigned int value;
                std::memcpy(&value, bytes, sizeof(unsigned int));
                indices.push_back((GLuint)value);
            }
            break;
        }
        case 5123: { // unsigned short
            unsigned int length = count * 2;
            for (unsigned int i = start; i < start + length; i += 2) {
                unsigned char bytes[] = { data[i], data[i + 1] };
                unsigned short value;
                std::memcpy(&value, bytes, sizeof(unsigned short));
                indices.push_back((GLuint)value);
            }
            break;
        }
        case 5122: { // short
            unsigned int length = count * 2;
            for (unsigned int i = start; i < start + length; i += 2) {
                unsigned char bytes[] = { data[i], data[i + 1] };
                short value;
                std::memcpy(&value, bytes, sizeof(short));
                indices.push_back((GLuint)value);
            }
            break;
        }
    }
    return indices;
}

std::vector<Texture> Model::getTextures() {
    std::vector<Texture> textures;
    std::string pathString(path);
    std::string filePath = pathString.substr(0, pathString.find_last_of('/') + 1);

    for (const auto& image : json["images"]) {
        std::string texPath = image["uri"];
        std::string fullPath = filePath + texPath;

        // Ensure textures are not loaded multiple times
        for (const Texture& tex : loadedTextures) {
            if (std::string(tex.getImage()) == fullPath) {
                textures.push_back(tex);
                continue;
            }
        }

        if (texPath.contains("baseColor")) {
            Texture diffuse(fullPath.c_str(), "diffuse", loadedTextures.size());
            textures.push_back(diffuse);
            loadedTextures.push_back(diffuse);
        }
        else if (texPath.contains("metallicRoughness")) {
            Texture specular(fullPath.c_str(), "specular", loadedTextures.size());
            textures.push_back(specular);
            loadedTextures.push_back(specular);
        }
    }   
    return textures;
}

std::vector<Vertex> Model::assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texCoords) {
    std::vector<Vertex> vertices;
    for (unsigned int i = 0; i < positions.size(); i++) {
        vertices.push_back(Vertex{positions[i], normals[i], glm::vec3(1.0f, 1.0f, 1.0f), texCoords[i]});
    }
    return vertices;
}

std::vector<glm::vec4> Model::getVec4s(std::vector<float> floatVec) {
    std::vector<glm::vec4> vec4s;
    const unsigned int floatsPerVector = 4;
    for (unsigned int i = 0; i < floatVec.size(); i += floatsPerVector) {
        vec4s.push_back(glm::vec4(floatVec[i], floatVec[i + 1], floatVec[i + 2], floatVec[i + 3]));
    }
    return vec4s;
}

std::vector<glm::vec3> Model::getVec3s(std::vector<float> floatVec) {
    std::vector<glm::vec3> vec3s;
    const unsigned int floatsPerVector = 3;
    for (unsigned int i = 0; i < floatVec.size(); i += floatsPerVector) {
        vec3s.push_back(glm::vec3(floatVec[i], floatVec[i + 1], floatVec[i + 2]));
    }
    return vec3s;
}

std::vector<glm::vec2> Model::getVec2s(std::vector<float> floatVec) {
    std::vector<glm::vec2> vec2s;
    for (unsigned int i = 0; i < floatVec.size(); i += 2) {
        vec2s.push_back(glm::vec2(floatVec[i], floatVec[i + 1]));
    }
    return vec2s;
}