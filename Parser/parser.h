#ifndef PARSER_H
#define PARSER_H

#include <list>
using std::list;

#include "../AST/ast.h"
#include "../AST/declarations.h"
#include "../AST/expressions.h"
#include "../AST/statements.h"
#include "../AST/types.h"
#include "../Lexer/lexer.h"

#ifndef YYBISON
#include "y.tab.h"
#endif

int yyparse();

#endif // PARSER_H
