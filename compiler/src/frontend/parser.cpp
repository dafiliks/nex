// parser.cpp
// Copyright (C) 2024 David Filiks <davidfiliks55@gmail.com>

#include <cstdlib>
#include <iostream>
#include <memory>
#include "tokenizer.hpp"
#include "parser.hpp"
#include "ast.hpp"

Parser::Parser(const std::vector<Token>& tokens): m_tokens(tokens) {}

TermExpr Parser::parse_term() {
    if (peek().get_type() == TokenType::int_lit) {
        return TermExpr{IntExpr{std::stoi(consume().get_value())}};
    } else if (peek().get_type() == TokenType::identifier) {
        return TermExpr{IdentExpr{consume().get_value()}};
    }
    parser_error("Expected term");
}

Expr Parser::parse_expr(int min_prec) {
    Expr lhs{parse_term()};
    while (true) {
        if (!is_bin_op(peek().get_type()) || is_eof(peek().get_type())) {
            break;
        }
        BinOpExpr bin_op_expr{};
        int prec{1};
        auto match{bin_op_prec.find(peek().get_type())};
        if (match != bin_op_prec.end()) {
            prec = match->second;
        }
        if (prec < min_prec) {
            break;
        }
        std::string op{consume().get_value()};
        Expr rhs{parse_expr(prec + 1)};
        bin_op_expr.m_op = op;
        bin_op_expr.m_lhs = std::make_shared<Expr>(lhs);
        bin_op_expr.m_rhs = std::make_shared<Expr>(rhs);
        lhs = Expr{bin_op_expr};
    }
    return lhs;
}

Stmt Parser::parse_stmt() {
    if (peek().get_type() == TokenType::ret) {
        ReturnStmt ret_stmt{};
        consume(); // consume ret
        ret_stmt.m_expr = std::make_shared<Expr>(parse_expr());
        try_consume_expect(TokenType::semi);
        return Stmt{ret_stmt};
    } else if (peek().get_type() == TokenType::var) {
        VariableStmt var_stmt{};
        consume(); // consume var
        var_stmt.m_name = peek().get_value();
        try_consume_expect(TokenType::identifier);
        try_consume_expect(TokenType::equals);
        var_stmt.m_expr = std::make_shared<Expr>(parse_expr());
        try_consume_expect(TokenType::semi);
        return Stmt{var_stmt};
    } else if (peek().get_type() == TokenType::esc) {
        EscapeStmt esc_stmt{};
        consume(); // consume esc
        esc_stmt.m_expr = std::make_shared<Expr>(parse_expr());
        try_consume_expect(TokenType::semi);
        return Stmt{esc_stmt};
    } else if (peek().get_type() == TokenType::set) {
        // not parsing it as expr because I don't want "set 52;" functionality
        LabelStmt label_stmt{};
        consume(); // consume set
        label_stmt.m_name = peek().get_value();
        try_consume_expect(TokenType::identifier);
        try_consume_expect(TokenType::semi);
        return Stmt{label_stmt};
    } else if (peek().get_type() == TokenType::go) {
        GoStmt go_stmt{};
        consume(); // consume go
        go_stmt.m_dest = peek().get_value();
        try_consume_expect(TokenType::identifier);
        try_consume_expect(TokenType::semi);
        return Stmt{go_stmt};
    }
    parser_error("Expected statement");
}

void Parser::parse_program() {
    while (!is_eof(peek().get_type())) {
        m_program.m_body.push_back(parse_stmt());
    }
}

Token Parser::peek(const std::size_t offset) const {
    if (m_index + offset < m_tokens.size()) {
        return m_tokens.at(m_index + offset);
    } else {
        parser_error("Peek offset out of range");
    }
}

Token Parser::consume(const std::size_t distance) {
    if (m_index + distance < m_tokens.size()) {
        m_index += distance;
        return m_tokens.at(m_index - distance);
    } else {
        parser_error("Consume distance out of range");
    }
}

Token Parser::try_consume_expect(const TokenType type) {
    if (peek().get_type() == type) {
        return consume();
    }
    parser_error("Expected " + to_string(type));
}

void Parser::parser_error(const std::string& err_message) const {
    std::cout << "Nex: parser error: " << err_message << "\n";
    exit(EXIT_FAILURE);
}

std::vector<Token> Parser::get_tokens() const {
    return m_tokens;
}

Program Parser::get_program() const {
    return m_program;
}

bool is_eof(const TokenType type) {
    if (type == TokenType::eof) {
        return true;
    }
    return false;
}

bool is_stmt(const TokenType type) {
    if (type == TokenType::ret ||
        type == TokenType::var ||
        type == TokenType::esc ||
        type == TokenType::set ||
        type == TokenType::go) {
        return true;
    }
    return false;
}

bool is_bin_op(const TokenType type) {
    if (type == TokenType::add ||
        type == TokenType::neg ||
        type == TokenType::multiply ||
        type == TokenType::divide) {
        return true;
    }
    return false;
}
