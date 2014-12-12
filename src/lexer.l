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
%option yyclass="Lexer"

%%

[0-9]+  { std::cout << "INT" << std::endl; }

%%

/* Code section */

