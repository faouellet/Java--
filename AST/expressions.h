#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "ast.h"

class BooleanConstant : public Expression
{
private:
	bool m_Val;

public:
	BooleanConstant();
	virtual ~BooleanConstant();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG) { in_CG->Visit(this); }
	bool GetValue() const { return m_Val; }
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
	virtual void Accept(std::unique_ptr<CodeGen> in_CG) { in_CG->Visit(this); }
	int GetValue() const { return m_Val; }
};

class DoubleConstant : public NumericConst
{
private:
	double m_Val;

public:
	DoubleConstant();
	virtual ~DoubleConstant();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG) { in_CG->Visit(this); }
	double GetValue() const { return m_Val; }
};

class NullConstant : public Expression
{
public:
	NullConstant();
	virtual ~NullConstant();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG) { in_CG->Visit(this); }
};

class LeftValue : public Expression
{
public:
	LeftValue();
	virtual ~LeftValue();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG) { in_CG->Visit(this); }
};

class RightValue : public Expression
{
public:
	RightValue();
	virtual ~RightValue();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG) { in_CG->Visit(this); }
};

// TODO : I'm confused concerning the difference between FunctionCall & ProcedureCall

class ArrayCreator : public Expression
{
public:
	ArrayCreator();
	virtual ~ArrayCreator();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG) { in_CG->Visit(this); }
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
	virtual void Accept(std::unique_ptr<CodeGen> in_CG) { in_CG->Visit(this); }
	char GetOp() const { return m_Op; }
	Expression* GetLHS() const { return m_LHS; }
	Expression* GetRHS() const { return m_RHS; }
};

class UnaryExpression : public Expression
{
private:
	char m_Op;
	Expression* m_Expr;

public:
	UnaryExpression();
	virtual ~UnaryExpression();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG) { in_CG->Visit(this); }
	char GetOp() const { return m_Op; }
	Expression* GetExpr() const { return m_Expr; }
};

#endif // EXPRESSIONS_H
