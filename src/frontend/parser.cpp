#include <optional>
#include <stdexcept>
#include "tokenizer.hpp"
#include "parser.hpp"

Parser::Parser(const std::vector<Token>& tokens): m_tokens(tokens) {}

Expr Parser::parse_expr() {
    Expr expr{};
    switch (peek().get_type()) {
    case (TokenType::int_lit):
        expr.m_expr_value = Return(std::stoi(peek().get_value()));
        consume();
        break;
    default:
        throw std::runtime_error("Parsing expression failed");
    }
    return expr;
}

Stmt Parser::parse_stmt() {
    Stmt stmt {};
    if (peek().get_type() == TokenType::int_lit) {
        stmt.m_expr = parse_expr();
        consume();
    } else {
        throw std::runtime_error("Parsing statement failed");
    }
    return stmt;
}

FuncDecl Parser::parse_func_decl() {
    FuncDecl func_decl {};
    if (peek().get_type() == TokenType::c_paren &&
        peek(1).get_type() == TokenType::o_bracket) {
        func_decl.m_name = peek(-2).get_value();
        consume(2);
        while (peek().get_type() != TokenType::c_bracket) {
            if (peek().get_type() == TokenType::return_) {
                consume();
                func_decl.m_body.push_back(parse_stmt());
            } else {
                consume();
            }
        }
    } else {
        throw std::runtime_error("Parsing function declaration failed");
    }
    return func_decl;
}

void Parser::parse_program() {
    while (peek().get_type() != TokenType::eof) {
        if (peek().get_type() == TokenType::int_ &&
            peek(1).get_type() == TokenType::identifier &&
            peek(2).get_type() == TokenType::o_paren) {
            consume(3);
            m_program.m_body.push_back(parse_func_decl());
        } else {
            consume();
        }
    }
}

Token Parser::peek(const std::size_t offset) const {
    if (m_index + offset < m_tokens.size()) {
        return m_tokens.at(m_index + offset);
    } else {
        throw std::out_of_range("Parser peek offset out of range");
    }
}

void Parser::consume(const std::size_t distance) {
    if (m_index + distance < m_tokens.size()) {
        m_index += distance;
    } else {
        throw std::out_of_range("Parser consume distance out of range");
    }
}

std::vector<Token> Parser::get_tokens() const {
    return m_tokens;
}

Program Parser::get_program() const {
    return m_program;
}