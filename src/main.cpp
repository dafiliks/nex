#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>

#include "tokenizer.hpp"


int main(int argc, char* argv[])
{
    std::vector<token> tokens{};

    std::ifstream file("test.txt");
    std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    tokens = tokenize(str);

    for (token token : tokens)
    {
        std::cout << token << "\n";
    }    

    return 0;
}