#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include <vector>
#include <unordered_map>

enum class TokenType {
    int_lit,
    string_lit,
    semi,
    identifier,
    set,
    go,
    underscore,
    equals,
    add,
    neg,
    multiply,
    divide,
    o_paren,
    c_paren,
    o_bracket,
    c_bracket,
    fn,
    var,
    esc,
    ret,
    eof,
};

static const std::unordered_map<std::string, TokenType> nex_keywords {
    {"set", TokenType::set},
    {"go", TokenType::go},
    {"ret", TokenType::ret},
    {"fn", TokenType::fn},
    {"var", TokenType::var},
    {"esc", TokenType::esc}
};

static const std::unordered_map<TokenType, std::string> token_names {
    {TokenType::int_lit, "int_lit"},
    {TokenType::string_lit, "string_lit"},
    {TokenType::semi, "semi"},
    {TokenType::identifier, "identifier"},
    {TokenType::set, "set"},
    {TokenType::go, "go"},
    {TokenType::underscore, "underscore"},
    {TokenType::equals, "equals"},
    {TokenType::add, "add"},
    {TokenType::neg, "neg"},
    {TokenType::multiply, "multiply"},
    {TokenType::divide, "divide"},
    {TokenType::o_paren, "o_paren"},
    {TokenType::c_paren, "c_paren"},
    {TokenType::o_bracket, "o_bracket"},
    {TokenType::c_bracket, "c_bracket"},
    {TokenType::fn, "fn"},
    {TokenType::var, "var"},
    {TokenType::esc, "esc"},
    {TokenType::ret, "ret"},
    {TokenType::eof, "eof"}
};

static const std::unordered_map<char, TokenType> single_char_tokens {
    {';', TokenType::semi},
    {'(', TokenType::o_paren},
    {')', TokenType::c_paren},
    {'{', TokenType::o_bracket},
    {'}', TokenType::c_bracket},
    {'=', TokenType::equals},
    {'+', TokenType::add},
    {'-', TokenType::neg},
    {'*', TokenType::multiply},
    {'/', TokenType::divide}
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

class Tokenizer {
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

void tokenizer_rt_error(const std::string& err_message);
bool is_skippable(const char c);
char string_to_char(const std::string& s);
std::string char_to_string(const char c);
std::string to_string(const TokenType token_type);

#endif
