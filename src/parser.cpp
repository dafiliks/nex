#include <optional>

#include "tokenizer.hpp"
#include "parser.hpp"

Parser::Parser(const std::vector<Token>& c_tokens) : m_tokens(c_tokens) {}

Expr Parser::parse_expr()
{
	if (peek().get_type() == TokenType::int_lit)
	{
		Expr expr{ .m_int_lit = std::stoi(peek().get_value())};

		advance();

		return expr;
	}
	else
	{
		std::cerr << "Nex : parsing expression failed\n";
		exit(EXIT_FAILURE);
	}
}

Stmt Parser::parse_stmt()
{
	if (peek().get_type() == TokenType::int_lit)
	{

		Expr return_expr{ parse_expr() };
		return_expr.m_expr_type = ExprType::return_;
		Stmt stmt{ .m_expr = return_expr };

		advance();

		return stmt;
	}
	else
	{
		std::cerr << "Nex : parsing statement failed\n";
		exit(EXIT_FAILURE);
	}
}

FuncDecl Parser::parse_func_decl()
{
	if (peek().get_type() == TokenType::c_paren &&
		peek(1).get_type() == TokenType::o_bracket)
	{
		FuncDecl func_decl{};
		func_decl.m_name = peek(-2).get_value();

		advance(2);

		while (peek().get_type() != TokenType::c_bracket)
		{
			if (peek().get_type() == TokenType::return_)
			{
				advance();
				func_decl.m_body.push_back(parse_stmt());
			}
			else
			{
				advance();
			}
		}

		return func_decl;
	}
	else
	{
		std::cerr << "Nex : parsing function declaration failed\n";
		exit(EXIT_FAILURE);
	}
}

void Parser::parse_program()
{
	while (peek().get_type() != TokenType::eof)
	{
		if (peek().get_type() == TokenType::int_ &&
			peek(1).get_type() == TokenType::identifier &&
			peek(2).get_type() == TokenType::o_paren)
		{
			advance(3);
			m_program.m_body.push_back(parse_func_decl());
		}
		else
		{
			advance();
		}
	}
}

Token Parser::peek(const std::size_t c_offset) const
{
	if (m_index + c_offset < m_tokens.size())
	{
		return m_tokens.at(m_index + c_offset);
	}
	throw std::out_of_range("Nex : Peek offset out of range");
}

Token Parser::advance(const std::size_t c_distance)
{
	if (m_index + c_distance < m_tokens.size())
	{
		m_index += c_distance;
		return m_tokens.at(m_index);
	}
	throw std::out_of_range("Nex : Advance offset out of range");
}

std::vector<Token> Parser::get_tokens() const { return m_tokens; }
Program Parser::get_program()           const { return m_program; }
