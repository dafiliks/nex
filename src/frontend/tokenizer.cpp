#include <cctype>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <map>
#include "tokenizer.hpp"

Token::Token(const std::string& value, const TokenType& type) : m_value(value), m_type(type) {}

std::string Token::get_value() const {
    return m_value;
}

TokenType Token::get_type() const {
    return m_type;
}

bool is_skippable(const char character) {
    return character == ' ' || character == '\n' || character == '\t';
}

std::string char_to_string(const char character) {
    return std::string(1, character);
}

std::string to_string(const TokenType token_type) {
    auto match = token_names.find(token_type);
    if (match != token_names.end()) {
        return match->second;
    }
    throw std::runtime_error("Token unknown");
}

Tokenizer::Tokenizer(const std::string& src) : m_src(src) {
    m_src += " ";
}

std::vector<Token> Tokenizer::get_tokens() const {
    return m_tokens;
}

char Tokenizer::peek(const std::size_t offset) const {
    if (m_index + offset < m_src.size()) {
        return m_src.at(m_index + offset);
    } else {
        throw std::out_of_range("Tokenizer peek offset out of range");
    }
}

void Tokenizer::consume(const std::size_t distance) {
    if (m_index + distance < m_src.size()) {
        m_index += distance;
    } else {
        throw std::out_of_range("Tokenizer consume distance out of range");
    }
}

std::vector<Token> Tokenizer::tokenize() {
    while (m_index < m_src.size() - 1) {
        if (!is_skippable(peek())) {
            auto char_token = single_char_tokens.find(peek());
            if (char_token != single_char_tokens.end()) {
                    m_tokens.push_back({char_to_string(peek()), char_token->second});
                    consume();
            } else {
                if (std::isdigit(peek())) {
                    m_buffer += peek();
                    if (!std::isdigit(peek(1))) {
                        m_tokens.push_back({m_buffer, TokenType::int_lit});
                        m_buffer.clear();
                    }
                    consume();
                } else if (std::isalpha(peek())) {
                    m_buffer += peek();
                    if (!std::isalpha(peek(1))) {
                        auto keyword = nex_keywords.find(m_buffer);
                        if (keyword != nex_keywords.end()) {
                            m_tokens.push_back({m_buffer, keyword->second});
                        } else {
                            m_tokens.push_back({m_buffer, TokenType::identifier});
                        }
                        m_buffer.clear();
                    }
                    consume();
                } else {
                    throw std::runtime_error("No matching token found for " + peek());
                }
            }
        } else {
            consume();
        }
    }
    m_tokens.push_back({"eof", TokenType::eof});
    return m_tokens;
}