#include <iostream>
#include <fstream>
#include "global.h"

int main() {
    std::ifstream inFile(CNF_IN_FILE);
    if (! inFile.is_open()) {
        perror("CNF file open fail.");
    }
    std::string line;
    while (std::getline(inFile, line)) {
        std::cout << line << std::endl;
    }

    return 0;
}