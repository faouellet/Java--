%{

#include <string>
#include "lexer.h"
#include "parser.h"

%}

%%             /* BEGIN RULES SECTION */

/* ---------------------- Definitions ----------------------*/
digit             ([0-9])
nonZeroDigit      ([1-9])
octalDigit        ([0-7])
hexDigit          ([0-9A-Fa-f])
integer           ({nonZeroDigit}{digit}+)
double            ({integer}"."{digit})
identifier        ([a-zA-Z][a-zA-Z_0-9]*)
operator          ([-+/*%=.,;!<>()[\]{}])

/* ----------------------- Keywords -----------------------*/
"int"                   { return T_Int;           }
"double"                { return T_Double;        }
"return"                { return T_Return;        }

/* ----------------------- Operators ----------------------*/
"=="					{ return T_Equal;         }
"!="					{ return T_NotEqual;      }
"<"						{ return T_LessThan;      }
"<="					{ return T_LessEqual;     }
">"						{ return T_GreaterThan;   }
">="					{ return T_GreaterEqual;  }
"&&"                    { return T_And;           }
"||"                    { return T_Or;            }
{operator}				{ return yytext[0];       }

/* ----------------------- Constants ----------------------*/
{integer}           { yylval.IntConstant = strtol(yytext, NULL, 10);
                         return T_IntConstant; }
{double}            { yylval.DoubleConstant = atof(yytext);
                         return T_DoubleConstant; }

 /* -------------------- Identifiers --------------------------- */
{identifier}        {  strncpy(yylval.identifier, yytext, MaxIdentLen);
                       yylval.identifier[MaxIdentLen] = '\0';
                       return T_Identifier; }

/* ----------------------- Comments -----------------------*/


/* ----------------------- Default ------------------------*/
.						printf("Unknown token\n"); yyterminate();

%%
