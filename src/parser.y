/*** Definition section ***/
%skeleton "lalr1.cc"
%require "3.0"
%defines
%define parser_class_name { Parser }
%define api.namespace { javamm }

%code requires {
  #include "node.h"

  namespace javamm
  {
    class Lexer;
  }
}

%lex-param      { javamm::Lexer &Lex }
%parse-param    { javamm::Lexer &Lex }

%code {
  static int yylex(javamm::Parser::semantic_type *yyval, javamm::Lexer &Lex);
}

/* yyval */
%union {
  std::string *Str;
  double Num;
  char Op;
  ExprNode *ENode;
  PrototypeNode *PNode;
  FunctionNode *FNode;
  std::vector<std::string> *Names;
  std::vector<ExprNode *> *Nodes;
}

/* Clean up routine */
%destructor {
  if($$) {
    delete $$;
    $$ = nullptr;
  }
} <Str> <ENode> <PNode> <FNode> <Names> <Nodes>

%define api.token.prefix {}

/* Tokens (terminal symbols) declarations */
%token          END             0
%token          DEF             "def"
%token          EXTERN          "extern"

%token          COMMA           ","
%token          CLOSE_PAREN     ")"
%token          OPEN_PAREN      "("
%token          STATEMENT_END   ";"

%token  <Str>   IDENTIFIER
%token  <Num>   NUMBER

%token  <Op>    ASSIGNMENT      "="
%token  <Op>    ADD             "+"
%token  <Op>    DIVIDE          "/"
%token  <Op>    GREATER         ">"
%token  <Op>    LESS            "<"
%token  <Op>    MULTIPLY        "*"
%token  <Op>    SUBTRACT        "-"

/* Non terminal symbols declarations */
%type   <ENode>  expression 
%type   <ENode>  binaryexpr callexpr identifierexpr numberexpr parenexpr
%type   <FNode>  definition
%type   <PNode>  external prototype
%type   <Names>  argsnames
%type   <Nodes>  callargs

/* Operator precedence */
%left "="
%left "<" ">"
%left "+" "-"
%left "/" "*"

%start top

%%
/*** Rules section ***/
top : definition 
 | external 
 | expression 
 ;

expression : binaryexpr END
 | callexpr END
 | identifierexpr END
 | numberexpr END
 | parenexpr END
 ;

binaryexpr : expression "=" expression { $$ = new BinaryExprNode($2, $1, $3); }
 | expression "+" expression { $$ = new BinaryExprNode($2, $1, $3); }
 | expression "/" expression { $$ = new BinaryExprNode($2, $1, $3); }
 | expression ">" expression { $$ = new BinaryExprNode($2, $1, $3); }
 | expression "<" expression { $$ = new BinaryExprNode($2, $1, $3); }
 | expression "*" expression { $$ = new BinaryExprNode($2, $1, $3); }
 | expression "-" expression { $$ = new BinaryExprNode($2, $1, $3); }
 ;

callexpr : IDENTIFIER "(" callargs ")" { $$ = new CallExprNode(*$1, *$3); }
 ;

callargs : { $$ = new std::vector<ExprNode *>(); }
 | callargs "," expression { $$ = $1; $$->push_back($3); }
 | expression { $$ = new std::vector<ExprNode *>(); $$->push_back($1); }
 ;

identifierexpr : IDENTIFIER { $$ = new VariableExprNode(*$1); }
 ;

numberexpr : NUMBER { $$ = new NumberExprNode($1); }
 ;

parenexpr : "(" expression ")" { $$ = $2; }
 ;

definition : "def" prototype expression { $$ = new FunctionNode($2, $3); }
 ;

external : "extern" prototype { $$ = $2; }
 ;

prototype : IDENTIFIER "(" argsnames ")" { $$ = new PrototypeNode(*$1, *$3); }
 ;

argsnames : { $$ = new std::vector<std::string>(); }
 | argsnames "," IDENTIFIER { $$ = $1; $$->push_back(*$3); }
 | IDENTIFIER { $$ = new std::vector<std::string>(); $$->push_back(*$1); }
 ;

%%
/*** Code section ***/
static int yylex(javamm::Parser::semantic_type *yyval, javamm::Lexer &Lex) {
  return Lex.yylex(yyval);
}

void javamm::Parser::error(const std::string &Err) {
  std::cerr << "Error: " << Err << std::endl;
}
