// generator.cpp
// Copyright (C) David Filiks

#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include "generator.hpp"
#include "../frontend/tokenizer.hpp"

Generator::Generator(const Program &program) : m_program(program) {}

void Generator::gen_term(const TermExpr& term) {
    struct TermVisitor {
        Generator& m_gen;
        void operator()(IntExpr int_expr) {
            m_gen.m_output << "    mov rax, " << int_expr.m_value << "\n";
            m_gen.push("rax");
        }
        void operator()(IdentExpr ident_expr) {
            if (existing_vars.contains(ident_expr.m_ident)) {
                auto var{existing_vars.at(ident_expr.m_ident)};
                std::stringstream reg{};
                reg << "QWORD [rsp + " << (m_gen.m_stack_size - var.m_stack_loc - 1) * 8 << "]";
                m_gen.push(reg.str());
            } else {
                m_gen.gen_error("Variable undefined");
            }
        }
        void operator()(ArrayExpr arr_expr) {
            if (existing_arrs.contains(arr_expr.m_ident)) {
                auto var{existing_arrs.at(arr_expr.m_ident)};
                std::stringstream reg{};
                m_gen.gen_expr(*arr_expr.m_index);
                m_gen.pop("rax");
                m_gen.m_output << "    mov rbx, " << m_gen.m_stack_size << "\n";
                m_gen.m_output << "    mov rbp, " << var.m_arr_head_loc << "\n";
                m_gen.m_output << "    sub rbx, rbp\n"; // using a pointer for math... gorilla mode...
                m_gen.m_output << "    add rbx, rax\n";
                m_gen.m_output << "    sub rbx, 1\n";
                m_gen.m_output << "    mov rax, 8\n";
                m_gen.m_output << "    mul rbx\n";
                m_gen.m_output << "    mov rbx, rax\n";
                reg << "QWORD [rsp + rbx]";
                m_gen.push(reg.str());
            } else {
                m_gen.gen_error("Array undefined");
            }
        }
        void operator()(InExpr in_expr) {
            m_gen.m_output << "    mov rax, 0\n";
            m_gen.m_output << "    sub rsp, 8\n";
            m_gen.m_output << "    lea rsi, [rsp]\n";
            m_gen.m_output << "    mov rdx, 1\n";
            m_gen.m_output << "    syscall\n";
            m_gen.m_output << "    movzx rax, byte [rsp]\n";
            m_gen.push("rax");
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
        // switch case
        void operator()(BinOpExpr bin_op_expr) {
            if (bin_op_expr.m_op == "+") {
                m_gen.gen_expr(*bin_op_expr.m_lhs);
                m_gen.gen_expr(*bin_op_expr.m_rhs);
                m_gen.pop("rbx");
                m_gen.pop("rax");
                m_gen.m_output << "    add rax, rbx\n";
                m_gen.push("rax");
            }
            else if (bin_op_expr.m_op == "-") {
                m_gen.gen_expr(*bin_op_expr.m_lhs);
                m_gen.gen_expr(*bin_op_expr.m_rhs);
                m_gen.pop("rbx");
                m_gen.pop("rax");
                m_gen.m_output << "    sub rax, rbx\n";
                m_gen.push("rax");
            }
            else if (bin_op_expr.m_op == "*") {
                m_gen.gen_expr(*bin_op_expr.m_lhs);
                m_gen.gen_expr(*bin_op_expr.m_rhs);
                m_gen.pop("rbx");
                m_gen.pop("rax");
                m_gen.m_output << "    mul rbx\n";
                m_gen.push("rax");
            }
            else if (bin_op_expr.m_op == "/") {
                m_gen.gen_expr(*bin_op_expr.m_lhs);
                m_gen.gen_expr(*bin_op_expr.m_rhs);
                m_gen.pop("rbx");
                m_gen.pop("rax");
                m_gen.m_output << "    div rbx\n";
                m_gen.push("rax");
            }
        }
    };
    ExprVisitor visitor{*this};
    std::visit(visitor, expr.m_value);
}

void Generator::gen_scope(const Scope& scope) {
    for (std::size_t i = 0; i < scope.m_body.size(); i++) {
        gen_stmt(scope.m_body.at(i));
    }
}

void Generator::gen_stmt(const Stmt& stmt) {
    struct StmtVisitor {
        Generator& m_gen;
        void operator()(VariableStmt var_stmt) {
            if (!existing_vars.contains(var_stmt.m_name) && !existing_arrs.contains(var_stmt.m_name)
                && !nex_keywords.contains(var_stmt.m_name)) {
                VariableStmt new_var{};
                new_var.m_stack_loc = m_gen.m_stack_size;
                existing_vars.insert({var_stmt.m_name, new_var});
                if (var_stmt.m_expr == nullptr) {
                    TermExpr term_expr{};
                    IntExpr int_expr{};
                    int_expr.m_value = 0;
                    term_expr.m_term = int_expr;
                    m_gen.gen_expr(*std::make_shared<Expr>(term_expr));
                } else {
                    m_gen.gen_expr(*var_stmt.m_expr);
                }
            } else {
                m_gen.gen_error("Container exists elsewhere or use of reserved keyword as identifier");
            }
        }
        void operator()(ArrayStmt arr_stmt) {
            if (!existing_arrs.contains(arr_stmt.m_name) && !existing_vars.contains(arr_stmt.m_name)) {
                arr_stmt.m_arr_head_loc = m_gen.m_stack_size;
                existing_arrs.insert({arr_stmt.m_name, arr_stmt});
                m_gen.m_output << "    mov rcx, " << arr_stmt.m_arr_size << "\n";
                m_gen.m_output << "__loop" << m_gen.m_arr_count << ":\n";
                m_gen.push("rax");
                m_gen.m_output << "    loop __loop" << m_gen.m_arr_count << "\n";
                m_gen.m_stack_size += arr_stmt.m_arr_size - 1;
                m_gen.m_arr_count++;
            } else {
                m_gen.gen_error("Container exists elsewhere");
            }
        }
        void operator()(IdentStmt ident_stmt)
        {
            if (existing_vars.contains(ident_stmt.m_dest)) {
                auto var{existing_vars.at(ident_stmt.m_dest)};
                m_gen.gen_expr(*ident_stmt.m_expr);
                m_gen.pop("rax");
                m_gen.m_output << "    mov QWORD [rsp + " << (m_gen.m_stack_size - var.m_stack_loc - 1) * 8 << "], rax\n";
            } else if (existing_arrs.contains(ident_stmt.m_dest)) {
                auto var{existing_arrs.at(ident_stmt.m_dest)};
                m_gen.m_output << "    mov rcx, " << m_gen.m_stack_size << "\n";
                m_gen.m_output << "    mov rbx, " << var.m_arr_head_loc << "\n";
                m_gen.m_output << "    sub rcx, rbx\n";
                m_gen.gen_expr(*ident_stmt.m_index);
                m_gen.pop("rax");
                m_gen.m_output << "    mov rbx, rax\n";
                m_gen.m_output << "    add rcx, rbx\n";
                m_gen.m_output << "    sub rcx, 1\n";
                m_gen.m_output << "    mov rax, 8\n";
                m_gen.m_output << "    mul rcx\n";
                m_gen.m_output << "    mov rcx, rax\n";
                m_gen.gen_expr(*ident_stmt.m_expr);
                m_gen.pop("rax");
                m_gen.m_output << "    mov QWORD [rsp + rcx], rax\n";
            } else {
                m_gen.gen_error("Variable specified does not exist");
            }
        }
        void operator()(ExitStmt ex_stmt) {
            m_gen.gen_expr(*ex_stmt.m_expr);
            m_gen.m_output << "    mov rax, 60\n";
            m_gen.pop("rdi");
            m_gen.m_output << "    syscall\n";
        }
        void operator()(LabelStmt label_stmt) {
            if (label_stmt.m_name.at(0) == '_' && label_stmt.m_name.at(1) == '_') {
                m_gen.gen_error("Labels starting with \"__\" are reserved");
            }
            if (!existing_labels.contains(label_stmt.m_name)) {
                existing_labels.insert({label_stmt.m_name, label_stmt});
                m_gen.m_output << label_stmt.m_name << ":\n";
            } else {
                m_gen.gen_error("Label exists elsewhere");
            }
        }
        void operator()(GoStmt go_stmt) {
            m_gen.m_output << "    jmp " << go_stmt.m_dest << "\n";
        }
        void operator()(OutStmt out_stmt) {
            IdentExpr ident_expr{};
            ident_expr.m_ident = out_stmt.m_output;
            if (existing_vars.contains(ident_expr.m_ident)) {
                auto var{existing_vars.at(ident_expr.m_ident)};
                m_gen.gen_term(TermExpr{ident_expr});
                m_gen.pop("rax");
                m_gen.m_output << "    mov rdi, 1\n";
                m_gen.m_output << "    lea rsi, [rsp]\n";
                m_gen.m_output << "    mov [rsp], al\n";
                m_gen.m_output << "    mov rdx, 1\n";
                m_gen.m_output << "    mov rax, 0x1\n";
                m_gen.m_output << "    syscall\n";
            } else {
                m_gen.gen_error("Variable specified does not exist");
            }
        }
        void operator()(IfzStmt ifz_stmt) {
            m_gen.gen_expr(*ifz_stmt.m_cond);
            m_gen.pop("rax");
            m_gen.m_output << "    cmp rax, 0\n";
            m_gen.m_output << "    jz __jz" << m_gen.m_jz_count << "\n";
            if (ifz_stmt.m_else != nullptr) {
                m_gen.gen_stmt(*ifz_stmt.m_else);
            }
            m_gen.m_output << "    jmp __jz_af" << m_gen.m_jz_count << "\n";
            m_gen.m_output << "__jz" << m_gen.m_jz_count << ":\n";
            m_gen.gen_scope(*ifz_stmt.m_body);
            m_gen.m_output << "__jz_af" << m_gen.m_jz_count << ":\n";
            m_gen.m_jz_count++;
        }
        void operator()(ElseStmt else_stmt) {
            m_gen.gen_scope(*else_stmt.m_body);
        }
    };
    StmtVisitor visitor{*this};
    std::visit(visitor, stmt.m_stmt);
}

void Generator::gen_program() {
    m_output << "global _start\n";
    m_output << "_start:\n";
    for (const auto& item : m_program.m_body) {
        gen_stmt(item);
    }
    m_output << "    mov rax, 60\n";
    m_output << "    mov rdi, 0\n";
    m_output << "    syscall\n";
}

void Generator::push(const std::string& reg) {
    m_output << "    push " << reg << "\n";
    m_stack_size++;
}

void Generator::pop(const std::string &reg) {
    m_output << "    pop " << reg << "\n";
    m_stack_size--;
}

void Generator::gen_error(const std::string& err_message) const {
    std::cout << "Nex: generator error: " << err_message << "\n";
    exit(EXIT_FAILURE);
}

std::string Generator::get_output_str() const {
    return m_output.str();
}
