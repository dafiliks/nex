// parser.hpp
// Copyright (C) David Filiks

#ifndef PARSER_HPP
#define PARSER_HPP

#include <unordered_map>
#include "ast.hpp"
#include "tokenizer.hpp"

static const std::unordered_map<TokenType, int> bin_op_prec{
    {TokenType::add, 1},
    {TokenType::neg, 1},
    {TokenType::multiply, 2},
    {TokenType::divide, 2},
};

class Parser {
public:
	Parser(const std::vector<Token>& tokens);
    TermExpr parse_term();
    Expr parse_expr(int min_prec = 1);
    Scope parse_scope();
    Stmt parse_stmt();
    void parse_program();
    Token peek(const std::size_t offset = 0) const;
    Token consume(const std::size_t distance = 1);
    Token try_consume_expect(const TokenType type);
    void parser_error(const std::string& err_message) const;
    Program get_program() const;
    std::vector<Token> get_tokens() const;
    bool contains_main() const;
private:
	Program m_program{};
    std::vector<Token> m_tokens{};
    std::size_t m_index{};
};

bool is_eof(const TokenType type);
bool is_stmt(const TokenType type);
bool is_bin_op(const TokenType type);

#endif
