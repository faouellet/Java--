#ifndef STATEMENTS_H
#define STATEMENTS_H

#include <vector>

#include "ast.h"

class Assignment : public Statement
{
public:
	Assignment();
	virtual ~Assignment();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
};

class IfStatement : public Statement
{
private:
	Expression* m_Cond;
	Expression* m_Then;
	Expression* m_Else;

public:
	IfStatement();
	virtual ~IfStatement();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
};

class ProcedureCall : public Statement
{
private:
	std::string m_FuncName;
	std::vector<Expression*> m_Args;

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
