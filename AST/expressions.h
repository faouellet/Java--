#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "ast.h"

class Expression
{
public:
	Expression();
	virtual ~Expression();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG) = 0;
};

class IntConstant : public Expression
{
public:
	IntConstant();
	virtual ~IntConstant();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
};

class EmptyExpression : public Expression
{
public:
	EmptyExpression();
	virtual ~EmptyExpression();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
};

class DoubleConstant : public Expression
{
public:
	DoubleConstant();
	virtual ~DoubleConstant();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
};

class Operator : public Expression
{
public:
	Operator();
	virtual ~Operator();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
};

class CompoundExpression : public Expression
{
public:
	CompoundExpression();
	virtual ~CompoundExpression();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
};

class ArithmeticExpression : public Expression
{
public:
	ArithmeticExpression();
	virtual ~ArithmeticExpression();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
};

class FunctionCall : public Expression
{
public:
	FunctionCall();
	virtual ~FunctionCall();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
};

#endif // EXPRESSIONS_H
