#ifndef AST_HPP
#define AST_HPP

#include <vector>
#include <string>
#include <variant>
#include <memory>
#include <cstdint>
#include "tokenizer.hpp"

struct Normal {
    int m_constant{};
};

struct Return {
    int m_constant{};
};

struct Expr {
    std::variant<Normal, Return> m_expr_value{};
};

struct Stmt {
    Expr m_expr{};
};

struct FuncDecl {
    std::string m_name{};
    std::vector<Stmt> m_body{};
};

struct Program {
    std::vector<std::variant<FuncDecl, Stmt>> m_body{};
};

void print_expr(const Expr& expr);
void print_stmt(const Stmt& stmt);
void print_func_decl(const FuncDecl& func_decl);
void print_program(const Program& program);

#endif