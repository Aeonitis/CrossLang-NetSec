// main.cpp
// Demonstrates usage of our SHA256 hashing functions

#include "sha256.h"
#include <iostream>
#include <fstream>
#include <string>

bool fileExists(const std::string& filePath) {
    std::ifstream file(filePath);
    return file.good();
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::string file_path = argv[1];

    if (!fileExists(file_path)) {
        std::cerr << "Error: File not found - " << file_path << std::endl;
        return 1;
    }

    try {
        std::string hash = shortHashFile(file_path);
        std::cout << hash << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
