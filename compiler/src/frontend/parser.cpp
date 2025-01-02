#include <stdexcept>
#include <iostream>
#include "tokenizer.hpp"
#include "parser.hpp"

Parser::Parser(const std::vector<Token>& tokens): m_tokens(tokens) {}

Expr Parser::parse_atom() {
    if (peek().get_type() == TokenType::int_lit) {
        return Expr(IntExpr(std::stoi(consume().get_value())));
    } else if (peek().get_type() == TokenType::string_lit) {
        return Expr(StringExpr(consume().get_value()));
    } else if (peek().get_type() == TokenType::identifier) {
        return Expr(IdentExpr(consume().get_value()));
    }
    throw std::runtime_error("Expected an atom");
}

Expr Parser::parse_expr(int min_prec) {
    Expr lhs{parse_atom()};
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
        lhs = Expr(bin_op);
    }
    return lhs;
}

Stmt Parser::parse_stmt() {
    if (peek().get_type() == TokenType::ret) {
        ReturnStmt ret{};
        consume();
        ret.m_expr = std::make_shared<Expr>(parse_expr());
        if (consume().get_type() == TokenType::semi) {
            return Stmt(ret);
        } else {
            throw std::runtime_error("Expected a \";\" after ret");
        }
    } else if (peek().get_type() == TokenType::var) {
        VariableStmt var{};
        consume();
        if (peek().get_type() == TokenType::identifier) {
            var.m_name = peek().get_value();
            consume();
            if (peek().get_type() == TokenType::equals) {
                consume();
                var.m_expr = std::make_shared<Expr>(parse_expr());
                if (consume().get_type() == TokenType::semi) {
                    return Stmt(var);
                } else {
                    throw std::runtime_error("Expected a \";\" after var def");
                }
            } else {
                throw std::runtime_error("Expected \"=\"");
            }
        } else {
            throw std::runtime_error("Expected an identifier");
        }
    } else if (peek().get_type() == TokenType::esc) {
        EscapeStmt esc{};
        consume();
        esc.m_expr = std::make_shared<Expr>(parse_expr());
        if (consume().get_type() == TokenType::semi) {
            return Stmt(esc);
        } else {
            throw std::runtime_error("Expected a \";\" after esc");
        }
    } else if (peek().get_type() == TokenType::set) { // not parsing it as expr because I don't want "set 52;"
        LabelStmt label{};
        consume();
        if (peek().get_type() == TokenType::identifier) {
            label.m_name = peek().get_value();
            consume();
            if (consume().get_type() == TokenType::semi) {
                return Stmt(label);
            } else {
                // t
            }
        } else {
            // t
        }
    } else if (peek().get_type() == TokenType::go) {
        GoStmt go{};
        consume();
        if (peek().get_type() == TokenType::identifier) {
            go.m_dest = peek().get_value();
            consume();
            if (consume().get_type() == TokenType::semi) {
                return Stmt(go);
            } else {
                // t
            }
        } else {
            // t
        }
    }
    throw std::runtime_error("Expected a statement");
}



FuncDecl Parser::parse_func_decl() {
    FuncDecl func_decl{};
    consume();
    if (peek().get_type() == TokenType::identifier) {
        if (peek().get_value() == "_start") {
            throw std::runtime_error("Function name cannot be _start\n");
        }
        func_decl.m_name = peek().get_value();
        consume();
        if (peek().get_type() == TokenType::o_paren) {
            // later on, we'll parse args here
            consume();
            if (peek().get_type() == TokenType::c_paren) {
                consume();
                if (peek().get_type() == TokenType::o_bracket) {
                    consume();
                    while (peek().get_type() != TokenType::c_bracket) {
                        if (is_stmt(peek().get_type())) {
                            func_decl.m_body.push_back(parse_stmt());
                        } else {
                            throw std::runtime_error("Expected a \"}\"");
                        }
                    }
                    consume();
                } else {
                    throw std::runtime_error("Expected a \"{\"");
                }
            } else {
                throw std::runtime_error("Expected a \")\"");
            }
        } else {
            throw std::runtime_error("Expected a \"(\"");
        }
    } else {
        throw std::runtime_error("Expected an identifier");
    }
    return func_decl;
}

void Parser::parse_program() {
    while (!is_eof(peek().get_type())) {
        try_consume_expect(TokenType::fn);
        m_program.m_body.push_back(parse_func_decl());
    }
    if (!contains_main()) {
        parser_rt_error("Nex: Program lacks an entry point");
    }
}

Token Parser::peek(const std::size_t offset) const {
    if (m_index + offset < m_tokens.size()) {
        return m_tokens.at(m_index + offset);
    } else {
        parser_rt_error("Nex: Parser -> peek offset out of range");
    }
}

Token Parser::consume(const std::size_t distance) {
    if (m_index + distance < m_tokens.size()) {
        m_index += distance;
        return m_tokens.at(m_index - distance);
    } else {
        parser_rt_error("Nex: Parser -> consume distance out of range");
    }
}

Token Parser::try_consume_expect(TokenType type) {
    if (peek().get_type() == type) {
        return consume();
    }
    parser_rt_error("Nex: Expected " + to_string(type));
}

void Parser::parser_rt_error(const std::string& err_message) const {
    throw std::runtime_error(err_message);
}

std::vector<Token> Parser::get_tokens() const {
    return m_tokens;
}

bool Parser::contains_main() const {
    for (const auto& item : m_program.m_body) {
        if (std::get<FuncDecl>(item).m_name == "main") {
            return true;
        }
    }
    return false;
}

Program Parser::get_program() const {
    return m_program;
}

bool is_eof(TokenType type) {
    if (type == TokenType::eof) {
        return true;
    }
    return false;
}

bool is_stmt(TokenType type) {
    if (type == TokenType::ret ||
        type == TokenType::var ||
        type == TokenType::esc ||
        type == TokenType::set ||
        type == TokenType::go) {
        return true;
    }
    return false;
}

bool is_bin_op(TokenType type) {
    if (type == TokenType::add ||
        type == TokenType::neg ||
        type == TokenType::multiply ||
        type == TokenType::divide) {
        return true;
    }
    return false;
}
