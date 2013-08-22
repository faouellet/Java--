#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#include "ast.h"

class FunctionDeclaration : public Declaration
{
public:
	FunctionDeclaration();
	virtual ~FunctionDeclaration();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
};

class VariableDeclaration : public Declaration
{
private:
	std::string m_Name;

public:
	VariableDeclaration();
	virtual ~VariableDeclaration();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
};

#endif //DECLARATIONS_H
