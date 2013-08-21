#ifndef TYPES_H
#define TYPES_H

#include "ast.h"

// Should suffice for the primitive types
class Type : public Node
{
public:
	Type();
	virtual ~Type();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG) = 0;
};

class ArrayType : public Type
{
public:
	ArrayType();
	virtual ~ArrayType();
	virtual void Accept(std::unique_ptr<CodeGen> in_CG);
}

#endif // TYPES_H
