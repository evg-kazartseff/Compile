%{
    #include <stdio.h>
    #include "../inc/HashTable.h"
    #include "../inc/Ast.h"
    extern FILE *yyin;
    extern int yylineno;
    extern int ch;
    extern char *yytext;
    extern void yyerror(const char *);
    extern void yyerror(const char *, const char *);
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
    char* str;
    AST::BaseAST* expr;
    // TODO Fix unary priority

}

%type <str> ID CONST_INT CONST_DOUBLE ID_TOK
%type <type> INT CHAR DOUBLE TYPE
%type <expr> EXPR2 EXPR1 EXPR0 EXPR CONST DEFVAR UNDEFVAR VAR EVAL
%type <expr> MARK GOTO ATOM CALL ARGS ARG
%type <expr> ANYVAR LOOP ATOMLLIST COND IFELSE ELSEIF BODY LEVAL

%%
START:  ATOM { ast->AddToLink(AST::typeLink, $1); }
        | START ATOM { ast->AddToLink(AST::typeLink, $2); }

BODY: ATOMLLIST { $$ = ast->GetBody(AST::typeBody, $1); }

ATOMLLIST:   ATOM { $$ = ast->GetBodyLList(AST::typeBodyLList, nullptr, $1); }
        | ATOM BODY{ $$ = ast->GetBodyLList(AST::typeBodyLList, $2, $1); }

ATOM:   DEFVAR { $$ = $1; }
        | UNDEFVAR { $$ = $1; }
        | LOOP { $$ = $1; }
        | GOTO { $$ = $1; }
        | MARK { $$ = $1; }
        | IFELSE { $$ = $1; }
        | EVAL { $$ = $1; }
        | CALL { $$ = $1; }

CALL:   ID '(' ARGS ')' ';' { }

ARGS:   ARG { }
        | ARGS ',' ARG { }

ARG: ID { }

DEFVAR: TYPE ID '=' EXPR ';' { hash_table->CreateEntry($1, $2); $$ = ast->GetVariableDef($1, $2, $4); }

UNDEFVAR: TYPE ID ';' { hash_table->CreateEntry($1, $2); $$ = ast->GetVariableUndef($1, $2); }

ANYVAR: DEFVAR { $$ = $1; }
        | EVAL { $$ = $1; }
        | ';' { $$ = nullptr; }

LOOP:   FOR '(' ANYVAR  COND ';'  LEVAL ')' ATOM { $$ = ast->GetLoop(AST::typeLoop, $3, $4, $6, $8); }
        | FOR '(' ANYVAR  COND ';' LEVAL')' '{' BODY '}' { $$ = ast->GetLoop(AST::typeLoop, $3, $4, $6, $9); }

LEVAL:  ID '=' EXPR { $$ = ast->GetEval(AST::typeEval, $1, $3); }

COND:   VAR { $$ = $1; }
        | VAR '<' COND { $$ = ast->GetLogicExpr(AST::typeOpr, '<', $1, $3); }
        | VAR '>' COND { $$ = ast->GetLogicExpr(AST::typeOpr, '>', $1, $3); }
        | VAR "==" COND {$$ = ast->GetLogicExpr(AST::typeOpr, '=', $1, $3); }

MARK:   ID ':' { if (hash_table->LookupEntry($1) == nullptr) {
                    hash_table->CreateEntry(MARK_TOK, $1); $$ = ast->GetMark(AST::typeMark, $1);
                 } else { yyerror("Identificator already created", $1); $$ = nullptr; } }

GOTO:   JUMP ID_TOK ';' { if ($2) { $$ = ast->GetJump(AST::typeJump, $2); } else { $$ = nullptr; } }

IFELSE: IF '(' COND ')' ATOM { $$ = ast->GetIf(AST::typeIf, $3, $5, nullptr); }
        | IF '(' COND ')' '{' BODY '}' { $$ = ast->GetIf(AST::typeIf, $3, $6, nullptr); }
        | IF '(' COND ')' '{' BODY '}' ELSEIF { $$ = ast->GetIf(AST::typeIf, $3, $6, $8); }

ELSEIF: ELSE '{' BODY '}' { $$ = ast->GetElse(AST::typeElse, $3); }
        | ELSE ATOM  { $$ = ast->GetElse(AST::typeElse, $2); }

EVAL: ID '=' EXPR ';'  { if ($1) $$ = ast->GetEval(AST::typeEval, $1, $3); else $$ = nullptr; }

EXPR:   EXPR0 { $$ = $1; }
        | EXPR0 '&' EXPR { $$ = ast->GetBinaryExpr(AST::typeOpr, '&', $1, $3); }
        | EXPR0 '^' EXPR { $$ = ast->GetBinaryExpr(AST::typeOpr, '^', $1, $3); }
        | EXPR0 '|' EXPR { $$ = ast->GetBinaryExpr(AST::typeOpr, '|', $1, $3); }


EXPR0:   EXPR1 { $$ = $1; }
        | EXPR1 '+' EXPR0 { $$ = ast->GetBinaryExpr(AST::typeOpr, '+', $1, $3); }
        | EXPR1 '-' EXPR0 { $$ = ast->GetBinaryExpr(AST::typeOpr, '-', $1, $3); }

EXPR1:  EXPR2 { $$ = $1; }
        | EXPR2 '*' EXPR1 { $$ = ast->GetBinaryExpr(AST::typeOpr, '*', $1, $3); }
        | EXPR2 '/' EXPR1 { $$ = ast->GetBinaryExpr(AST::typeOpr, '/', $1, $3); }
        | EXPR2 '%' EXPR1 { $$ = ast->GetBinaryExpr(AST::typeOpr, '%', $1, $3); }

EXPR2:  VAR { $$ = $1; }
        | '(' EXPR ')' { $$ = $2;}
        | '~' EXPR { $$ = ast->GetUnary(AST::typeOpr, '~', $2); }

VAR:    CONST { $$ = $1; }
        | ID_TOK { $$ = ast->GetVariableExpr(AST::typeIvar, std::string($1)); }

ID_TOK: ID { if (hash_table->LookupEntry($1) != nullptr) { $$ = $1; } else { yyerror("Var not declaration"); $$ = ""; } }

CONST:  CONST_INT { $$ = ast->GetIntNumberExpr(AST::typeIvar, atoi($1)); }
        | CONST_DOUBLE { $$ = ast->GetDoubleNumberExpr(AST::typeDvar, atof($1)); }

TYPE:   INT { $$ = $1; }
        | CHAR { $$ = $1; }
        | DOUBLE { $$ = $1; }
%%

void yyerror(const char *errmsg)
{
    fprintf(stderr, "Position (%d, %d): [%s] %s\n", yylineno, ch, yytext, errmsg);
}

void yyerror(const char *errmsg, const char *msg)
{
    fprintf(stderr, "Position (%d, %d): [%s] %s\n", yylineno, ch, msg, errmsg);
}

int main(int argc, char** argv)
{
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
    delete hash_table;
    delete ast;
    return EXIT_SUCCESS;
}