#include <stdexcept>
#include <variant>
#include "generator.hpp"

Generator::Generator(const Program &program) : m_program(program) {}

void Generator::gen_expr(const Expr& expr) {
    if (std::holds_alternative<IntExpr>(expr.m_value)) {
        m_output << "   mov rax, " << std::get<IntExpr>(expr.m_value).m_value << "\n";
        push("rax");
    } else if (std::holds_alternative<IdentExpr>(expr.m_value)) {
        if (existing_vars.contains(std::get<IdentExpr>(expr.m_value).m_ident)) {
            auto var = existing_vars.at(std::get<IdentExpr>(expr.m_value).m_ident);
            std::stringstream offset{};
            offset << "QWORD [rsp + " << (m_stack_size - var.m_stack_loc - 1) * 8 << "]";
            push(offset.str());
        } else {
            throw std::runtime_error("No such variable exists");
        }
    } else if (std::holds_alternative<BinOpExpr>(expr.m_value)) {
        if (std::get<BinOpExpr>(expr.m_value).m_op == "+") {
            gen_expr(*std::get<BinOpExpr>(expr.m_value).m_lhs);
            gen_expr(*std::get<BinOpExpr>(expr.m_value).m_rhs);
            pop("rbx");
            pop("rax");
            m_output << "   add rax, rbx\n";
            push("rax");
        }
        else if (std::get<BinOpExpr>(expr.m_value).m_op == "-") {
            gen_expr(*std::get<BinOpExpr>(expr.m_value).m_lhs);
            gen_expr(*std::get<BinOpExpr>(expr.m_value).m_rhs);
            pop("rbx");
            pop("rax");
            m_output << "   sub rax, rbx\n";
            push("rax");
        }
        else if (std::get<BinOpExpr>(expr.m_value).m_op == "*") {
            gen_expr(*std::get<BinOpExpr>(expr.m_value).m_lhs);
            gen_expr(*std::get<BinOpExpr>(expr.m_value).m_rhs);
            pop("rbx");
            pop("rax");
            m_output << "   mul rbx\n";
            push("rax");
        }
        else if (std::get<BinOpExpr>(expr.m_value).m_op == "/") {
            gen_expr(*std::get<BinOpExpr>(expr.m_value).m_lhs);
            gen_expr(*std::get<BinOpExpr>(expr.m_value).m_rhs);
            pop("rbx");
            pop("rax");
            m_output << "   div rbx\n";
            push("rax");
        }
    } else {
        throw std::runtime_error("No valid expression could be generated");
    }
}

void Generator::gen_stmt(const Stmt& stmt) {
    if (std::holds_alternative<ReturnStmt>(stmt.m_stmt)) {
        gen_expr(*std::get<ReturnStmt>(stmt.m_stmt).m_expr);
        m_output << "   ret\n";
    } else if (std::holds_alternative<VariableStmt>(stmt.m_stmt)) {
        if (!existing_vars.contains(std::get<VariableStmt>(stmt.m_stmt).m_name)) {
            existing_vars.insert({std::get<VariableStmt>(stmt.m_stmt).m_name, Variable{.m_stack_loc = m_stack_size}});
            gen_expr(*std::get<VariableStmt>(stmt.m_stmt).m_expr);
        } else {
            throw std::runtime_error("Variable with the same name already exists");
        }
    } else if (std::holds_alternative<EscapeStmt>(stmt.m_stmt)) {
        gen_expr(*std::get<EscapeStmt>(stmt.m_stmt).m_expr);
        m_output << "   mov rax, 60\n";
        pop("rdi");
        m_output << "   syscall\n";
    } else if (std::holds_alternative<LabelStmt>(stmt.m_stmt)) {
        m_output << "   " << std::get<LabelStmt>(stmt.m_stmt).m_name << ":\n";
    } else if (std::holds_alternative<GoStmt>(stmt.m_stmt)) {
        m_output << "   jmp " << std::get<GoStmt>(stmt.m_stmt).m_dest << "\n";
    }
}

void Generator::gen_func_decl(const FuncDecl& func_decl) {
    m_output << "global " << func_decl.m_name << "\n";
    m_output << func_decl.m_name << ":\n";
    for (const auto& item : func_decl.m_body) {
        gen_stmt(Stmt(item.m_stmt));
    }
}

void Generator::gen_program() {
    for (const auto& item : m_program.m_body) {
        if (std::holds_alternative<FuncDecl>(item)) {
            gen_func_decl(std::get<FuncDecl>(item));
        } else {
            throw std::runtime_error("Program body variant type not found");
        }
    }
    m_output << "global _start\n";
    m_output << "_start:\n";
    m_output << "   call main\n";
    m_output << "   mov rax, 60\n";
    m_output << "   mov rdi, 0\n";
    m_output << "   syscall\n";
}

std::string Generator::get_output_str() const {
    return m_output.str();
}

void Generator::push(const std::string& reg) {
    m_output << "   push " << reg << "\n";
    m_stack_size++;
}

void Generator::pop(const std::string &reg) {
    m_output << "   pop " << reg << "\n";
    m_stack_size--;
}
