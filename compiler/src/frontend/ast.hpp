// ast.hpp
// Copyright (C) David Filiks

#ifndef AST_HPP
#define AST_HPP

#include <vector>
#include <string>
#include <variant>
#include <memory>

struct Expr;

struct ExitStmt
{
    std::shared_ptr<Expr> m_expr{};
};

struct VariableStmt
{
    std::string m_name{};
    std::shared_ptr<Expr> m_expr{};
    std::size_t m_stack_loc{};
};

struct ArrayStmt
{
    std::string m_name{};
    std::size_t m_arr_size{};
    std::size_t m_arr_head_loc{};
};

struct IdentStmt
{
    std::string m_dest{};
    std::shared_ptr<Expr> m_expr{};
    std::shared_ptr<Expr> m_index{};
};

struct LabelStmt
{
    std::string m_name{};
};

struct GoStmt
{
    std::string m_dest{};
};

struct OutStmt
{
    std::string m_output{};
};

struct Scope;

struct Stmt;

struct IfzStmt
{
    std::shared_ptr<Expr> m_cond{};
    std::shared_ptr<Scope> m_body{};
    std::shared_ptr<Stmt> m_el{};
};

struct ElStmt
{
    std::shared_ptr<Scope> m_body{};
};

struct Stmt
{
    std::variant<ExitStmt, VariableStmt, ArrayStmt, IdentStmt,
                 LabelStmt, GoStmt, OutStmt, IfzStmt, ElStmt> m_stmt{};
};

struct Scope
{
    std::vector<Stmt> m_body{};
};

struct IntExpr
{
    int m_value{};
};

struct IdentExpr
{
    std::string m_ident{};
};

struct ArrayExpr
{
    std::string m_ident{};
    std::shared_ptr<Expr> m_index{};
};

struct BinOpExpr
{
    std::shared_ptr<Expr> m_lhs{};
    std::shared_ptr<Expr> m_rhs{};
    std::string m_op{};
};

struct InExpr{};

struct TermExpr
{
    std::variant<IntExpr, InExpr, IdentExpr, ArrayExpr> m_term{};
};

struct Expr
{
    std::variant<TermExpr, BinOpExpr> m_value{};
};

struct Program
{
    std::vector<Stmt> m_body{};
};

#endif
