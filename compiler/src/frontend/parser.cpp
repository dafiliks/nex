#include <stdexcept>
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
    parser_rt_error("Expected an atom");
}

Expr Parser::parse_expr(int min_prec) {
    Expr lhs{parse_term()};
    int prec{1};
    int next_min_prec{};
    while (is_bin_op(peek().get_type()) &&
        !is_eof(peek().get_type()) &&
        prec >= min_prec) {
        std::string op{consume().get_value()};
        next_min_prec = prec + 1;
        Expr rhs{parse_expr(next_min_prec)};
        BinOpExpr bin_op{};
        bin_op.m_op = op;
        bin_op.m_lhs = std::make_shared<Expr>(lhs);
        bin_op.m_rhs = std::make_shared<Expr>(rhs);
        auto match{bin_op_prec.find(peek().get_type())};
        if (match != bin_op_prec.end()) {
            bin_op.m_prec = match->second;
        }
        lhs = Expr{bin_op};
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
    parser_rt_error("Expected statement");
}

Scope Parser::parse_scope() {
    Scope scope{};
    while (peek().get_type() != TokenType::c_bracket) {
        if (is_stmt(peek().get_type())) {
            scope.m_body.push_back(parse_stmt());
        } else {
            parser_rt_error("Expected " + to_string(TokenType::c_bracket));
        }
    }
    try_consume_expect(TokenType::c_bracket);
    return scope;
}

FuncDecl Parser::parse_func_decl() {
    FuncDecl func_decl{};
    try_consume_expect(TokenType::fn); // consume fn
    if (peek().get_value() == "_start") {
        parser_rt_error("Function name cannot be _start\n");
    }
    func_decl.m_name = peek().get_value();
    try_consume_expect(TokenType::identifier);
    try_consume_expect(TokenType::o_paren);
    // parse arguments here later
    try_consume_expect(TokenType::c_paren);
    try_consume_expect(TokenType::o_bracket);
    func_decl.m_scope = Scope{parse_scope()};
    return func_decl;
}

void Parser::parse_program() {
    while (!is_eof(peek().get_type())) {
        if (peek().get_type() == TokenType::fn) {
            m_program.m_body.push_back(parse_func_decl());
        }
    }
    if (!contains_main()) {
        parser_rt_error("Program lacks an entry point");
    }
}

Token Parser::peek(const std::size_t offset) const {
    if (m_index + offset < m_tokens.size()) {
        return m_tokens.at(m_index + offset);
    } else {
        parser_rt_error("Peek offset out of range");
    }
}

Token Parser::consume(const std::size_t distance) {
    if (m_index + distance < m_tokens.size()) {
        m_index += distance;
        return m_tokens.at(m_index - distance);
    } else {
        parser_rt_error("Consume distance out of range");
    }
}

Token Parser::try_consume_expect(const TokenType type) {
    if (peek().get_type() == type) {
        return consume();
    }
    parser_rt_error("Expected " + to_string(type));
}

void Parser::parser_rt_error(const std::string& err_message) const {
    throw std::runtime_error("Nex: Parser -> " + err_message);
}

std::vector<Token> Parser::get_tokens() const {
    return m_tokens;
}

bool Parser::contains_main() const {
    for (const auto& item : m_program.m_body) {
        if (item.m_name == "main") {
            return true;
        }
    }
    return false;
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
