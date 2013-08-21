#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#include "ast.h"

class Declaration : public Node
{
public:
	Declaration();
	virtual ~Declaration();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG) = 0;
};

class FunctionDeclaration : public Declaration
{
public:
	FunctionDeclaration();
	virtual ~FunctionDeclaration();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
};

class VariableDeclaration : public Declaration
{
public:
	VariableDeclaration();
	virtual ~VariableDeclaration();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
};

#endif //DECLARATIONS_H
