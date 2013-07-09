#ifndef TYPES_H
#define TYPES_H

#include "ast.h"

// Should suffice for the primitive types
class Type : public Node
{
public:
	Type();
	virtual ~Type();
	virtual void Emit();
};

class ArrayType : public Type
{
public:
	ArrayType();
	virtual ~ArrayType();
	virtual void Emit();
}

#endif // TYPES_H
