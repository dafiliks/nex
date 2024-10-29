#include <variant>
#include <iostream>

#include "ast.hpp"

UnOp::UnOp(const TokenType c_op, const std::shared_ptr<Expr> c_expr) : m_op(c_op), m_expr(c_expr) {};

void print_expr(const Expr& c_expr)
{
    std::cout << "Expr: { Type: ";

    if (std::holds_alternative<Return>(c_expr.m_expr_v))
    {
        std::cout << "ReturnExpr: { Constant: " << std::get<Return>(c_expr.m_expr_v).m_constant << " }";
    }
    else if (std::holds_alternative<std::shared_ptr<UnOp>>(c_expr.m_expr_v)) 
    {
        std::cout << "\nUnOp { Type: " << to_string(std::get<std::shared_ptr<UnOp>>(c_expr.m_expr_v)->m_op) << ", Expr: ";
        print_expr(*std::get<std::shared_ptr<UnOp>>(c_expr.m_expr_v)->m_expr);
        std::cout << " }";
    }

    std::cout << " }";
}

void print_stmt(const Stmt& c_stmt)
{
    std::cout << "Stmt: { ";
    print_expr(c_stmt.m_expr);
    std::cout << " }";
}

void print_func_decl(const FuncDecl& c_func_decl)
{
    std::cout << "FuncDecl: { Name: \"" << c_func_decl.m_name << "\", Body: [ ";
    for (const auto& stmt : c_func_decl.m_body)
    {
        print_stmt(stmt);
        std::cout << " ";
    }
    std::cout << "] }";
}

void print_program(const Program& c_program)
{
    std::cout << "Program: [ ";
    for (const auto& item : c_program.m_body)
    {
        if (std::holds_alternative<FuncDecl>(item))
        {
            print_func_decl(std::get<FuncDecl>(item));
        }
        else if (std::holds_alternative<Stmt>(item))
        {
            print_stmt(std::get<Stmt>(item));
        }
        std::cout << " ";
    }
    std::cout << "]\n";
}
