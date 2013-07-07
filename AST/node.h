#include <iostream>
#include <vector>
#include <llvm/IR/Value.h>

class CodeGenContext;
struct NStatement;
struct NExpression;
struct NVariableDeclaration;

typedef std::vector<NStatement*> StatementList;
typedef std::vector<NExpression*> ExpressionList;
typedef std::vector<NVariableDeclaration*> VariableList;



struct NExpression : public Node
{ };

struct NStatement : public Node
{ };

struct NInteger : public NExpression
{
	long long Value;

	NInteger(const long long & in_Value = 0L) : Value(in_Value) { }
	virtual llvm::Value* CodeGen(CodeGenContext & io_Context);
};

struct NDouble : public NExpression
{
	double Value;

	NDouble(const double & in_Value) : Value(in_Value) { }
	virtual llvm::Value* CodeGen(CodeGenContext & io_Context);
};

struct NIdentifier : public NExpression
{
	std::string Name;

	NIdentifier(const std::string & in_Name) : Name(in_Name) { }
	virtual llvm::Value* CodeGen(CodeGenContext & io_Context);
};

struct NMethodCall : public NExpression
{
	const NIdentifier& ID;
	ExpressionList Arguments;

	NMethodCall(const NIdentifier & in_ID, ExpressionList & in_Args) :
		ID(in_ID), Arguments(in_Args) { }
	NMethodCall(const NIdentifier & in_ID) : ID(in_ID) { }
	virtual llvm::Value* CodeGen(CodeGenContext & io_Context);
};

struct NBinaryOperator : public NExpression
{
	int Op;
	NExpression& LHS;
	NExpression& RHS;

	NBinaryOperator(NExpression & in_LHS, int in_Op, NExpression & in_RHS) :
		Op(in_Op), LHS(in_LHS), RHS(in_RHS) { }
	virtual llvm::Value* CodeGen(CodeGenContext & io_Context);
};

struct NAssignment : public NExpression
{
	NIdentifier& LHS;
	NExpression& RHS;

	NAssignment(NIdentifier & in_LHS, NExpression & in_RHS) :
		LHS(in_LHS), RHS(in_RHS) { }

	virtual llvm::Value* CodeGen(CodeGenContext & io_Context);
};

struct NBlock : public NExpression
{
	StatementList Statements;
	
	NBlock() { }
	virtual llvm::Value* CodeGen(CodeGenContext & io_Context);
};

struct NExpressionStatement : public NStatement
{
	NExpression& Expression;

	NExpressionStatement(NExpression & in_Expression) :
		Expression(in_Expression) { }
	virtual llvm::Value* CodeGen(CodeGenContext & io_Context);
};

struct NVariableDeclaration : public NStatement
{
	const NIdentifier& Type;
	NIdentifier& ID;
	NExpression* AssignmentExpr;

	NVariableDeclaration(const NIdentifier & in_Type, NIdentifier & in_ID, NExpression * in_Assignment = nullptr) :
		Type(in_Type), ID(in_ID), AssignmentExpr(in_Assignment) { }
	virtual llvm::Value* CodeGen(CodeGenContext & io_Context);
};

struct NFunctionDeclaration : NStatement
{
	const NIdentifier& Type;
	const NIdentifier& ID;
	VariableList Arguments;
	NBlock& Block;

	NFunctionDeclaration(const NIdentifier & in_Type, const NIdentifier & in_ID, const VariableList & in_Args, NBlock & in_Block) :
		Type(in_Type), ID(in_ID), Arguments(in_Args), Block(in_Block) { }
	virtual llvm::Value* CodeGen(CodeGenContext & io_Context);
};
