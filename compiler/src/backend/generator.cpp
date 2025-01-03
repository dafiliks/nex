#include <stdexcept>
#include <unordered_map>
#include "generator.hpp"

Generator::Generator(const Program &program) : m_program(program) {}

void Generator::gen_term(const TermExpr& term) {
    struct TermVisitor {
        Generator& m_gen;
        void operator()(IntExpr int_expr) {
            m_gen.m_output << "   mov rax, " << int_expr.m_value << "\n";
            m_gen.push("rax");
        }
        void operator()(IdentExpr ident_expr) {
            if (existing_vars.contains(ident_expr.m_ident)) {
                auto var{existing_vars.at(ident_expr.m_ident)};
                std::stringstream reg{};
                reg << "QWORD [rsp + " << (m_gen.m_stack_size - var.m_stack_loc - 1) * 8 << "]";
                m_gen.push(reg.str());
            } else {
                m_gen.gen_rt_error("Variable undefined");
            }
        }
    };
    TermVisitor visitor{*this};
    std::visit(visitor, term.m_term);
}

void Generator::gen_expr(const Expr& expr) {
    struct ExprVisitor {
        Generator& m_gen;
        void operator()(TermExpr term_expr) {
            m_gen.gen_term(term_expr);
        }
        void operator()(BinOpExpr bin_op_expr) {
            if (bin_op_expr.m_op == "+") {
                m_gen.gen_expr(*bin_op_expr.m_lhs);
                m_gen.gen_expr(*bin_op_expr.m_rhs);
                m_gen.pop("rbx");
                m_gen.pop("rax");
                m_gen.m_output << "   add rax, rbx\n";
                m_gen.push("rax");
            }
            else if (bin_op_expr.m_op == "-") {
                m_gen.gen_expr(*bin_op_expr.m_lhs);
                m_gen.gen_expr(*bin_op_expr.m_rhs);
                m_gen.pop("rbx");
                m_gen.pop("rax");
                m_gen.m_output << "   sub rax, rbx\n";
                m_gen.push("rax");
            }
            else if (bin_op_expr.m_op == "*") {
                m_gen.gen_expr(*bin_op_expr.m_lhs);
                m_gen.gen_expr(*bin_op_expr.m_rhs);
                m_gen.pop("rbx");
                m_gen.pop("rax");
                m_gen.m_output << "   mul rbx\n";
                m_gen.push("rax");
            }
            else if (bin_op_expr.m_op == "/") {
                m_gen.gen_expr(*bin_op_expr.m_lhs);
                m_gen.gen_expr(*bin_op_expr.m_rhs);
                m_gen.pop("rbx");
                m_gen.pop("rax");
                m_gen.m_output << "   div rbx\n";
                m_gen.push("rax");
            }
        }
    };
    ExprVisitor visitor{*this};
    std::visit(visitor, expr.m_value);
}

void Generator::gen_stmt(const Stmt& stmt) {
    struct StmtVisitor {
        Generator& m_gen;
        void operator()(ReturnStmt return_stmt) {
            m_gen.gen_expr(*return_stmt.m_expr);
            m_gen.m_output << "   ret\n";
        }
        void operator()(VariableStmt var_stmt) {
            if (!existing_vars.contains(var_stmt.m_name)) {
                existing_vars.insert({var_stmt.m_name, Variable{.m_stack_loc = m_gen.m_stack_size}});
                m_gen.gen_expr(*var_stmt.m_expr);
            } else {
                m_gen.gen_rt_error("Variable exists elsewhere");
            }
        }
        void operator()(EscapeStmt esc_stmt) {
            m_gen.gen_expr(*esc_stmt.m_expr);
            m_gen.m_output << "   mov rax, 60\n";
            m_gen.pop("rdi");
            m_gen.m_output << "   syscall\n";
        }
        void operator()(LabelStmt label_stmt) {
            if (!existing_labels.contains(label_stmt.m_name)) {
                existing_labels.insert({label_stmt.m_name, label_stmt});
                m_gen.m_output << "   " << label_stmt.m_name << ":\n";
            } else {
                m_gen.gen_rt_error("Label exists elsewhere");
            }
        }
        void operator()(GoStmt go_stmt) {
            if (existing_labels.contains(go_stmt.m_dest)) {
                m_gen.m_output << "   jmp " << go_stmt.m_dest << "\n";
            } else {
                m_gen.gen_rt_error("Label undefined");
            }
        }
    };
    StmtVisitor visitor{*this};
    std::visit(visitor, stmt.m_stmt);
}

void Generator::gen_scope(const Scope& scope) {
    for (const auto& stmt : scope.m_body) {
        gen_stmt(Stmt{stmt.m_stmt});
    }
}

void Generator::gen_func_decl(const FuncDecl& func_decl) {
    if (!existing_funcs.contains(func_decl.m_name)) {
        existing_funcs.insert({func_decl.m_name, func_decl});
        m_output << "global " << func_decl.m_name << "\n";
        m_output << func_decl.m_name << ":\n";
        gen_scope(func_decl.m_scope);
    } else {
        gen_rt_error("Function exists elsewhere");
    }
}

void Generator::gen_program() {
    for (const auto& item : m_program.m_body) {
        gen_func_decl(item);
    }
    m_output << "global _start\n";
    m_output << "_start:\n";
    m_output << "   call main\n";
    m_output << "   mov rax, 60\n";
    m_output << "   mov rdi, 0\n";
    m_output << "   syscall\n";
}

void Generator::push(const std::string& reg) {
    m_output << "   push " << reg << "\n";
    m_stack_size++;
}

void Generator::pop(const std::string &reg) {
    m_output << "   pop " << reg << "\n";
    m_stack_size--;
}

void Generator::gen_rt_error(const std::string& err_message) const {
    throw std::runtime_error("Nex: Generator -> " + err_message);
}

std::string Generator::get_output_str() const {
    return m_output.str();
}
