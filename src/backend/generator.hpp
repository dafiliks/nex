#ifndef GENERATOR_HPP
#define GENERATOR_HPP

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
#include "llvm/IR/Verifier.h"
#include "../frontend/ast.hpp"

class Generator
{
public:
	Generator(const Program& program);
	llvm::Value* gen_expr(const Expr& expr);
	void gen_stmt(const Stmt& stmt);
	void gen_func_decl(const FuncDecl& func_decl);
	std::string gen_program();
private:
    std::unique_ptr<llvm::LLVMContext> m_context{};
    std::unique_ptr<llvm::Module> m_module{};
    std::unique_ptr<llvm::IRBuilder<>> m_builder{};
	Program m_program{};
	std::string m_output{};
};

#endif