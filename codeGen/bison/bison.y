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
%token JUMP MARK_TOK

%union {
    int type;
    char *str;
    AST::BaseAST *expr;
}

%type <str> ID CONST_INT CONST_DOUBLE ID_TOK
%type <type> INT CHAR DOUBLE TYPE
%type <expr> EXPR CONST DEFVAR UNDEFVAR BODY COND IFELSE ELSEIF MARK GOTO ATOM
%type <expr> VAR EVAL ANYVAR LOOP ATOMLLIST

%%
START:  ATOM { ast->AddToLink(AST::typeLink, $1); }
        | START ATOM { ast->AddToLink(AST::typeLink, $2); }

BODY: ATOMLLIST { $$ = ast->GetBody(AST::typeBody, $1); }

ATOMLLIST:   ATOM { $$ = ast->GetBodyLList(AST::typeBodyLList, nullptr, $1); }
        | BODY ATOM { $$ = ast->GetBodyLList(AST::typeBodyLList, $1, $2); }

ATOM:   DEFVAR { $$ = $1; }
        | UNDEFVAR { $$ = $1; }
        | LOOP { $$ = $1; }
        | GOTO { $$ = $1; }
        | MARK { $$ = $1; }
        | IFELSE { $$ = $1; }
        | EVAL { $$ = $1;}

DEFVAR: TYPE ID '=' EXPR ';' { hash_table->CreateEntry($1, $2); $$ = ast->GetVariableDef($1, $2, $4); }

UNDEFVAR: TYPE ID ';' { hash_table->CreateEntry($1, $2); $$ = ast->GetVariableUndef($1, $2); }

ANYVAR: DEFVAR { $$ = $1; }
        | EVAL { $$ = $1; }
        | ';' { $$ = nullptr; }

LOOP:   FOR '(' ANYVAR  COND ';'  EVAL ')' ATOM { }
        | FOR '(' ANYVAR  COND ';' EVAL ')' '{' BODY '}' { }

COND:   VAR { }
        | VAR '<' COND { }
        | VAR '>' COND { }
        | VAR "==" COND { }

MARK:   ID ':' { hash_table->CreateEntry(MARK_TOK, $1); $$ = ast->GetMark(AST::typeMark, $1); }

GOTO:   JUMP ID_TOK ';' { if ($2) { $$ = ast->GetJump(AST::typeJump, $2); } else { $$ = nullptr; } }

IFELSE: IF '(' COND ')' ATOM { }
        | IF '(' COND ')' '{' BODY '}' { }
        | IF '(' COND ')' '{' BODY '}' ELSEIF { }

ELSEIF: ELSE '{' BODY '}' { }
        | ELSE ATOM  { }

EVAL: ID '=' EXPR ';'  { if ($1) $$ = ast->GetEval(AST::typeEval, $1, $3); else $$ = nullptr; }

EXPR:   EXPR '+' VAR { $$ = ast->GetBinaryExpr(AST::typeOpr, '+', $1, $3); }
        | EXPR '-' VAR { $$ = ast->GetBinaryExpr(AST::typeOpr, '-', $1, $3); }
        | EXPR '*' VAR { $$ = ast->GetBinaryExpr(AST::typeOpr, '*', $1, $3); }
        | EXPR '/' VAR { $$ = ast->GetBinaryExpr(AST::typeOpr, '/', $1, $3); }
        | VAR { $$ = $1; }

VAR:    CONST { $$ = $1; }
        | ID_TOK { $$ = ast->GetVariableExpr(AST::typeIvar, $1); }

ID_TOK: ID { if (hash_table->LookupEntry($1) != nullptr) { $$ = $1; } else { yyerror("Var not declaration"); $$ = ""; } }

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
    ast->DFS();
    fclose(yyin);
    // delete hash_table;
    delete ast;
    return EXIT_SUCCESS;
}