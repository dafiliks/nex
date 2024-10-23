#include <cctype>
#include <cstdlib>
#include <iostream>
#include <unordered_map>

#include "tokenizer.hpp"

// reserved keywords hashmap
std::unordered_map<std::string, tokentype> keywords
{
    {"return", tokentype::t_return},
    {"let",    tokentype::t_let}
};

token::token(const std::string& value, const tokentype& type) : value(value), type(type) {}

// for printing
std::ostream& operator<<(std::ostream& os, const token& token) 
{
    os << "{ Value: " << token.value << ", Token: " << tokentypetostring(token.type) << " }";
    return os;
}

// functions for better code
void clear(std::string& s)       { s = ""; }
bool isskippable(const char c)   { return c == ' ' || c == '\n' || c == '\t'; }
std::string ctos(const char c)   { return std::string(1, c); }

std::string tokentypetostring(const tokentype type)
{
    switch (type)
    {
        case (tokentype::t_number):     return "t_number";
        case (tokentype::t_identifier): return "t_identifier";
        case (tokentype::t_equals):     return "t_equals";
        case (tokentype::t_binaryopp):  return "t_binaryopp";
        case (tokentype::t_oparen):     return "t_oparen"; 
        case (tokentype::t_cparen):     return "t_cparen"; 
        case (tokentype::t_obracket):   return "t_obracket"; 
        case (tokentype::t_cbracket):   return "t_cbracket"; 
        case (tokentype::t_let):        return "t_let"; 
        case (tokentype::t_return):     return "t_return"; 
    }
    return "NO TOKEN";
}

// magic
std::vector<token> tokenize(const std::string& source) 
{
    std::vector<token> tokens{};
    std::string buffer{};
    
    for (std::size_t i = 0; i < source.size(); i++)
    {
        // prevents out of bounds error
        if (i < source.size() - 1)
        {
            char current{source.at(i)};
            char next{source.at(i + 1)};

            // all single character tokens
            if (!isskippable(current))
            {
                if      (current == '(') { tokens.push_back({ctos(current), tokentype::t_oparen}); }
                else if (current == ')') { tokens.push_back({ctos(current), tokentype::t_cparen}); }
                else if (current == '{') { tokens.push_back({ctos(current), tokentype::t_obracket}); }
                else if (current == '}') { tokens.push_back({ctos(current), tokentype::t_cbracket}); }
                else if (current == '=') { tokens.push_back({ctos(current), tokentype::t_equals}); }
                else if (current == '+' || current == '-' || current == '/' || current == '*')
                {
                    tokens.push_back({ctos(current), tokentype::t_binaryopp}); 
                } 
                else
                { 
                    // integers
                    if (std::isdigit(current))
                    {
                        buffer += current;
                        if (!std::isdigit(next))
                        {
                            tokens.push_back({buffer, tokentype::t_number});
                            clear(buffer);
                        }
                    }
                    // identifiers/keywords
                    else if (std::isalpha(current))
                    {
                        buffer += current;
                        if (!std::isalpha(next))
                        {
                            // check for reserved keywords
                            auto reserved = keywords.find(buffer);
                            if (reserved != keywords.end()) 
                            {
                                tokens.push_back({buffer, reserved->second});
                                clear(buffer);
                            } 
                            else 
                            {
                                tokens.push_back({buffer, tokentype::t_identifier});
                                clear(buffer);
                            }
                        }    
                    }
                    // no matching token found
                    else 
                    {   
                        std::cerr << "Nex : no matching token found for \"" << current << "\"\n";
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
    }

    return tokens; 
}
