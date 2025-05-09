// generator.hpp
// Copyright (C) David Filiks

#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <sstream>
#include <unordered_map>
#include "../frontend/ast.hpp"

static std::unordered_map<std::string, VariableStmt> existing_vars{};
static std::unordered_map<std::string, ArrayStmt> existing_arrs{};
static std::unordered_map<std::string, LabelStmt> existing_labels{};

class Generator
{
public:
	Generator(const Program& program);
    void gen_term(const TermExpr& term);
	void gen_expr(const Expr& expr);
    void gen_scope(const Scope& scope);
	void gen_stmt(const Stmt& stmt);
	void gen_program();
	void push(const std::string& reg);
	void pop(const std::string& reg);
    void new_line();
    void gen_error(const std::string& err_message) const;
    std::string get_output_str() const;

private:
	std::stringstream m_output{};
	Program m_program{};
	std::size_t m_stack_size{};
    std::size_t m_jz_count{};
    std::size_t m_arr_count{};
};

#endif
