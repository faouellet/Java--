#ifndef STATEMENTS_H
#define STATEMENTS_H

#include "ast.h"

class Statement : public Node
{
public:
	Statement();
	virtual ~Statement();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG) = 0;
};

class Assignment : public Statement
{
public:
	Assignment();
	virtual ~Assignment();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
};

class ProcedureCall : public Statement
{
public:
	ProcedureCall();
	virtual ~ProcedureCall();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
};

class ReturnStatement : public Statement
{
public:
	ReturnStatement();
	virtual ~ReturnStatement();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
};

class WhileStatement : public Statement
{
public:
	WhileStatement();
	virtual ~WhileStatement();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
};

#endif // STATEMENTS_H
