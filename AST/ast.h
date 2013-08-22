// Deals with the basic nodes

#ifndef AST_H
#define AST_H

#include <memory>

#include "../CodeGen/codegen.h"

class Node
{
public:
	Node();
	virtual ~Node() { }
	virtual void Accept(std::unique_ptr<CodeGen> in_CG) = 0;
};

// Categories of nodes

class Declaration : public Node
{
public:
	Declaration();
	virtual ~Declaration();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG) = 0;
};

class Expression
{
public:
	Expression();
	virtual ~Expression();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG) = 0;
};

class Statement : public Node
{
public:
	Statement();
	virtual ~Statement();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG) = 0;
};

#endif // AST_H
