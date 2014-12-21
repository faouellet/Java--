/* Definition section */

%{
#include "lexer.h"

typedef javamm::Parser::token token;

// Taking over yyterminate to return a real token instead of nullptr
#define yyterminate() return( token::END )
%}

%option c++
%option noyywrap
%option default
%option yyclass="javamm::Lexer"

%%

"def"                   { return token::DEF; }
"extern"                { return token::EXTERN; }
"if"                    { return token::IF; }
"then"                  { return token::THEN; }
"else"                  { return token::ELSE; }

","                     { return token::COMMA; }
"}"                     { return token::CLOSE_BRACE; }
")"                     { return token::CLOSE_PAREN; }
"{"                     { return token::OPEN_BRACE; }
"("                     { return token::OPEN_PAREN; }
";"                     { return token::STATEMENT_END; }

"="                     { yyval->Op = *yytext; return token::ASSIGNMENT; }
"+"                     { yyval->Op = *yytext; return token::ADD; }
"/"                     { yyval->Op = *yytext; return token::DIVIDE; }
">"                     { yyval->Op = *yytext; return token::GREATER; }
"<"                     { yyval->Op = *yytext; return token::LESS; }
"*"                     { yyval->Op = *yytext; return token::MULTIPLY; }
"-"                     { yyval->Op = *yytext; return token::SUBTRACT; }

[a-zA-Z][a-zA-Z0-9]*    { yyval->Str = new std::string(yytext); return token::IDENTIFIER; }
[0-9.]+                 { yyval->Num = strtod(yytext, 0); return token::NUMBER; } 

%%

/* Code section */

