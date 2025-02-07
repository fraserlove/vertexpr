#pragma once

#include <string>
#include <fstream>

namespace Utils {
    inline std::string readFile(const char* filename) {
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Failed to open file: " + std::string(filename));
        }
        return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    }
}