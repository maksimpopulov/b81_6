// Copyright 2026 Maksim Populov
#include "textgen.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0]
                  << " <input_file> [output_file]\n";
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = "result/gen.txt";
    if (argc >= 3) {
        outputFile = argv[2];
    }

    try {
        TextGenerator gen(2, 500);
        gen.build(inputFile);
        std::string generated = gen.generate();

        std::ofstream out(outputFile);
        out << generated;
        std::cout << "Generated text saved to " << outputFile << "\n";
        std::cout << "Preview:\n"
                  << generated.substr(0, 300) << "...\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
