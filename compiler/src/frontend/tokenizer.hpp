// tokenizer.hpp
// Copyright (C) David Filiks

#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include <vector>
#include <unordered_map>

enum class TokenType
{
    int_lit,
    semi,
    identifier,
    set,
    go,
    in,
    out,
    ifz,
    el,
    underscore,
    equals,
    add,
    neg,
    multiply,
    divide,
    sqo_bracket,
    sqc_bracket,
    o_bracket,
    c_bracket,
    var,
    arr,
    ex,
    eof,
};

static const std::unordered_map<std::string, TokenType> nex_keywords
{
    {"set", TokenType::set},
    {"go", TokenType::go},
    {"in", TokenType::in},
    {"out", TokenType::out},
    {"ifz", TokenType::ifz},
    {"el", TokenType::el},
    {"var", TokenType::var},
    {"arr", TokenType::arr},
    {"ex", TokenType::ex}
};

static const std::unordered_map<TokenType, std::string> token_names
{
    {TokenType::int_lit, "int_lit"},
    {TokenType::semi, "semi"},
    {TokenType::identifier, "identifier"},
    {TokenType::set, "set"},
    {TokenType::go, "go"},
    {TokenType::in, "in"},
    {TokenType::out, "out"},
    {TokenType::ifz, "ifz"},
    {TokenType::el, "el"},
    {TokenType::underscore, "underscore"},
    {TokenType::equals, "equals"},
    {TokenType::add, "add"},
    {TokenType::neg, "neg"},
    {TokenType::multiply, "multiply"},
    {TokenType::divide, "divide"},
    {TokenType::sqo_bracket, "sqo_bracket"},
    {TokenType::sqc_bracket, "sqc_bracket"},
    {TokenType::o_bracket, "o_bracket"},
    {TokenType::c_bracket, "c_bracket"},
    {TokenType::var, "var"},
    {TokenType::arr, "arr"},
    {TokenType::ex, "ex"},
    {TokenType::eof, "eof"}
};

static const std::unordered_map<char, TokenType> single_char_tokens
{
    {';', TokenType::semi},
    {'[', TokenType::sqo_bracket},
    {']', TokenType::sqc_bracket},
    {'{', TokenType::o_bracket},
    {'}', TokenType::c_bracket},
    {'=', TokenType::equals},
    {'+', TokenType::add},
    {'-', TokenType::neg},
    {'*', TokenType::multiply},
    {'/', TokenType::divide}
};

class Token
{
public:
    Token(const std::string& value, const TokenType& type);
    std::string get_value() const;
    TokenType get_type() const;

private:
    std::string m_value{};
    TokenType m_type{};
};

class Tokenizer
{
public:
    Tokenizer(const std::string& src);
    std::vector<Token> tokenize();
    char peek(const std::size_t offset = 0) const;
    char consume(const std::size_t distance = 1);
    std::vector<Token> get_tokens() const;

private:
    std::string m_src{};
    std::string m_buffer{};
    std::size_t m_index{};
    std::vector<Token> m_tokens{};
};

void tokenizer_error(const std::string& err_message);
bool is_skippable(const char c);
char string_to_char(const std::string& s);
std::string char_to_string(const char c);
std::string to_string(const TokenType token_type);

#endif
