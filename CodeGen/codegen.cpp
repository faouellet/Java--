#include "codegen.h"

#include "../AST/declarations.h"
#include "../AST/expressions.h"
#include "../AST/statements.h"

#include <vector>

CodeGen::CodeGen() : m_Builder(llvm::getGlobalContext()) { }

CodeGen::~CodeGen() { }

llvm::Value* CodeGen::Visit(const BooleanConstant* in_BConst)
{

}

llvm::Value* CodeGen::Visit(const DoubleConstant* in_DConst)
{
	return llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(in_DConst->GetValue()));
}

llvm::Value* CodeGen::Visit(const IntConstant* in_IConst)
{

}

llvm::Value* CodeGen::Visit(const NullConstant* in_NConst)
{

}

llvm::Value* CodeGen::Visit(const LeftValue* in_LVal)
{

}

llvm::Value* CodeGen::Visit(const RightValue* in_RVal)
{

}

llvm::Value* CodeGen::Visit(const ArrayCreator* in_ArrCtor)
{

}

llvm::Value* CodeGen::Visit(const BinaryExpression* in_BinExpr)
{
	Value* l_LVal = Visit();
	Value* l_RVal = Visit();

	if(l_LVal == nullptr || l_RVal == nullptr)
		return nullptr;

	// TODO : Type checking should be done here ??
	switch (Op)
	{
		case '+':
			return m_Builder.CreateFAdd(l_LVal, l_RVal);
		case '-':
			return m_Builder.CreateFSub(l_LVal, l_RVal);
		case '*':
			return m_Builder.CreateFMul(l_LVal, l_RVal);
		case '/':
			return m_Builder.CreateFDiv(l_LVal, l_RVal);
		default:
			break; // TODO : Should return some kind of error
	}
}

llvm::Value* CodeGen::Visit(const UnaryExpression* in_UExpr)
{
	Value* l_Val = Visit();

	switch (Op)
	{
		case '!':
			return m_Builder.CreateNot(l_Val);
		default:
			break;
	}
}

llvm::Value* CodeGen::Visit(const Assignment* in_Assign)
{

}

llvm::Value* CodeGen::Visit(const ProcedureCall* in_ProcCall)
{
	llvm::Function* l_Callee = m_Module->getFunction(Callee);
	if(l_Callee == nullptr)
		return nullptr; // TODO : Error handling

	if(l_Callee->arg_size() != Args.size())
		return nullptr; // TODO : Error handling

	return m_Builder.CreateCall(l_Callee, ...);
}

llvm::Value* CodeGen::Visit(const ReturnStatement* in_RetStmt)
{

}

llvm::Value* CodeGen::Visit(const WhileStatement* in_WhileStmt)
{

}

llvm::Value* CodeGen::Visit(const FunctionDeclaration* in_FuncDecl)
{

}

llvm::Value* CodeGen::Visit(const VariableDeclaration* in_VarDecl)
{

}
