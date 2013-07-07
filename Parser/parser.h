#ifndef PARSER_H
#define PARSER_H

#ifndef YYBISON
#include "y.tab.h"
#endif

int yyparse();

#endif // PARSER_H
