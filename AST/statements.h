#ifndef STATEMENTS_H
#define STATEMENTS_H

#include <vector>

#include "ast.h"

class Assignment : public Statement
{
public:
	Assignment();
	virtual ~Assignment();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG) { in_CG->Visit(this); }
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
	virtual void Accept(std::unique_ptr<CodeGen> in_CG) { in_CG->Visit(this); }
	Expression* GetCond() const { return m_Cond; }
	Expression* GetThen() const { return m_Then; }
	Expression* GetElse() const { return m_Else; }
};

class ProcedureCall : public Statement
{
private:
	std::string m_FuncName;
	std::vector<Expression*> m_Args;

public:
	ProcedureCall();
	virtual ~ProcedureCall();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG) { in_CG->Visit(this); }
	std::string GetName() const { return m_FuncName; }
	std::vector<Expression*> GetCond() const { return m_Args; }

};

class ReturnStatement : public Statement
{
public:
	ReturnStatement();
	virtual ~ReturnStatement();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG) { in_CG->Visit(this); }
};

class WhileStatement : public Statement
{
private:
	Expression* m_Cond;
	Expression* m_Body;

public:
	WhileStatement();
	virtual ~WhileStatement();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG) { in_CG->Visit(this); }
	Expression* GetCond() const { return m_Cond; }
	Expression* GetBody() const { return m_Body; }
};

#endif // STATEMENTS_H
