#include "utils.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace Utils {
    std::string readFile(const char* filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + std::string(filename));
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        return buffer.str();
    }
}