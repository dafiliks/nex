#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include "ast.hpp"

class Generator
{
public:
	Generator(const Program& c_program);

	void gen_expr(const Expr& c_expr);
	void gen_stmt(const Stmt& c_stmt);
	void gen_func_decl(const FuncDecl& c_func_decl);
	std::string gen_program();
private:
	Program m_program{};
	std::string m_output{};
	
	int m_neg_count{}, m_dec_count{}, m_comp_count{};
};

#endif