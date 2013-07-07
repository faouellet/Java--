// Deals with the basic nodes

#ifndef AST_H
#define AST_H

struct Node
{
	virtual ~Node() { }
	virtual llvm::Value* CodeGen(CodeGenContext & io_Context) { return nullptr; }
};

#endif // AST_H
