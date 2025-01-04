// ast.hpp
// Copyright (C) 2024 David Filiks <davidfiliks55@gmail.com>

#ifndef AST_HPP
#define AST_HPP

#include <vector>
#include <string>
#include <variant>
#include <memory>
#include <cstdint>

struct Expr;

struct EscapeStmt {
    std::shared_ptr<Expr> m_expr{};
};

struct ReturnStmt {
    std::shared_ptr<Expr> m_expr{};
};

struct VariableStmt {
    std::string m_name{};
    std::shared_ptr<Expr> m_expr{};
};

struct LabelStmt {
    std::string m_name{};
};

struct GoStmt {
    std::string m_dest{};
};

struct Stmt {
    std::variant<ReturnStmt, EscapeStmt, VariableStmt, LabelStmt, GoStmt> m_stmt{};
};

struct IntExpr {
    std::int32_t m_value{};
};

struct IdentExpr {
    std::string m_ident{};
};

struct BinOpExpr {
    std::shared_ptr<Expr> m_lhs{};
    std::shared_ptr<Expr> m_rhs{};
    std::string m_op{};
    int m_prec{};
};

struct TermExpr {
    std::variant<IntExpr, IdentExpr> m_term{};
};

struct Expr {
    std::variant<TermExpr, BinOpExpr> m_value{};
};

struct Scope {
    std::vector<Stmt> m_body{};
};

struct FuncDecl {
    std::string m_name{};
    Scope m_scope{};
};

struct Program {
    std::vector<FuncDecl> m_body{};
};

#endif
