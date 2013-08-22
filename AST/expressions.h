#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "ast.h"

class BooleanConstant : public Expression
{
public:
	BooleanConstant();
	virtual ~BooleanConstant();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
};

class NumericConst : public Expression
{
public:
	NumericConst();
	virtual ~NumericConst();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG) = 0;
};

class IntConstant : public NumericConst
{
private:
	int m_Val;

public:
	IntConstant();
	virtual ~IntConstant();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
	int GetValue() const { return m_Val; }
};

class DoubleConstant : public NumericConst
{
private:
	double m_Val;

public:
	DoubleConstant();
	virtual ~DoubleConstant();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
	double GetValue() const { return m_Val; }
};

class NullConstant : public Expression
{
public:
	NullConstant();
	virtual ~NullConstant();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
};

class LeftValue : public Expression
{
public:
	LeftValue();
	virtual ~LeftValue();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
};

class RightValue : public Expression
{
public:
	RightValue();
	virtual ~RightValue();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
};

// TODO : I'm confused concerning the difference between FunctionCall & ProcedureCall

class ArrayCreator : public Expression
{
public:
	ArrayCreator();
	virtual ~ArrayCreator();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
};

class BinaryExpression : public Expression
{
private:
	char m_Op;
	Expression* m_LHS;
	Expression* m_RHS;

public:
	BinaryExpression();
	virtual ~BinaryExpression();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
};

class UnaryExpression : public Expression
{
private:
	char m_Op;
	Expression* m_Expr;

public:
	UnaryExpression();
	virtual ~UnaryExpression();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
};

#endif // EXPRESSIONS_H
