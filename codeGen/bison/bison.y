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
%token for ELSE PRINT FOR LEX_ERROR RETURN
%token ID CONST_INT CONST_DOUBLE

%union {
    int type;
    char* str;
    AST::BaseAST* expr;
}

%type <str> ID CONST_INT CONST_DOUBLE
%type <type> INT CHAR DOUBLE TYPE
%type <expr> EXPR CONST DEFVAR UNDEFVAR BODY COND

%%
START:  ATOM { }
        | START ATOM { }

BODY:   ATOM { }
        | BODY ATOM { }

ATOM:   DEFVAR { }
        | UNDEFVAR { }
        | LOOP { }
        | RETURN { }

DEFVAR: TYPE ID '=' EXPR';' { }

UNDEFVAR: TYPE ID';' { }

LOOP:   for '(' DEFVAR ';' COND ';' EXPR ')' ATOM ';' { }
        | for '(' DEFVAR ';' COND ';' EXPR ')' '{' BODY '}' { }

COND:   CONST { }
        | CONST '<' COND { }
        | CONST '>' COND { }
        | CONST "==" COND { }


EXPR:   EXPR '+' CONST { $$ = ast->ParseBinaryExpr(ast->GetBinaryExpr(AST::typeOpr, '+', $1, $3)); }
        | EXPR '-' CONST { $$ = ast->ParseBinaryExpr(ast->GetBinaryExpr(AST::typeOpr, '-', $1, $3)); }
        | EXPR '*' CONST { $$ = ast->ParseBinaryExpr(ast->GetBinaryExpr(AST::typeOpr, '*', $1, $3)); }
        | EXPR '/' CONST { $$ = ast->ParseBinaryExpr(ast->GetBinaryExpr(AST::typeOpr, '/', $1, $3)); }
        | CONST { $$ = $1; }

CONST:  CONST_INT { $$ = ast->GetIntNumberExpr(AST::typeIvar, atoi($1)); }
        | CONST_DOUBLE { $$ = ast->GetDoubleNumberExpr(AST::typeDvar, atof($1)); }

TYPE:   INT {$$ = $1;}
        | CHAR {$$ = $1;}
        | DOUBLE {$$ = $1;}
%%

void yyerror(const char *errmsg) {
    fprintf(stderr, "Position (%d, %d): %s\n", yylineno, ch, errmsg);
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
    ast = new AST::AST();
    ch = 1;
    yylineno = 1;

    yyparse();
    fclose(yyin);
    delete hash_table;
    return EXIT_SUCCESS;
}