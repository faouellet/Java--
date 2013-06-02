%{
	#include "node.h"
	NBlock * _ProgramBlock; /*Top level root node of the final AST*/

	extern int yylex();
	void yyerror(const char * in_ErrorMsg) { printf("ERROR: %s\n", in_ErrorMsg); }
%}

/*Possible ways to access the data*/
%union
{
	Node* node;
	NBlock* block;
	NExpression* expr;
	NStatement* stmt;
	NIdentifier* ident;
	NVariableDeclaration* var_decl;
	std::vector<NVariableDeclaration*>* var_vec;
	std::vector<NExpression*>* expr_vec;
	std::string* string;
	int token;
}

/**/
%token <string> TIDENTIFIER TINTEGER TDOUBLE
%token <token> TCEQ TCNE TCLT TCLE TCGT TCGE TEQUAL
%token <token> TLPAREN TRPAREN TLBRACE TRBRACE TCOMMA TDOT
%token <token> TPLUS TMINUS TMUL TDIV

/**/
%type <ident>	ident
%type <expr>	numeric expr
%type <varvec>	func_decl_args
%type <exprvec>	call_args
%type <block>	program stmts block
%type <stmt>	stmt var_decl func_decl
%type <token>	comparison

/* Operator precedence */
%left TPLUS TMINUS
%left TMUL TDIV

%start program

%%

program 	: stmts { programBlock = $1; }
			;

stmts 		: stmt { $$ = new NBlock(); $$->statements.push_back($<stmt>1); }
			| stmts stmt { $1->statements.push_back($<stmt>2); }
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
			| TPLUS 
			| TMINUS 
			| TMUL 
			| TDIV
;
