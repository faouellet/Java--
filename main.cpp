#include <iostream>

#include "codegen.h"
#include "node.h"

extern int yyparse();
extern NBlock* programBlock;

int main(int argc, char** argv)
{
	yyparse();
	std::cout << programBlock << std::endl;

	InitializeNativeTarget();
	CodeGenContext l_Context;
	l_Context.GenerateCode(*programBlock);
	l_Context.RunCode();
}
