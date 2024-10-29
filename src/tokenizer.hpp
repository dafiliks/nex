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
    dec,
    comp,
    l_neg,

    int_,
    return_,

    eof,
};

static const std::unordered_map<std::string, TokenType> c_nex_keywords
{
    {"return", TokenType::return_},
    {"int",    TokenType::int_}
};

static const std::unordered_map<TokenType, std::string> c_token_names
{
    {TokenType::int_lit,    "int_lit"},
    {TokenType::identifier, "identifier"},
    {TokenType::semi,       "semi"},
    {TokenType::equals,     "equals"},
    {TokenType::add,        "add"},
    {TokenType::multiply,   "multiply"},
    {TokenType::divide,     "divide"},
    {TokenType::o_paren,    "o_paren"},
    {TokenType::c_paren,    "c_paren"},
    {TokenType::o_bracket,  "o_bracket"},
    {TokenType::c_bracket,  "c_bracket"},
    {TokenType::neg,        "neg"},
    {TokenType::dec,        "dec"},
    {TokenType::comp,       "comp"},
    {TokenType::l_neg,      "l_neg"},
    {TokenType::int_,       "int"},
    {TokenType::return_,    "return"},
    {TokenType::eof,        "eof"}
};

static const std::unordered_map<char, TokenType> c_single_char_tokens
{
    {'(', TokenType::o_paren},
    {')', TokenType::c_paren},
    {'{', TokenType::o_bracket},
    {'}', TokenType::c_bracket},
    {';', TokenType::semi},
    {'=', TokenType::equals},
    {'+', TokenType::add},
    {'*', TokenType::multiply},
    {'/', TokenType::divide},
    {'-', TokenType::neg},
    {'~', TokenType::comp},
    {'!', TokenType::l_neg}
};

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
std::string char_to_string(const char c_c) ;

std::string to_string(const TokenType c_token_type);
std::vector<Token> tokenize(std::string& source);

#endif
