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

class Identifier : public Node
{
public:
	Identifier();
	virtual ~Identifier();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
};

#endif // AST_H
