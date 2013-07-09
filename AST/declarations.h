#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#include "ast.h"

class Declaration : public Node
{
public:
	Declaration();
	virtual ~Declaration();
	virtual void Emit() = 0;
};

class FunctionDeclaration : public Declaration
{
public:
	FunctionDeclaration();
	virtual ~FunctionDeclaration();
	virtual void Emit();
};

class VariableDeclaration : public Declaration
{
public:
	VariableDeclaration();
	virtual ~VariableDeclaration();
	virtual void Emit();
};

#endif //DECLARATIONS_H
