#ifndef AST_HPP
#define AST_HPP

#include <vector>
#include <string>
#include <variant>
#include <memory>

#include "tokenizer.hpp"

struct Normal
{
    int m_constant{};
};

struct Return
{
    int m_constant{};
};

struct UnOp;

struct Expr
{
    std::variant<std::shared_ptr<UnOp>, Normal, Return> m_expr_v{};
};

struct UnOp
{
    TokenType m_op{};
    std::shared_ptr<Expr> m_expr{};

    UnOp(const TokenType c_op, const std::shared_ptr<Expr> c_expr);
};

struct Stmt
{
    Expr m_expr{};
};

struct FuncDecl
{
    std::string m_name{};
    std::vector<Stmt> m_body{};
};

struct Program
{
    std::vector<std::variant<FuncDecl, Stmt>> m_body{};
};

void print_expr(const Expr& c_expr);
void print_stmt(const Stmt& c_stmt);
void print_func_decl(const FuncDecl& c_func_decl);
void print_program(const Program& c_program);

#endif