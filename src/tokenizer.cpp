#include <cctype>
#include <cstdlib>
#include <iostream>
#include <map>

#include "tokenizer.hpp"

Token::Token(const std::string& c_value, const TokenType& c_type) : m_value(c_value), m_type(c_type) {}

std::ostream& operator<<(std::ostream& os, const Token& c_token) 
{
    os << "{ Value: " << c_token.m_value << ", Token: " << to_string(c_token.m_type) << " }";
    return os;
}

std::string Token::get_value() const
{
    return m_value; 
}

TokenType Token::get_type() const 
{
    return m_type; 
}

bool is_skippable(const char c_c) 
{
    return c_c == ' ' || c_c == '\n' || c_c == '\t'; 
}

std::string char_to_string(const char c_c) 
{
    return std::string(1, c_c); 
}

std::string to_string(const TokenType c_token_type)
{
    auto token = c_token_names.find(c_token_type);
    if (token != c_token_names.end())
    {
        return token->second;
    }
    return "Token unknown";
}

std::vector<Token> tokenize(std::string& source)
{
    source += " ";

    std::vector<Token> tokens{};
    std::string buffer{};

    for (std::size_t i = 0; i < source.size(); i++)
    {
        if (i < source.size() - 1)
        {
            char current{ source.at(i) };
            char next{ source.at(i + 1) };

            if (!is_skippable(current))
            {
                if (current == '-' && next == '-')
                {
                    tokens.push_back({ "--", TokenType::dec });
                    i++;
                    continue;
                }

                auto token = c_single_char_tokens.find(current);
                if (token != c_single_char_tokens.end())
                {
                    tokens.push_back({ char_to_string(current), token->second });
                }
                else
                {
                    if (std::isdigit(current))
                    {
                        buffer += current;
                        if (!std::isdigit(next))
                        {
                            tokens.push_back({ buffer, TokenType::int_lit });
                            buffer.clear();
                        }
                    }
                    else if (std::isalpha(current))
                    {
                        buffer += current;
                        if (!std::isalpha(next))
                        {
                            auto reserved = c_nex_keywords.find(buffer);
                            if (reserved != c_nex_keywords.end())
                            {
                                tokens.push_back({ buffer, reserved->second });
                            }
                            else
                            {
                                tokens.push_back({ buffer, TokenType::identifier });
                            }
                            buffer.clear();
                        }
                    }
                    else
                    {
                        std::cerr << "No matching token found for \"" << current << "\"\n";
                        exit(EXIT_FAILURE);
                    }
                }
                
            }
        }
    }

    tokens.push_back({ "eof", TokenType::eof });

    return tokens;
}