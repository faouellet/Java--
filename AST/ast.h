// Deals with the basic nodes

#ifndef AST_H
#define AST_H

class Node
{
public:
	Node();
	virtual ~Node() { }
	virtual void Emit() = 0;
};

class Identifier : public Node
{
public:
	Identifier();
	virtual ~Identifier();
	virtual void Emit();
};

#endif // AST_H
