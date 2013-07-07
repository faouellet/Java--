%{

#include "scanner.h"
#include "parser.h"

void yyerror(const char * in_ErrorMsg) { printf("ERROR: %s\n", in_ErrorMsg); std::exit(1); }

%}

/*yylval*/
%union
{
	int					intConstant;
	double				doubleConstant;
	char				identifier[MaxIdentLen+1]; // +1 for terminating null
	Declaration			*decl;
    list<Decl*>			*declList;
    Type				*type;
    NamedType			*cType;
    list<NamedType*>	*cTypeList;
    FnDecl				*fDecl;
    VarDecl				*var;
    list<VarDecl*>		*varList;
    Expr				*expr;
    list<Expr*>			*exprList;
    Stmt				*stmt;
    list<Stmt*>			*stmtList;
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

Program     : DeclList				  {
										  @1;
										  Program * program = new Program($1);
										  program->Emit();
			  						  }
	   	    ;

DeclList    : DeclList Decl			  { ($$=$1)->push_back($2); }
		    | Decl					  { ($$=new list<Decl*>)->push_back($1); }
		    ;

Decl        :    FnDecl				  { $$=$1; }
            |    VarDecl			  { $$=$1; }
            ;

VarDecl     :    Variable ';' 
		    ;

Type        :    T_Int                { $$ = Type::IntType; }
            |    T_Double             { $$ = Type::DoubleType; }
            |    T_Identifier         { $$ = new NamedType(new Identifier(@1,$1)); }
		    ;

Formals     :    FormalList           { $$ = $1; }
            |    /* empty */          { $$ = new list<VarDecl*>; }
            ;

FormalList  :    FormalList ',' Variable  
                                      { ($$=$1)->push_back($3); }
            |    Variable             { ($$ = new list<VarDecl*>)->push_back($1); }
            ;

FnDecl      :    FnHeader StmtBlock   { ($$=$1)->SetFunctionBody($2); }
            ;

StmtBlock   :    '{' VarDecls StmtList '}' 
                                      { $$ = new StmtBlock($2, $3); }
            ;

VarDecls    :    VarDecls VarDecl     { ($$=$1)->push_back($2); }
            |    /* empty */          { $$ = new list<VarDecl*>; }
            ;

StmtList    :    Stmt StmtList        { $$ = $2; $$->push_front($1); }
            |    /* empty */          { $$ = new list<Stmt*>; }
            ;

Stmt        :    OptExpr ';'          { $$ = $1; }
            |    StmtBlock
            |    T_Return Expr ';'    { $$ = new ReturnStmt(@2, $2); }
            |    T_Return ';'         { $$ = new ReturnStmt(@1, new EmptyExpr()); }
            ;

Call        :    T_Identifier '(' Actuals ')' 
                                      { $$ = new Call(Join(@1,@4), NULL, new Identifier(@1,$1), $3); }
            |    Expr '.' T_Identifier '(' Actuals ')' 
                                      { $$ = new Call(Join(@1,@6), $1, new Identifier(@3,$3), $5); }
            ;

OptExpr     :    Expr                 { $$ = $1; }
            |    /* empty */          { $$ = new EmptyExpr(); }

Expr        :    Call
            |    Constant
            |    LValue '=' Expr      { $$ = new AssignExpr($1, new Operator(@2,"="), $3); }
            |    Expr '+' Expr        { $$ = new ArithmeticExpr($1, new Operator(@2, "+"), $3); }
            |    Expr '-' Expr        { $$ = new ArithmeticExpr($1, new Operator(@2, "-"), $3); }
            |    Expr '/' Expr        { $$ = new ArithmeticExpr($1, new Operator(@2,"/"), $3); }
            |    Expr '*' Expr        { $$ = new ArithmeticExpr($1, new Operator(@2,"*"), $3); }
            |    Expr '%' Expr        { $$ = new ArithmeticExpr($1, new Operator(@2,"%"), $3); }
            |    Expr T_Equal Expr    { $$ = new EqualityExpr($1, new Operator(@2,"=="), $3); }
            |    Expr T_NotEqual Expr { $$ = new EqualityExpr($1, new Operator(@2,"!="), $3); }
            |    Expr T_LessThan Expr
							          { $$ = new RelationalExpr($1, new Operator(@2,"<"), $3); }
            |    Expr T_GreaterThan Expr        
									  { $$ = new RelationalExpr($1, new Operator(@2,">"), $3); }
            |    Expr T_LessEqual Expr 
                                      { $$ = new RelationalExpr($1, new Operator(@2,"<="), $3); }
            |    Expr T_GreaterEqual Expr 
                                      { $$ = new RelationalExpr($1, new Operator(@2,">="), $3); }
            |    Expr T_And Expr      { $$ = new LogicalExpr($1, new Operator(@2,"&&"), $3); }
            |    Expr T_Or Expr       { $$ = new LogicalExpr($1, new Operator(@2,"||"), $3); }
            |    '(' Expr ')'         { $$ = $2; }
            ;

Constant  :    T_IntConstant        { $$ = new IntConstant(@1,$1); }
          |    T_DoubleConstant     { $$ = new DoubleConstant(@1,$1); }
          ;

Actuals   :    ExprList             { $$ = $1; }
          |    /* empty */          { $$ = new list<Expr*>; }
          ;

ExprList  :    ExprList ',' Expr    { ($$=$1)->push_back($3); }
          |    Expr                 { ($$ = new list<Expr*>)->push_back($1); }
          ;
