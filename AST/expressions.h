#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "ast.h"

class Expression
{
public:
	Expression();
	virtual ~Expression();
	virtual void Emit() = 0;
};

class IntConstant : public Expression
{
public:
	IntConstant();
	virtual ~IntConstant();
	virtual void Emit();
};

class EmptyExpression : public Expression
{
public:
	EmptyExpression();
	virtual ~EmptyExpression();
	virtual void Emit();
};

class DoubleConstant : public Expression
{
public:
	DoubleConstant();
	virtual ~DoubleConstant();
	virtual void Emit();
};

class Operator : public Expression
{
public:
	Operator();
	virtual ~Operator();
	virtual void Emit();
};

class CompoundExpression : public Expression
{
public:
	CompoundExpression();
	virtual ~CompoundExpression();
	virtual void Emit();
};

class ArithmeticExpression : public Expression
{
public:
	ArithmeticExpression();
	virtual ~ArithmeticExpression();
	virtual void Emit();
};

class FunctionCall : public Expression
{
public:
	FunctionCall();
	virtual ~FunctionCall();
	virtual void Emit();
};

#endif // EXPRESSIONS_H
