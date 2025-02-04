#include "model.h"
#include "utils.h"

Model::Model(const char* path) {
    std::string text = Utils::readFile(path);
    json = nlohmann::json::parse(text);

    this->path = path;
    this->data = import();

    traverseNode(0);
}

void Model::draw(Shader& shader, Camera& camera) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].draw(shader, camera, matrices[i]);
    }
}

std::vector<unsigned char> Model::import() {
    std::string uri = json["buffers"][0]["uri"];
    std::string pathString = std::string(path);
    std::string filePath = pathString.substr(0, pathString.find_last_of('/') + 1);
    std::string bytesText = Utils::readFile((filePath + uri).c_str());
    std::vector<unsigned char> data(bytesText.begin(), bytesText.end());
    return data;
}

void Model::loadMesh(unsigned int indexMesh) {
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
    meshes.push_back(Mesh(vertices, indices, textures));
}

void Model::traverseNode(unsigned int nextNode, glm::mat4 matrix)
{
	// Current node
	nlohmann::json node = json["nodes"][nextNode];

	// Get translation if it exists
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
	if (node.find("translation") != node.end())
	{
		float transValues[3];
		for (unsigned int i = 0; i < node["translation"].size(); i++)
			transValues[i] = (node["translation"][i]);
		translation = glm::make_vec3(transValues);
	}
	// Get quaternion if it exists
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	if (node.find("rotation") != node.end())
	{
		float rotValues[4] =
		{
			node["rotation"][3],
			node["rotation"][0],
			node["rotation"][1],
			node["rotation"][2]
		};
		rotation = glm::make_quat(rotValues);
	}
	// Get scale if it exists
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	if (node.find("scale") != node.end())
	{
		float scaleValues[3];
		for (unsigned int i = 0; i < node["scale"].size(); i++)
			scaleValues[i] = (node["scale"][i]);
		scale = glm::make_vec3(scaleValues);
	}
	// Get matrix if it exists
	glm::mat4 matNode = glm::mat4(1.0f);
	if (node.find("matrix") != node.end())
	{
		float matValues[16];
		for (unsigned int i = 0; i < node["matrix"].size(); i++)
			matValues[i] = (node["matrix"][i]);
		matNode = glm::make_mat4(matValues);
	}

	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	trans = glm::translate(trans, translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);

	glm::mat4 matNextNode = matrix * matNode * trans * rot * sca;

	if (node.find("mesh") != node.end())
	{
		translations.push_back(translation);
		rotations.push_back(rotation);
		scales.push_back(scale);
		matrices.push_back(matNextNode);

		loadMesh(node["mesh"]);
	}

	if (node.find("children") != node.end())
	{
		for (unsigned int i = 0; i < node["children"].size(); i++)
			traverseNode(node["children"][i], matNextNode);
	}
}

std::vector<float> Model::getFloats(nlohmann::json accessor) {
    std::vector<float> floatVec;
    unsigned int bufferViewIndex = accessor.value("bufferView", 1);
    unsigned int accessorByteOffset = accessor.value("byteOffset", 0);
    unsigned int count = accessor["count"];
    std::string type = accessor["type"];

    nlohmann::json bufferView = json["bufferViews"][bufferViewIndex];
    unsigned int byteOffset = bufferView["byteOffset"];

    unsigned int nComponents = 4;
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

    nlohmann::json bufferView = json["bufferViews"][bufferViewIndex];
    unsigned int byteOffset = bufferView["byteOffset"];

    unsigned int start = byteOffset + accessorByteOffset;
    if (componentType == 5125) {
        unsigned int length = count * 4;
        for (unsigned int i = start; i < start + length; i += 4) {
            unsigned char bytes[] = { data[i], data[i + 1], data[i + 2], data[i + 3] };
            unsigned int value;
            std::memcpy(&value, bytes, sizeof(unsigned int));
            indices.push_back((GLuint)value);
        }
    }
    else if (componentType == 5123) {
        unsigned int length = count * 2;
        for (unsigned int i = start; i < start + length; i += 2) {
            unsigned char bytes[] = { data[i], data[i + 1] };
            unsigned short value;
            std::memcpy(&value, bytes, sizeof(unsigned short));
            indices.push_back((GLuint)value);
        }
    }
    else if (componentType == 5122) {
        unsigned int length = count * 2;
        for (unsigned int i = start; i < start + length; i += 2) {
            unsigned char bytes[] = { data[i], data[i + 1] };
            short value;
            std::memcpy(&value, bytes, sizeof(short));
            indices.push_back((GLuint)value);
        }
    }
    return indices;
}

std::vector<Texture> Model::getTextures() {
    std::vector<Texture> textures;
    std::string pathString = std::string(path);
    std::string filePath = pathString.substr(0, pathString.find_last_of('/') + 1);

    for (unsigned int i = 0; i < json["images"].size(); i++) {
        std::string texPath = json["images"][i]["uri"];

        bool skip = false;
        for (unsigned int j = 0; j < loadedTextureNames.size(); j++) {
            if (loadedTextureNames[j] == texPath) {
                textures.push_back(textures[j]);
                skip = true;
                break;
            }
        }
        if (!skip) {
            // Load diffuse texture
            if (texPath.find("baseColor") != std::string::npos) {
                Texture diffuse = Texture((filePath + texPath).c_str(), "diffuse", loadedTextures.size());
                textures.push_back(diffuse);
                loadedTextures.push_back(diffuse);
                loadedTextureNames.push_back(texPath);
            }
            // Load specular texture
            else if (texPath.find("metallicRoughness") != std::string::npos) {
                Texture specular = Texture((filePath + texPath).c_str(), "specular", loadedTextures.size());
                textures.push_back(specular);
                loadedTextures.push_back(specular);
                loadedTextureNames.push_back(texPath);
            }
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
    const unsigned int floatsPerVector = 2;
    for (unsigned int i = 0; i < floatVec.size(); i += floatsPerVector) {
        vec2s.push_back(glm::vec2(floatVec[i], floatVec[i + 1]));
    }
    return vec2s;
}