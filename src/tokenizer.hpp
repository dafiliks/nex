#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include <vector>
#include <iostream>

enum class tokentype
{
    t_number,
    t_identifier,
    t_equals,
    t_binaryopp,
    t_oparen,
    t_cparen,
    t_obracket,
    t_cbracket,
    t_let,
    t_return
};

class token 
{
public:
    token(const std::string& value, const tokentype& type);
    friend std::ostream& operator<<(std::ostream& os, const token& token); 
private:
    std::string value;
    tokentype type;
};

void clear(std::string& s);
bool isskippable(const char c);
std::string ctos(const char c);
std::string tokentypetostring(const tokentype type);

std::vector<token> tokenize(const std::string& source);

#endif