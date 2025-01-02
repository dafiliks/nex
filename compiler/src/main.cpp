#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>
#include "frontend/tokenizer.hpp"
#include "frontend/parser.hpp"
#include "frontend/ast.hpp"
#include "backend/generator.hpp"

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::ifstream file{argv[argc - 1]};
	    std::stringstream buffer{};
	    buffer << file.rdbuf();
        Tokenizer tokens{buffer.str()};
        tokens.tokenize();
        Parser parser{tokens.get_tokens()};
        parser.parse_program();
        Generator generator{parser.get_program()};
        generator.gen_program();
        std::cout << "\n" << generator.get_output_str() << "\n";
    } else {
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
            Generator generator{parser.get_program()};
            generator.gen_program();
            std::cout << "\n" << generator.get_output_str() << "\n";
        }
    }
    return 0;
}