#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

enum class TokenType
{
    int_lit,
    identifier,

    semi,
    equals,

    add,
    multiply,
    divide,

    o_paren,
    c_paren,
    o_bracket,
    c_bracket,

    neg,
    b_compl,
    l_neg,

    int_,
    return_,

    eof,
};

extern const std::unordered_map<std::string, TokenType> c_keywords;

class Token 
{
public:
    Token(const std::string& c_value, const TokenType& c_type);
    friend std::ostream& operator<<(std::ostream& os, const Token& c_token); 

    std::string get_value() const;
    TokenType get_type() const;
private:
    std::string m_value{};
    TokenType m_type{};
};

bool is_skippable(const char c_c);
std::string ctos(const char c_c);

std::string to_string(const TokenType c_token_type);
std::vector<Token> tokenize(std::string& source);

#endif