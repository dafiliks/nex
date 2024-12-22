#include <cctype>
#include <cstdlib>
#include <iostream>

#include "tokenizer.hpp"

const std::unordered_map<std::string, TokenType> c_keywords
{
    {"return", TokenType::return_},
    {"int",    TokenType::int_}
};

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

std::string ctos(const char c_c) 
{
    return std::string(1, c_c); 
}

std::string to_string(const TokenType c_token_type)
{
    switch (c_token_type)
    {
    case (TokenType::int_lit):
        return "int_lit";

    case (TokenType::identifier): 
        return "identifier";

    case (TokenType::semi):
        return "semi";

    case (TokenType::equals):
        return "equals";

    case (TokenType::add):
        return "add";

    case (TokenType::multiply):
        return "multiply";

    case (TokenType::divide):
        return "divide";

    case (TokenType::o_paren):
        return "o_paren";

    case (TokenType::c_paren):
        return "c_paren";

    case (TokenType::o_bracket):
        return "o_bracket";

    case (TokenType::c_bracket):
        return "c_bracket";

    case (TokenType::neg):
        return "neg";

    case (TokenType::b_compl):
        return "b_compl";

    case (TokenType::l_neg):
        return "l_neg";

    case (TokenType::int_):
        return "int";

    case (TokenType::return_):
        return "return";

    case (TokenType::eof):
        return "eof";

    default:
        return "unknown";
    }
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
                switch (current)
                {
                case ('('): 
                    tokens.push_back({ ctos(current), TokenType::o_paren });   
                    break;

                case (')'): 
                    tokens.push_back({ ctos(current), TokenType::c_paren });
                    break;

                case ('{'):
                    tokens.push_back({ ctos(current), TokenType::o_bracket });
                    break;

                case ('}'):
                    tokens.push_back({ ctos(current), TokenType::c_bracket });
                    break;

                case (';'):
                    tokens.push_back({ ctos(current), TokenType::semi });
                    break;

                case ('='):
                    tokens.push_back({ ctos(current), TokenType::equals });
                    break;

                case ('+'):
                    tokens.push_back({ ctos(current), TokenType::add });
                    break;

                case ('*'):
                    tokens.push_back({ ctos(current), TokenType::multiply });
                    break;

                case ('/'):
                    tokens.push_back({ ctos(current), TokenType::divide });
                    break;

                case ('-'):
                    tokens.push_back({ ctos(current), TokenType::neg });
                    break;

                case ('~'):
                    tokens.push_back({ ctos(current), TokenType::b_compl });
                    break;

                case ('!'):
                    tokens.push_back({ ctos(current), TokenType::l_neg });
                    break;

                default:
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
                            auto reserved = c_keywords.find(buffer);
                            if (reserved != c_keywords.end())
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
                        std::cerr << "Nex : no matching token found for \"" << current << "\"\n";
                        exit(EXIT_FAILURE);
                    }
                }

            }
        }
    }

    tokens.push_back({ "eof", TokenType::eof });

    return tokens;
}
