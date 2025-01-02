#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <sstream>
#include <unordered_map>
#include "../frontend/ast.hpp"

struct Variable {
	std::size_t m_stack_loc{};
};

static std::unordered_map<std::string, Variable> existing_vars{};
static std::unordered_map<std::string, FuncDecl> existing_funcs{};

class Generator {
public:
	Generator(const Program& program);
	void gen_expr(const Expr& expr);
	void gen_stmt(const Stmt& stmt);
	void gen_func_decl(const FuncDecl& func_decl);
	void gen_program();
	std::string get_output_str() const;
	void push(const std::string& reg);
	void pop(const std::string& reg);
private:
	std::stringstream m_output{};
	Program m_program{};
	std::size_t m_stack_size{};
};

#endif
