#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>

#include "tokenizer.hpp"
#include "parser.hpp"
#include "ast.hpp"
#include "generator.hpp"

int main(int argc, char* argv[])
{
    std::vector<Token> tokens{};

    while (true)
    {
        std::string input{};
        std::cout << "> ";
        std::getline(std::cin, input);

        tokens = tokenize(input);

        std::cout << "\n";

        for (const Token& token : tokens)
        {
            std::cout << token << "\n";
        }

        Parser parser{ tokens };
        parser.parse_program();

        std::cout << "\n";

        print_program(parser.get_program());

        Generator generator{ parser.get_program() };
        std::cout << "\n" << generator.gen_program() << "\n\n";
    }

    return 0;
}