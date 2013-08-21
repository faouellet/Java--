#include "codegen.h"

CodeGen::CodeGen() { }

CodeGen::~CodeGen() { }

void CodeGen::Visit(const DoubleConstant* in_DConst)
{
	llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat());
}

void CodeGen::Visit(const EmptyExpression* in_EmptyExpr)
{}

void CodeGen::Visit(const FunctionDeclaration* in_FuncDecl)
{}

void CodeGen::Visit(const Identifier* in_Id)
{}

void CodeGen::Visit(const IntConstant* in_IConst)
{
	llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt());
}

void CodeGen::Visit(const ReturnStatement* in_RetStmt)
{}

void CodeGen::Visit(const VariableDeclaration* in_VarDecl)
{}
