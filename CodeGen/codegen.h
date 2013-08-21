#ifndef CODEGEN_H
#define CODEGEN_H

#include <llvm\IR\IRBuilder.h>
#include <llvm\IR\LLVMContext.h>
#include <llvm\IR\Module.h>   // <-- Including this here, does it limits me in the number of modules that I can produce ??

class DoubleConstant;
class EmptyExpression;
class FunctionDeclaration;
class Identifier;
class IntConstant;
class ReturnStatement;
class VariableDeclaration;

/*
* CodeGen
* Implements a visitor that will emit LLVM IR for each node produced by the parser
*/
class CodeGen
{
private:
	llvm::Module* m_Module;
	llvm::IRBuilder<> m_Builder;
	
public:
	CodeGen();
	~CodeGen();

public:
	void Visit(const DoubleConstant* in_DConst);
	void Visit(const EmptyExpression* in_EmptyExpr);
	void Visit(const FunctionDeclaration* in_FuncDecl);
	void Visit(const Identifier* in_Id);
	void Visit(const IntConstant* in_IConst);
	void Visit(const ReturnStatement* in_RetStmt);
	void Visit(const VariableDeclaration* in_VarDecl);
};

#endif // CODEGEN_H
