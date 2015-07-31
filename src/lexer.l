/* Definition section */

%{
#include "lexer.h"

#include <iostream>

typedef javamm::Parser::token token;

// Taking over yyterminate to return a real token instead of nullptr
#define yyterminate() return( token::END )
%}

%option c++
%option noyywrap
%option default
%option yyclass="javamm::Lexer"
%option nounistd

%%

"def"                   { return token::DEF; }
"var"                   { return token::VAR; }
"extern"                { return token::EXTERN; }
"for"                   { return token::FOR; }
"if"                    { return token::IF; }
"then"                  { return token::THEN; }
"else"                  { return token::ELSE; }
"while"                 { return token::WHILE; }

"const"                 { std::cout << "Keyword const is not allowed" << std::endl; }
"goto"                  { std::cout << "Keyword goto is not allowed" << std::endl; }

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

