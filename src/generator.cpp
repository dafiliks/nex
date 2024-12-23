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
        if (m_neg_count >= 1 && m_neg_count % 2 != 0)
        {
            m_output += "   neg     %eax\n";
            m_neg_count = 0;
        }
        if (m_dec_count >= 1)
        {
            m_output += "   subl    $" + std::to_string(m_dec_count) + ", %eax\n";
            m_dec_count = 0;
        }
        if (m_comp_count >= 1 && m_comp_count % 2 != 0)
        {
            m_output += "   notl    %eax\n";
            m_comp_count = 0;
        }
        m_output += "   ret\n";
    }
    else if (std::holds_alternative<std::shared_ptr<UnOp>>(c_expr.m_expr_v))
    {
        if (std::get<std::shared_ptr<UnOp>>(c_expr.m_expr_v)->m_op == TokenType::neg)
        {
            m_neg_count++;
        }
        else if (std::get<std::shared_ptr<UnOp>>(c_expr.m_expr_v)->m_op == TokenType::dec)
        {
            m_dec_count++;
        }
        else if (std::get<std::shared_ptr<UnOp>>(c_expr.m_expr_v)->m_op == TokenType::l_neg)
        {
            //wip
            m_output += "g: l_neg\n";
        }
        else if (std::get<std::shared_ptr<UnOp>>(c_expr.m_expr_v)->m_op == TokenType::comp)
        {
            m_comp_count++;
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

#if __linux__
    m_output += ".section .note.GNU-stack,\"\",@progbits";
#endif

	return m_output;
}
