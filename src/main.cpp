#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>
#include "frontend/tokenizer.hpp"
#include "frontend/parser.hpp"
#include "frontend/ast.hpp"
#include "backend/generator.hpp"

int main(int argc, char* argv[]) {
    while (true) {
        std::string input{};
        std::cout << "> ";
        std::getline(std::cin, input);
        Tokenizer tokens{input};
        tokens.tokenize();
        std::cout << "\n";
        for (const auto& token: tokens.get_tokens()) {
            std::cout << "{ Value: " << token.get_value() << ", Token: " << to_string(token.get_type()) << " }\n";
        }
        Parser parser{tokens.get_tokens()};
        parser.parse_program();
        std::cout << "\n";
        print_program(parser.get_program());
        Generator generator {parser.get_program()};
        std::cout << "\n" << generator.gen_program() << "\n\n";
    }
    return 0;
}