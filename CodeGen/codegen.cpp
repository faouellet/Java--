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
	llvm::Value* l_LVal = Visit(in_BinExpr->m_LHS());
	llvm::Value* l_RVal = Visit(in_BinExpr->GetRHS());

	if(l_LVal == nullptr || l_RVal == nullptr)
		return nullptr;

	// TODO : Type checking should be done here ??
	switch (in_BinExpr->GetOp())
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
	Value* l_Val = Visit(in_UExpr->GetExpr());

	switch (in_UExpr->GetOp())
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

llvm::Value* CodeGen::Visit(const IfStatement* in_IfStmt)
{
	// TODO : Document this. Especially the phi node sorcery.
	llvm::Value* l_CondVal = Visit(in_IfStmt->GetCond());
	if(l_CondVal == nullptr)
		return nullptr;

	l_CondVal = m_Builder.CreateFCmpONE(l_CondVal, llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(0.0)));

	llvm::Function* l_Func = m_Builder.GetInsertBlock()->getParent();

	llvm::BasicBlock* l_ThenBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "then", l_Func);
	llvm::BasicBlock* l_ElseBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "else");
	llvm::BasicBlock* l_MergeBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "ifcont");

	m_Builder.CreateCondBr(l_CondVal, l_ThenBlock, l_ElseBlock);

	m_Builder.SetInsertPoint(l_ThenBlock);

	llvm::Value* l_ThenVal = Visit(in_IfStmt->GetThen());
	if(l_ThenVal == nullptr)
		return nullptr;
	
	m_Builder.CreateBr(l_MergeBlock);

	l_ThenBlock = m_Builder.GetInsertBlock();

	l_Func->getBasicBlockList().push_back(l_ElseBlock);
	m_Builder.SetInsertPoint(l_ElseBlock);

	llvm::Value* l_ElseVal = Visit(in_IfStmt->GetElse());
	if(l_ElseVal == nullptr)
		return nullptr;

	m_Builder.CreateBr(l_MergeBlock);
	l_ElseBlock = m_Builder.GetInsertBlock();

	l_Func->getBasicBlockList().push_back(l_MergeBlock);
	m_Builder.SetInsertPoint(l_MergeBlock);
	
	llvm::PHINode *l_PHI = m_Builder.CreatePHI(llvm::Type::getDoubleTy(llvm::getGlobalContext()), 2, "iftmp");
	l_PHI->addIncoming(l_ThenVal, l_ThenBlock);
	l_PHI->addIncoming(l_ElseVal, l_ElseBlock);

	return l_PHI;
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
