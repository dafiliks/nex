// main.cpp
// Copyright (C) David Filiks

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

bool is_file_type_3(const char* file_name, const std::string ext) {
    if (file_name[strlen(file_name) - 3] == ext.at(0) &&
        file_name[strlen(file_name) - 2] == ext.at(1) &&
        file_name[strlen(file_name) - 1] == ext.at(2)) {
        return true;
    }
    return false;
}

int main(int argc, char* argv[]) {
    if (argc == 2) {
        if (std::strcmp(argv[1], "--help") == 0) {
            std::cout << "Nex: Usage: nex <file.nex> -o <file.asm>\n";
            std::cout << "nex --version         Displays all version information\n";
        } else if (std::strcmp(argv[1], "--version") == 0) {
            std::cout << "nex (Nex) v1.0\n";
            std::cout << "Copyright (C) David Filiks\n";
            std::cout << "The software is provided \"as is\", without warranty of any kind\n";
        } else {
            itf_error("Invalid options");
        }
    } else if (argc == 4) {
        if (is_file_type_3(argv[1], "nex")) {
            if (std::strcmp(argv[2], "-o") == 0) {
                if (is_file_type_3(argv[3], "asm")) {
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
        std::cout << "nex (Nex) v1.0 debug mode\n";
        std::string input{};
        while (true) {
            std::cout << "> ";
            std::getline(std::cin, input);
            std::cout << "\n";
            Tokenizer tokenizer{input};
            tokenizer.tokenize();
            Parser parser{tokenizer.get_tokens()};
            parser.parse_program();
            Generator gen{parser.get_program()};
            gen.gen_program();
            std::cout << gen.get_output_str();
        }
    }
    return 0;
}
