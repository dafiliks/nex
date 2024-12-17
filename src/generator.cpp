#include <optional>

#include "generator.hpp"

Generator::Generator(const Program& c_program) : m_program(c_program) {}

void Generator::gen_expr(const Expr& c_expr)
{
    if (std::holds_alternative<Return>(c_expr.m_expr_v))
    {
        m_output += "   movl    $" + std::to_string(std::get<Return>(c_expr.m_expr_v).m_constant) + ", %eax\n";
        m_output += "   ret\n";
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
        std::visit([this](auto&& arg)
            {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, FuncDecl>)
                    this->gen_func_decl(arg);
                else if constexpr (std::is_same_v<T, Stmt>)
                    this->gen_stmt(arg);
            }, item);
    }
	return m_output;
}
