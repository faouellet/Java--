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
%type <expr>      Constant Expr Call OptExpr
%type <decl>      Decl
%type <fDecl>     FnDecl FnHeader
%type <declList>  DeclList
%type <var>       Variable VarDecl
%type <varList>   Formals FormalList VarDecls
%type <exprList>  Actuals ExprList
%type <stmt>      Stmt StmtBlock
%type <stmtList>  StmtList

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

Program					: DeclarationList								{
																			@1;
																			Program * program = new Program($1);
																			program->Emit();
			  															}
	   					;

DeclarationList		    : DeclarationList Declaration   				{ ($$=$1)->push_back($2); }
						| Declaration									{ ($$=new list<Declaration*>)->push_back($1); }
						;

Declaration				:    FunctionDeclaration						{ $$=$1; }
						|    VariableDeclaration 						{ $$=$1; }
						;

VariableDeclaration     :    Variable ';' 
						;

Type					:    T_Int										{ $$ = Type::IntType; }
						|    T_Double									{ $$ = Type::DoubleType; }
						|    T_Identifier								{ $$ = new NamedType(new Identifier(@1,$1)); }
						;

FunctionHeader		    :    Type T_Identifier '(' Formals ')'			{ $$ = new FunctionDeclaration(new Identifier(@2, $2), $1, $4); }
						|    T_Void T_Identifier '(' Formals ')'		{ $$ = new FunctionDeclaration(new Identifier(@2, $2), Type::VoidType, $4); }
						;

Formals					:    FormalList									{ $$ = $1; }
						|    /* empty */								{ $$ = new list<VarDecl*>; }
						;

FormalList				:    FormalList ',' Variable					{ ($$=$1)->push_back($3); }
						|    Variable									{ ($$ = new list<VarDecl*>)->push_back($1); }
						;

FunctionDeclaration     :    FunctionHeader StatementBlock				{ ($$=$1)->SetFunctionBody($2); }
						;

StatementBlock			:    '{' VariableDeclarations StatementList '}' { $$ = new StatementBlock($2, $3); }
						;

VariableDeclarations    :    VariableDeclarations VariableDeclaration   { ($$=$1)->push_back($2); }
						|    /* empty */								{ $$ = new list<VariableDeclaration*>; }
						;

StatementList			:    Statement StatementList					{ $$ = $2; $$->push_front($1); }
						|    /* empty */								{ $$ = new list<Statement*>; }
						;

Stmt					:    OptExpr ';'								{ $$ = $1; }
						|    StatementBlock
						|    T_Return Expression ';'					{ $$ = new ReturnStmt(@2, $2); }
						|    T_Return ';'								{ $$ = new ReturnStmt(@1, new EmptyExpr()); }
						;

FunctionCall			:    T_Identifier '(' Actuals ')'				{ $$ = new Call(Join(@1,@4), NULL, new Identifier(@1,$1), $3); }
						|    Expression '.' T_Identifier '(' Actuals ')' 
																		{ $$ = new Call(Join(@1,@6), $1, new Identifier(@3,$3), $5); }
						;

OptExpr					:    Expression									{ $$ = $1; }
						|    /* empty */								{ $$ = new EmptyExpression(); }
						;

Expression				:    FunctionCall
						|    Constant
						|    LValue '=' Expression						{ $$ = new AssignExpr($1, new Operator(@2,"="), $3); }
						|    Expression '+' Expression					{ $$ = new ArithmeticExpression($1, new Operator(@2, "+"), $3); }
						|    Expression '-' Expression					{ $$ = new ArithmeticExpression($1, new Operator(@2, "-"), $3); }
						|    Expression '/' Expression					{ $$ = new ArithmeticExpression($1, new Operator(@2,"/"), $3); }
						|    Expression '*' Expression					{ $$ = new ArithmeticExpression($1, new Operator(@2,"*"), $3); }
						|    Expression '%' Expression					{ $$ = new ArithmeticExpression($1, new Operator(@2,"%"), $3); }
						|    Expression T_Equal Expression				{ $$ = new EqualityExpression($1, new Operator(@2,"=="), $3); }
						|    Expression T_NotEqual Expression			{ $$ = new EqualityExpression($1, new Operator(@2,"!="), $3); }
						|    Expression T_LessThan Expression			{ $$ = new RelationalExpression($1, new Operator(@2,"<"), $3); }
						|    Expression T_GreaterThan Expression		{ $$ = new RelationalExpression($1, new Operator(@2,">"), $3); }
						|    Expression T_LessEqual Expression			{ $$ = new RelationalExpression($1, new Operator(@2,"<="), $3); }
						|    Expression T_GreaterEqual Expression		{ $$ = new RelationalExpression($1, new Operator(@2,">="), $3); }
						|    Expression T_And Expression				{ $$ = new LogicalExpression($1, new Operator(@2,"&&"), $3); }
						|    Expression T_Or Expression					{ $$ = new LogicalExpression($1, new Operator(@2,"||"), $3); }
						|    '(' Expression ')'							{ $$ = $2; }
						;

Constant				:    T_IntConstant								{ $$ = new IntConstant(@1,$1); }
						|    T_DoubleConstant							{ $$ = new DoubleConstant(@1,$1); }
						;

Actuals					:    ExpressionList								{ $$ = $1; }
						|    /* empty */								{ $$ = new list<Expression*>; }
						;

ExpressionList			:    ExpressionList ',' Expression				{ ($$=$1)->push_back($3); }
						|    Expression									{ ($$ = new list<Expression*>)->push_back($1); }
						;
