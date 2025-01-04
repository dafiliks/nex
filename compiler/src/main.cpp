// main.cpp
// Copyright (C) 2024 David Filiks <davidfiliks55@gmail.com>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include "frontend/tokenizer.hpp"
#include "frontend/parser.hpp"
#include "backend/generator.hpp"

void itf_error(const std::string& err_message) {
    std::cout << "Nex: interface error: " << err_message << ". Run nex --help\n";
    exit(EXIT_FAILURE);
}

bool is_nex_file(const char* file_name) {
    if (file_name[strlen(file_name) - 1] == 'x' &&
        file_name[strlen(file_name) - 2] == 'e' &&
        file_name[strlen(file_name) - 3] == 'n') {
        return true;
    }
    return false;
}

bool is_asm_file(const char* file_name) {
    if (file_name[strlen(file_name) - 1] == 'm' &&
        file_name[strlen(file_name) - 2] == 's' &&
        file_name[strlen(file_name) - 3] == 'a') {
        return true;
    }
    return false;
}

int main(int argc, char* argv[]) {
    if (argc == 2) {
        if (std::strcmp(argv[1], "--help") == 0) {
            std::cout << "Nex: Usage: nex <file.nex> -o <file.asm>\n";
            std::cout << "--version         Displays all version information\n";
            std::cout << "-o <file.asm>     Specifies the file to dump asm code into\n";
        } else if (std::strcmp(argv[1], "--version") == 0) {
            std::cout << "nex (Nex) v1.0\n";
            std::cout << "Copyright (C) 2024 David Filiks <davidfiliks55@gmail.com>\n";
            std::cout << "The software is provided \"as is\", without warranty of any kind\n";
        } else {
            itf_error("Invalid options");
        }
    } else if (argc == 4) {
        if (is_nex_file(argv[1])) {
            if (std::strcmp(argv[2], "-o") == 0) {
                if (is_asm_file(argv[3])) {
                    std::ifstream source_file{argv[1]};
                    std::stringstream buffer{};
                    buffer << source_file.rdbuf();
                    Tokenizer tokenizer{buffer.str()};
                    tokenizer.tokenize();
                    Parser parser{tokenizer.get_tokens()};
                    parser.parse_program();
                    Generator gen{parser.get_program()};
                    gen.gen_program();
                    gen.get_output_str();
                    std::ofstream output_file{argv[3]};
                    output_file << gen.get_output_str();
                } else {
                    itf_error("Expected output file name");
                }
            } else {
                itf_error("Expected an option");
            }
        } else {
            itf_error("Expected source file name");
        }
    } else {
        itf_error("Invalid options");
    }
    return 0;
}
