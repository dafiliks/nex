#include <variant>
#include <iostream>
#include "ast.hpp"

void print_expr(const Expr& expr) {
    std::cout << "Expr: { Type: ";
    if (std::holds_alternative<Return>(expr.m_expr_value)) {
        std::cout << "ReturnExpr: { Constant: " << std::get<Return>(expr.m_expr_value).m_constant << " }";
    }
    std::cout << " }";
}

void print_stmt(const Stmt& stmt) {
    std::cout << "Stmt: { ";
    print_expr(stmt.m_expr);
    std::cout << " }";
}

void print_func_decl(const FuncDecl& func_decl) {
    std::cout << "FuncDecl: { Name: \"" << func_decl.m_name << "\", Body: [ ";
    for (const auto& stmt: func_decl.m_body) {
        print_stmt(stmt);
        std::cout << " ";
    }
    std::cout << "] }";
}

void print_program(const Program& program) {
    std::cout << "Program: [ ";
    for (const auto& item: program.m_body) {
        if (std::holds_alternative<FuncDecl>(item)) {
            print_func_decl(std::get<FuncDecl>(item));
        } else if (std::holds_alternative<Stmt>(item)) {
            print_stmt(std::get<Stmt>(item));
        }
        std::cout << " ";
    }
    std::cout << "]\n";
}