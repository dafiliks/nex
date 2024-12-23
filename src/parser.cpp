#include <optional>

#include "tokenizer.hpp"
#include "parser.hpp"

Parser::Parser(const std::vector<Token>& c_tokens) : m_tokens(c_tokens) {}

Expr Parser::parse_expr()
{
	Expr expr{};

	if (peek().get_type() == TokenType::int_lit)
	{
		expr.m_expr_v = Return(std::stoi(peek().get_value()));
		advance();
	}
	else if (peek().get_type() == TokenType::neg)
	{
		advance();
		expr.m_expr_v = std::make_shared<UnOp>(TokenType::neg, std::make_shared<Expr>(parse_expr()));
	}
	else if (peek().get_type() == TokenType::dec)
	{
		advance();
		expr.m_expr_v = std::make_shared<UnOp>(TokenType::dec, std::make_shared<Expr>(parse_expr()));
	}
	else if (peek().get_type() == TokenType::l_neg)
	{
		advance();
		expr.m_expr_v = std::make_shared<UnOp>(TokenType::l_neg, std::make_shared<Expr>(parse_expr()));
	}
	else if (peek().get_type() == TokenType::comp)
	{
		advance();
		expr.m_expr_v = std::make_shared<UnOp>(TokenType::comp, std::make_shared<Expr>(parse_expr()));
	}
	else
	{
		std::cerr << "Parsing expression failed\n";
		exit(EXIT_FAILURE);
	}

	return expr;
}

Stmt Parser::parse_stmt()
{
	Stmt stmt{};

	if (peek().get_type() == TokenType::int_lit || peek().get_type() == TokenType::neg ||
	    peek().get_type() == TokenType::dec || peek().get_type() == TokenType::l_neg ||
	    peek().get_type() == TokenType::comp)
	{
		stmt.m_expr = parse_expr();
		advance();
	}
	else
	{
		std::cerr << "Parsing statement failed\n";
		exit(EXIT_FAILURE);
	}

	return stmt;
}

FuncDecl Parser::parse_func_decl()
{
	FuncDecl func_decl{};

	if (peek().get_type() == TokenType::c_paren && peek(1).get_type() == TokenType::o_bracket)
	{
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
	}
	else
	{
		std::cerr << "Parsing function declaration failed\n";
		exit(EXIT_FAILURE);
	}

	return func_decl;
}

void Parser::parse_program()
{
	while (peek().get_type() != TokenType::eof)
	{
		if (peek().get_type() == TokenType::int_ && peek(1).get_type() == TokenType::identifier &&
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
	throw std::out_of_range("Peek offset out of range");
}

Token Parser::advance(const std::size_t c_distance)
{
	if (m_index + c_distance < m_tokens.size())
	{
		m_index += c_distance;
		return m_tokens.at(m_index);
	}
	throw std::out_of_range("Advance offset out of range");
}

std::vector<Token> Parser::get_tokens() const
{
	return m_tokens; 
}

Program Parser::get_program() const 
{
	return m_program; 
}
