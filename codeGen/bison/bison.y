%{
    #include <stdio.h>
    #include "HashTable.h"
    #include "AST.h"
    extern FILE *yyin;
    extern int yylineno;
    extern int ch;
    extern char *yytext;
    extern void yyerror(const char *);
    extern HashTable* hash_table;
    extern int yylex();
    AST::AST* ast;
%}

%token INT CHAR DOUBLE
%token IF ELSE PRINT FOR LEX_ERROR RETURN
%token ID CONST_INT CONST_DOUBLE

%union {
    int type;
    char* str;
    AST::BaseAST* expr;
}

%type <type> INT CHAR DOUBLE TYPE
%type <str> ID CONST_INT CONST_DOUBLE
%type <expr> EXPR CONST CALC

%%
START: BODY {}

BODY: ATOM
      | ATOM BODY {}

ATOM: DEFVAR | UNDEFVAR | CALL | LOOP | RETURN

DEFAVR: TYPE ID '=' EXPR';' {}
UNDEF: TYPE ID';' {}
CALL: ID'('COND')'';' {}

LOOP: 'for''('DEFVAR';'COND';'EXPR')' ATOM ';' {}
       | 'for''('DEFVAR';'COND';'EXPR')' '{' BODY '}' {}

EXPR: EXPR '+' CONST { $$ = ast->ParseBinaryExpr(ast->GetBinaryExpr(AST::typeOpr, '+', $1, $3)); }
      | EXPR '-' CONST { $$ = ast->ParseBinaryExpr(ast->GetBinaryExpr(AST::typeOpr, '-', $1, $3)); }
      | EXPR '*' CONST { $$ = ast->ParseBinaryExpr(ast->GetBinaryExpr(AST::typeOpr, '*', $1, $3)); }
      | EXPR '/' CONST { $$ = ast->ParseBinaryExpr(ast->GetBinaryExpr(AST::typeOpr, '/', $1, $3)); }
      | CONST { $$ = $1; }

CONST: CONST_INT { $$ = ast->GetIntNumberExpr(AST::typeIvar, atoi($1)); }
       | CONST_DOUBLE { $$ = ast->GetDoubleNumberExpr(AST::typeDvar, atof($1)); }

TYPE: INT {$$ = $1;}
      | CHAR {$$ = $1;}
      | DOUBLE {$$ = $1;}
%%

void yyerror(const char *errmsg) {
    fprintf(stderr, "Line %d: %s\n", yylineno, errmsg);
}

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("\nNot enough arguments. Please specify filename.\n");
        return -1;
    }
    if((yyin = fopen(argv[1], "r")) == nullptr) {
        printf("\nCannot open file %s.\n", argv[1]);
        return -1;
    }
    ast = new AST::AST();
    ch = 1;
    yylineno = 1;

    yyparse();
    fclose(yyin);
    delete hash_table;
    return 0;
}