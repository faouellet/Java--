/* Definition section */
%skeleton "lalr1.cc"
%require "3.0"
%defines
%define parser_class_name { Parser }
%define api.namespace { javamm }

%code requires
{
  namespace javamm
  {
    class Lexer;
  }
}

%lex-param      { Lexer &Lex }
%parse-param    { Lexer &Lex }

%locations
%define parse.trace
%define parse.error verbose

/* Tokens (terminal symbols) declarations */
%token <>

/* Non terminal symbols declarations */
%type <>

/* Operator precedence */

%start program

%%
/* Rules section */

%%
/* Code section */
