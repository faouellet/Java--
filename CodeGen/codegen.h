#ifndef CODEGEN_H
#define CODEGEN_H

#include <map>
#include <string>

#include <llvm\IR\DerivedTypes.h>
#include <llvm\IR\IRBuilder.h>
#include <llvm\IR\LLVMContext.h>  // This might need to be included somewhere else
#include <llvm\IR\Module.h>       // This might need to be included somewhere else

// Expressions
class BooleanConstant;
class DoubleConstant;
class IntConstant;
class NullConstant;
class LeftValue;
class RightValue;
class ArrayCreator;
class BinaryExpression;
class UnaryExpression;

// Statements
class Assignment;
class IfStatement;
class ProcedureCall;
class ReturnStatement;
class WhileStatement;

// Declarations
class FunctionDeclaration;
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
	std::map<std::string, llvm::Value*> m_SymbolTable;

public:
	CodeGen();
	~CodeGen();

public:
	llvm::Value* Visit(const BooleanConstant* in_BConst);
	llvm::Value* Visit(const DoubleConstant* in_DConst);
	llvm::Value* Visit(const IntConstant* in_IConst);
	llvm::Value* Visit(const NullConstant* in_NConst);
	llvm::Value* Visit(const LeftValue* in_LVal);
	llvm::Value* Visit(const RightValue* in_RVal);
	llvm::Value* Visit(const ArrayCreator* in_ArrCtor);
	llvm::Value* Visit(const BinaryExpression* in_BinExpr);
	llvm::Value* Visit(const UnaryExpression* in_UExpr);
	
public:
	llvm::Value* Visit(const Assignment* in_Assign);
	llvm::Value* Visit(const IfStatement* in_RetStmt);
	llvm::Value* Visit(const ProcedureCall* in_ProcCall);
	llvm::Value* Visit(const ReturnStatement* in_RetStmt);
	llvm::Value* Visit(const WhileStatement* in_WhileStmt);

public:
	llvm::Value* Visit(const FunctionDeclaration* in_FuncDecl);
	llvm::Value* Visit(const VariableDeclaration* in_VarDecl);
};

#endif // CODEGEN_H
