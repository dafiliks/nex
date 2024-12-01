#ifndef PARSER_HPP
#define PARSER_HPP

#include <optional>

#include "ast.hpp"

class Parser
{
public:
	Parser(const std::vector<Token>& c_tokens);

	Expr parse_expr();
	Stmt parse_stmt();
	FuncDecl parse_func_decl();
	void parse_program();

	Token peek(const std::size_t c_offset = 0) const;
	Token advance(const std::size_t c_distance = 1);

	Program get_program() const;
	std::vector<Token> get_tokens() const;
private:
	Program m_program{};
	std::vector<Token> m_tokens{};

	std::size_t m_index{};
};

#endif
