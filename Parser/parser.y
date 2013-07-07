%{

#include "scanner.h"
#include "parser.h"

void yyerror(const char * in_ErrorMsg) { printf("ERROR: %s\n", in_ErrorMsg); std::exit(1); }

%}

/*yylval*/
%union
{
	int      IntConstant;
	double   DoubleConstant;
	char *   Identifier;
}

/* Leaf Types */
%token   T_Int T_Double
%token   T_LessThan T_GreaterThan T_LessEqual T_GreaterEqual T_Equal T_NotEqual
%token   T_And T_Or

%token   <identifier>      T_Identifier
%token   <IntConstant>     T_IntConstant
%token   <DoubleConstant>  T_DoubleConstant

/* Node Types */
%type

/* Operators precedence */
%left      '='
%left      T_Or
%left      T_And
%nonassoc  T_Equal T_NotEqual
%nonassoc  T_LessThan T_GreaterThan T_LessEqual T_GreaterEqual
%left      '+' '-'
%left      '*' '/' '%'
%nonassoc  '!'
%nonassoc  '.'

%%

program 	: stmts { programBlock = $1; }
			;

stmts 		: stmt { $$ = new NBlock(); $$->Statements.push_back($<stmt>1); }
			| stmts stmt { $1->Statements.push_back($<stmt>2); }
			;

stmt 		: var_decl | func_decl
			| expr { $$ = new NExpressionStatement(*$1); }
	     	;

block 		: TLBRACE stmts TRBRACE { $$ = $2; }
			| TLBRACE TRBRACE { $$ = new NBlock(); }
			;

var_decl 	: ident ident { $$ = new NVariableDeclaration(*$1, *$2); }
			| ident ident TEQUAL expr { $$ = new NVariableDeclaration(*$1, *$2, $4); }
			;

func_decl 	: ident ident TLPAREN func_decl_args TRPAREN block { $$ = new NFunctionDeclaration(*$1, *$2, *$4, *$6); delete $4; }
			;

func_decl_args 	: /*blank*/ { $$ = new VariableList(); }
				| var_decl { $$ = new VariableList(); $$->push_back($<var_decl>1); }
				| func_decl_args TCOMMA var_decl { $1->push_back($<var_decl>3); }
				;

ident 		: TIDENTIFIER { $$ = new NIdentifier(*$1); delete $1; }
			;

numeric 	: TINTEGER { $$ = new NInteger(atol($1->c_str())); delete $1; }
			| TDOUBLE { $$ = new NDouble(atof($1->c_str())); delete $1; }
			;

expr 		: ident TEQUAL expr { $$ = new NAssignment(*$<ident>1, *$3); }
			| ident TLPAREN call_args TRPAREN { $$ = new NMethodCall(*$1, *$3); delete $3; }
			| ident { $<ident>$ = $1; }
			| numeric
			| expr comparison expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
		    | TLPAREN expr TRPAREN { $$ = $2; }
			;

call_args 	: /*blank*/ { $$ = new ExpressionList(); }
			| expr { $$ = new ExpressionList(); $$->push_back($1); }
			| call_args TCOMMA expr { $1->push_back($3); }
			;

comparison	: TCEQ 
			| TCNE 
			| TCLT 
			| TCLE 
			| TCGT 
			| TCGE
;
