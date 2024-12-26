#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include <vector>
#include <iostream>
#include <optional>
#include <unordered_map>

enum class TokenType {
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
    int_,
    return_,
    eof,
};

static const std::unordered_map<std::string, TokenType> nex_keywords {
    {"return", TokenType::return_},
    {"int",    TokenType::int_}
};

static const std::unordered_map<TokenType, std::string> token_names {
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
    {TokenType::int_,       "int"},
    {TokenType::return_,    "return"},
    {TokenType::eof,        "eof"}
};

static const std::unordered_map<char, TokenType> single_char_tokens {
    {'(', TokenType::o_paren},
    {')', TokenType::c_paren},
    {'{', TokenType::o_bracket},
    {'}', TokenType::c_bracket},
    {';', TokenType::semi},
    {'=', TokenType::equals},
    {'+', TokenType::add},
    {'*', TokenType::multiply},
    {'/', TokenType::divide},
};

class Token {
public: 
    Token(const std::string& value, const TokenType& type);
    std::string get_value() const;
    TokenType get_type() const;
private:
    std::string m_value{};
    TokenType m_type{};
};

bool is_skippable(const char character);
std::string char_to_string(const char character);
std::string to_string(const TokenType token_type);

class Tokenizer {
public:
    Tokenizer(const std::string& src);
    std::vector<Token> get_tokens() const;
    char peek(const std::size_t offset = 0) const;
    void consume(const std::size_t distance = 1);
    std::vector<Token> tokenize();
private:
    std::string m_src{};
    std::string m_buffer{};
    std::size_t m_index{};
    std::vector<Token> m_tokens{};
};

#endif