/*** Definition section ***/
%skeleton "lalr1.cc"
%require "3.0"
%defines
%define parser_class_name { Parser }
%define api.namespace { javamm }

%code requires {
  #include "driver.h"

  namespace javamm
  {
    class Lexer;
  }
}

%lex-param      { javamm::Lexer &Lex }
%parse-param    { javamm::Lexer &Lex }
%parse-param    { javamm::Driver &Drive }

%define parse.trace
%define parse.error verbose

%code {
  #include "lexer.h"

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
  IONode *ION;
  std::vector<std::string> *Names;
  std::vector<ExprNode *> *ENodes;
}

/* Clean up routine */
%destructor {
  if($$) {
    delete $$;
    $$ = nullptr;
  }
} <Str> <ENode> <PNode> <FNode> <Names> <ENodes> 

%define api.token.prefix {}

/* Tokens (terminal symbols) declarations */
%token          END             0
%token          DEF             "def"
%token          VAR             "var"
%token          EXTERN          "extern"
%token          FOR             "for"
%token          IF              "if"
%token          THEN            "then"
%token          ELSE            "else"
%token          WHILE           "while"
%token          PRINTLN         "println"

%token          COMMA           ","
%token          CLOSE_BRACE     "}"
%token          CLOSE_PAREN     ")"
%token          OPEN_BRACE      "{"
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
%type   <ENode>  decl
%type   <ENode>  expression 
%type   <ENode>  binaryexpr callexpr forexpr identifierexpr ifexpr numberexpr parenexpr whileexpr
%type   <FNode>  definition
%type   <PNode>  external prototype
%type   <Names>  argsnames
%type   <ENodes> callargs

/* Operator precedence */
%left "="
%left "<" ">"
%left "+" "-"
%left "/" "*"

%left "else" "then";

%start program;

%%
/*** Rules section ***/
program : definition { Drive.setRoot($1); }
 | external STATEMENT_END { Drive.setRoot($1); }
 | expression STATEMENT_END { 
                PrototypeNode *Proto = new PrototypeNode("", std::vector<std::string>()); 
                Drive.setRoot(new FunctionNode(Proto, $1));
              }
 ;

expression : decl
 | binaryexpr 
 | callexpr 
 | forexpr
 | identifierexpr 
 | ifexpr
 | numberexpr
 | parenexpr 
 | whileexpr
 ;

decl : VAR IDENTIFIER ASSIGNMENT expression STATEMENT_END { $$ = new DeclNode(*$2, $4); }

binaryexpr : expression ASSIGNMENT expression { $$ = new BinaryExprNode($2, $1, $3); }
 | expression ADD       expression { $$ = new BinaryExprNode($2, $1, $3); }
 | expression DIVIDE    expression { $$ = new BinaryExprNode($2, $1, $3); }
 | expression GREATER   expression { $$ = new BinaryExprNode($2, $1, $3); }
 | expression LESS      expression { $$ = new BinaryExprNode($2, $1, $3); }
 | expression MULTIPLY  expression { $$ = new BinaryExprNode($2, $1, $3); }
 | expression SUBTRACT  expression { $$ = new BinaryExprNode($2, $1, $3); }
 ;

callexpr : IDENTIFIER OPEN_PAREN callargs CLOSE_PAREN { $$ = new CallExprNode(*$1, *$3); delete $3; }
 ;

callargs : { $$ = new std::vector<ExprNode *>(); }
 | callargs COMMA expression { $$ = $1; $$->push_back($3); }
 | expression { $$ = new std::vector<ExprNode *>(); $$->push_back($1); }
 ;

forexpr : FOR OPEN_PAREN IDENTIFIER ASSIGNMENT expression STATEMENT_END expression STATEMENT_END expression CLOSE_PAREN expression
        { $$ = new ForNode(*$3, $5, $7, $9, $11); }
 ;

identifierexpr : IDENTIFIER { $$ = new VariableExprNode(*$1); }
 ;

ifexpr : IF OPEN_PAREN expression CLOSE_PAREN 
         THEN OPEN_BRACE expression CLOSE_BRACE 
         ELSE OPEN_BRACE expression CLOSE_BRACE { $$ = new IfNode($3, $7, $11); }

numberexpr : NUMBER { $$ = new NumberExprNode($1); }
 ;

parenexpr : OPEN_PAREN expression CLOSE_PAREN { $$ = $2; }
 ;

whileexpr : WHILE OPEN_PAREN expression CLOSE_PAREN
            OPEN_BRACE expression CLOSE_BRACE { $$ = new WhileNode($3, $6); }

definition : DEF prototype OPEN_BRACE expression CLOSE_BRACE { $$ = new FunctionNode($2, $4); }
 ;

external : EXTERN prototype { $$ = $2; }
 ;

prototype : IDENTIFIER OPEN_PAREN argsnames CLOSE_PAREN { $$ = new PrototypeNode(*$1, *$3); delete $3; }
 ;

argsnames : { $$ = new std::vector<std::string>(); }
 | argsnames COMMA IDENTIFIER { $$ = $1; $$->push_back(*$3); }
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
