%{
    #include <stdio.h>
    #include "../inc/HashTable.h"
    #include "../inc/Ast.h"
    extern FILE *yyin;
    extern int yylineno;
    extern int ch;
    extern char *yytext;
    extern void yyerror(const char *);
    extern HashTable* hash_table;
    extern int yylex();
    AST::Ast* ast;
%}

%token INT CHAR DOUBLE
%token IF ELSE PRINT FOR LEX_ERROR RETURN
%token ID CONST_INT CONST_DOUBLE
%token JUMP

%union {
    int type;
    char* str;
    AST::BaseAST* expr;
}

%type <str> ID CONST_INT CONST_DOUBLE
%type <type> INT CHAR DOUBLE TYPE
%type <expr> EXPR CONST DEFVAR UNDEFVAR BODY COND IFELSE ELSEIF MARK GOTO
%type <expr> VAR EVAL ANYVAR

%%
START:  ATOM { }
        | START ATOM { }

BODY:   ATOM { }
        | BODY ATOM { }

ATOM:   DEFVAR { }
        | UNDEFVAR { }
        | LOOP { }
        | RETURN { }
        | GOTO { }
        | MARK { }
        | IFELSE { }
        | EVAL { }

DEFVAR: TYPE ID '=' EXPR ';' { }

UNDEFVAR: TYPE ID ';' { }

ANYVAR: DEFVAR { }
        | EVAL { }
        | ';' { }

LOOP:   FOR '(' ANYVAR  COND ';'  EVAL ')' ATOM { }
        | FOR '(' ANYVAR  COND ';' EVAL ')' '{' BODY '}' { }

COND:   VAR { }
        | VAR '<' COND { }
        | VAR '>' COND { }
        | VAR "==" COND { }

MARK:   ID ':' { }

GOTO:   JUMP ID ';' { }

IFELSE: IF '(' COND ')' ATOM { }
        | IF '(' COND ')' '{' BODY '}' { }
        | IF '(' COND ')' '{' BODY '}' ELSEIF { }

ELSEIF: ELSE '{' BODY '}' { }
        | ELSE ATOM  { }

EVAL: ID '=' EXPR ';'  { }

EXPR:   EXPR '+' VAR { $$ = ast->ParseBinaryExpr(ast->GetBinaryExpr(AST::typeOpr, '+', $1, $3)); }
        | EXPR '-' VAR { $$ = ast->ParseBinaryExpr(ast->GetBinaryExpr(AST::typeOpr, '-', $1, $3)); }
        | EXPR '*' VAR { $$ = ast->ParseBinaryExpr(ast->GetBinaryExpr(AST::typeOpr, '*', $1, $3)); }
        | EXPR '/' VAR { $$ = ast->ParseBinaryExpr(ast->GetBinaryExpr(AST::typeOpr, '/', $1, $3)); }
        | VAR { $$ = $1; }

VAR:    CONST { $$ = $1; }
        | ID { $$ = ast->GetVariableExpr(AST::typeIvar, $1); }

CONST:  CONST_INT { $$ = ast->GetIntNumberExpr(AST::typeIvar, atoi($1)); }
        | CONST_DOUBLE { $$ = ast->GetDoubleNumberExpr(AST::typeDvar, atof($1)); }

TYPE:   INT { $$ = $1; }
        | CHAR { $$ = $1; }
        | DOUBLE { $$ = $1; }
%%

void yyerror(const char *errmsg) {
    fprintf(stderr, "Position (%d, %d): [%s] %s\n", yylineno, ch, yytext, errmsg);
}

int main(int argc, char** argv) {
    if(argc < 2) {
        fprintf(stderr, "\nNot enough arguments. Please specify filename.\n");
        return EXIT_FAILURE;
    }
    if((yyin = fopen(argv[1], "r")) == nullptr) {
        printf("\nCannot open file %s.\n", argv[1]);
        return EXIT_FAILURE;
    }
    ast = new AST::Ast();
    ch = 1;
    yylineno = 1;

    yyparse();
    fclose(yyin);
    delete hash_table;
    delete ast;
    return EXIT_SUCCESS;
}