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

struct StringExpr {
    std::string m_value{};
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

struct Expr {
    std::variant<IntExpr, StringExpr, IdentExpr, BinOpExpr> m_value{};
};

struct FuncDecl {
    std::string m_name{};
    std::vector<Stmt> m_body{};
};

struct Program {
    std::vector<std::variant<FuncDecl>> m_body{};
};

#endif
