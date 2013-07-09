#ifndef STATEMENTS_H
#define STATEMENTS_H

#include "ast.h"

class Statement : public Node
{
public:
	Statement();
	virtual ~Statement();
	virtual void Emit() = 0;
};

class ReturnStatement : public Statement
{
public:
	ReturnStatement();
	virtual ~ReturnStatement();
	virtual void Emit();
};

#endif // STATEMENTS_H
