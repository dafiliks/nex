#include <variant>
#include <iostream>

#include "ast.hpp"

void print_expr(const Expr& c_expr)
{
    std::cout << "Expr: { Type: ";

    if (std::holds_alternative<Return>(c_expr.m_expr_v))
    {
        std::cout << "ReturnExpr: { Constant: " << std::get<Return>(c_expr.m_expr_v).m_constant << " }";
    }
    else 
    {
        printf("else");
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
