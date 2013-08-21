#include "expressions.h"

////////// Expression //////////
Expression::Expression() { }

Expression::~Expression() { }


////////// EmptyExpression //////////
EmptyExpression::EmptyExpression() { }

EmptyExpression::~EmptyExpression() { }

void EmptyExpression::Accept(std::unique_ptr<CodeGen> in_CG) 
{
	in_CG->Visit(this);
}


////////// IntConstant //////////
IntConstant::IntConstant() { }

IntConstant::~IntConstant() { }

void IntConstant::Accept(std::unique_ptr<CodeGen> in_CG)
{
	in_CG->Visit(this);
}


////////// DoubleConstant //////////
DoubleConstant::DoubleConstant() { }

DoubleConstant::~DoubleConstant() { }

void DoubleConstant::Accept(std::unique_ptr<CodeGen> in_CG)
{
	in_CG->Visit(this);
}


////////// Operator //////////
Operator::Operator() { }

Operator::~Operator() { }

void Operator::Accept(std::unique_ptr<CodeGen> in_CG) { }


////////// FunctionCall //////////
FunctionCall::FunctionCall() { }

FunctionCall::~FunctionCall() { }

void FunctionCall::Accept(std::unique_ptr<CodeGen> in_CG) { }


////////// ArithmeticExpression //////////
ArithmeticExpression::ArithmeticExpression() { }

ArithmeticExpression::~ArithmeticExpression() { }

void ArithmeticExpression::Accept(std::unique_ptr<CodeGen> in_CG) { }


////////// CompoundExpression //////////
CompoundExpression::CompoundExpression() { }

CompoundExpression::~CompoundExpression() { }

void CompoundExpression::Accept(std::unique_ptr<CodeGen> in_CG) { }
