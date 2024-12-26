#include <memory>
#include <optional>
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"
#include "generator.hpp"
#include "../frontend/tokenizer.hpp"

Generator::Generator(const Program& program) : 
m_program(program),
m_context(std::make_unique<llvm::LLVMContext>()),
m_module(std::make_unique<llvm::Module>("module", *m_context.get())),
m_builder(std::make_unique<llvm::IRBuilder<>>(*m_context.get())) {}

llvm::Value* Generator::gen_expr(const Expr& expr) {
    if (std::holds_alternative<Return>(expr.m_expr_value)) {
        auto return_expr{std::get<Return>(expr.m_expr_value)};
        return m_builder->CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(*m_context.get()), return_expr.m_constant));
    }
}

void Generator::gen_stmt(const Stmt& stmt) {
    gen_expr(stmt.m_expr); 
}

void Generator::gen_func_decl(const FuncDecl& func_decl) {
    llvm::FunctionType* func_type = llvm::FunctionType::get(llvm::Type::getInt32Ty(*m_context), false);
    llvm::Function* function = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, func_decl.m_name, m_module.get());
    llvm::BasicBlock* entry = llvm::BasicBlock::Create(*m_context, "entry", function);
    m_builder->SetInsertPoint(entry);
    for (const auto& stmt : func_decl.m_body) {
        gen_stmt(stmt);
    }
}

std::string Generator::gen_program() {
    for (const auto& item : m_program.m_body) {
        if (std::holds_alternative<FuncDecl>(item)) {
            gen_func_decl(std::get<FuncDecl>(item));
        } else if (std::holds_alternative<Stmt>(item)) {
            gen_stmt(std::get<Stmt>(item));
        }
    }
    std::string ir{};
    llvm::raw_string_ostream ir_stream{ir};
    m_module->print(ir_stream, nullptr);
    return ir_stream.str();
}