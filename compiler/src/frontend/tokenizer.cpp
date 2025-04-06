// tokenizer.cpp
// Copyright (C) David Filiks <davidfiliks55@gmail.com>

#include <cctype>
#include <cstdlib>
#include <iostream>
#include "tokenizer.hpp"

Token::Token(const std::string& value, const TokenType& type)
: m_value(value), m_type(type) {}

std::string Token::get_value() const {
    return m_value;
}

TokenType Token::get_type() const {
    return m_type;
}

Tokenizer::Tokenizer(const std::string& src) : m_src(src) {
    m_src += '\0';
}

std::vector<Token> Tokenizer::tokenize() {
    while (peek() != '\0') {
        if (!is_skippable(peek())) {
            auto match{single_char_tokens.find(peek())};
            if (match != single_char_tokens.end()) { // single char tokens
                m_tokens.push_back({char_to_string(consume()), match->second});
            } else {
                if (std::isdigit(peek())) { // integers
                    m_buffer += consume();
                    if (!std::isdigit(peek())) {
                        m_tokens.push_back({m_buffer, TokenType::int_lit});
                        m_buffer.clear();
                    }
                } else if (std::isalpha(peek()) || peek() == '_') { // identifiers/keywords
                    m_buffer += consume();
                    if (!std::isalpha(peek()) && peek() != '_') {
                        auto match{nex_keywords.find(m_buffer)};
                        if (match != nex_keywords.end()) {
                            m_tokens.push_back({m_buffer, match->second});
                        } else {
                            m_tokens.push_back({m_buffer, TokenType::identifier});
                        }
                        m_buffer.clear();
                    }
                } else if (consume() == '#') { // comments
                    if (peek() == '$') { // multiline
                        do consume();
                        while (peek() != '#' && peek() != '\0');
                        consume(2);
                    } else {
                        do consume();
                        while (peek() != '\n' && peek() != '\0');
                    }
                } else {
                    tokenizer_error("No matching token found");
                }
            }
        } else {
            consume();
        }
    }
    m_tokens.push_back({"eof", TokenType::eof});
    return m_tokens;
}

char Tokenizer::peek(const std::size_t offset) const {
    if (m_index + offset < m_src.size()) {
        return m_src.at(m_index + offset);
    } else {
        tokenizer_error("Peek offset out of range");
    }
}

char Tokenizer::consume(const std::size_t distance) {
    if (m_index + distance < m_src.size()) {
        m_index += distance;
        return m_src.at(m_index - distance);
    } else {
        tokenizer_error("Consume distance out of range");
    }
}

std::vector<Token> Tokenizer::get_tokens() const {
    return m_tokens;
}

void tokenizer_error(const std::string& err_message) {
    std::cout << "Nex: tokenizer error: " << err_message << "\n";
    exit(EXIT_FAILURE);
}

bool is_skippable(const char c) {
    return c == ' ' || c == '\n' || c == '\t';
}

char string_to_char(const std::string& s) {
    if (s.size() == 1) {
        return s.at(0);
    }
    tokenizer_error("String longer than 1 char");
}

std::string char_to_string(const char c) {
    return std::string(1, c);
}

std::string to_string(const TokenType token_type) {
    auto match{token_names.find(token_type)};
    if (match != token_names.end()) {
        return match->second;
    }
    tokenizer_error("Token unknown");
}
