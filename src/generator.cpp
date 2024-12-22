#include <optional>
#include <memory>

#include "generator.hpp"
#include "tokenizer.hpp"

Generator::Generator(const Program& c_program) : m_program(c_program) {}

void Generator::gen_expr(const Expr& c_expr)
{
    if (std::holds_alternative<Return>(c_expr.m_expr_v))
    {
        m_output += "   movl    $" + std::to_string(std::get<Return>(c_expr.m_expr_v).m_constant) + ", %eax\n";
        m_output += "   ret\n";
    }
    else if (std::holds_alternative<std::shared_ptr<UnOp>>(c_expr.m_expr_v))
    {
        // std::cout << to_string(std::get<std::shared_ptr<UnOp>>(c_expr.m_expr_v)->m_op) << " vs " << to_string(TokenType::l_neg);
        if (std::get<std::shared_ptr<UnOp>>(c_expr.m_expr_v)->m_op == TokenType::neg)
        {
            m_output += "g: neg\n";
        }
        else if (std::get<std::shared_ptr<UnOp>>(c_expr.m_expr_v)->m_op == TokenType::l_neg)
        {
            m_output += "g: l_neg\n";
        }
        else if (std::get<std::shared_ptr<UnOp>>(c_expr.m_expr_v)->m_op == TokenType::b_compl)
        {
            m_output += "g: b_compl\n";
        }
        gen_expr(*std::get<std::shared_ptr<UnOp>>(c_expr.m_expr_v)->m_expr);
    }
}

void Generator::gen_stmt(const Stmt& c_stmt)
{
    gen_expr(c_stmt.m_expr);
}

void Generator::gen_func_decl(const FuncDecl& c_func_decl)
{
    m_output += ".globl _" + c_func_decl.m_name + "\n";
    m_output += "_" + c_func_decl.m_name + ":\n";
    for (std::size_t i = 0; i < c_func_decl.m_body.size(); i++)
    {
        gen_stmt(c_func_decl.m_body.at(i));
    }
}

std::string Generator::gen_program()
{
    for (const auto& item : m_program.m_body)
    {
        if (std::holds_alternative<FuncDecl>(item))
        {
            gen_func_decl(std::get<FuncDecl>(item));
        }
        else if (std::holds_alternative<Stmt>(item))
        {
            gen_stmt(std::get<Stmt>(item));
        }
    }
	return m_output;
}
