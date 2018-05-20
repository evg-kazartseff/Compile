%{
    #include <stdio.h>
    #include "../inc/HashTable.h"
    #include "../inc/Ast.h"
    #include "../inc/Asm.h"
    #include "../inc/Singleton.h"
    #include "../inc/WriteAdapter.h"
    #include "../inc/ASM_VARS.h"
    #include "../inc/StringNameGenerator.h"

    extern FILE *yyin;
    extern int yylineno;
    extern int ch;
    extern char *yytext;
    extern void yyerror(const char *);
    extern void yyerror(const char *, const char *);
    HashTable* hash_table = Singleton<HashTable>::getInstance();
    extern int yylex();
    AST::Ast* ast;
%}

%token INT CHAR DOUBLE STRING
%token IF ELSE PRINT FOR LEX_ERROR
%token ID CONST_INT CONST_DOUBLE
%token JUMP MARK_TOK RETURN

%union {
    int type;
    char* str;
    AST::BaseAST* expr;
    // TODO Fix unary priority

}

%type <str> ID CONST_INT CONST_DOUBLE ID_TOK STRING
%type <type> INT CHAR DOUBLE TYPE
%type <expr> EXPR2 EXPR1 EXPR0 EXPR CONST DEFVAR UNDEFVAR VAR EVAL
%type <expr> MARK GOTO ATOM CALL ARGS ARG RET
%type <expr> ANYVAR LOOP ATOMLLIST COND IFELSE ELSEIF BODY LEVAL

%%
START:  ATOM { ast->AddToLink($1); }
        | START ATOM { ast->AddToLink($2); }

BODY: ATOMLLIST { $$ = ast->GetBody($1); }

ATOMLLIST:   ATOM { $$ = ast->GetBodyLList(nullptr, $1); }
        | ATOM BODY { $$ = ast->GetBodyLList($2, $1); }

ATOM:   DEFVAR { $$ = $1; }
        | UNDEFVAR { $$ = $1; }
        | LOOP { $$ = $1; }
        | GOTO { $$ = $1; }
        | MARK { $$ = $1; }
        | IFELSE { $$ = $1; }
        | EVAL { $$ = $1; }
        | CALL { $$ = $1; }
        | RET { $$ = $1; }

RET: RETURN VAR ';' { $$ = ast->GetReturn($2); }

CALL:   ID  '(' ARGS ')'  ';' { $$ = ast->GetCallFunc($1, ast->GetArgs($3));}

ARGS:   ARG { $$ = ast->GetArgList(nullptr, $1); }
        | ARG ',' ARGS { $$ = ast->GetArgList($3, $1); }

ARG: VAR { $$ = $1; }
        | '&' VAR { reinterpret_cast<AST::VariableExprAST*>($2)->setAddr(); $$ = $2; }
        | STRING {      std::string str_name = Singleton<StringNameGenerator>::getInstance()->Generate();
                        Singleton<FormatAcum>::getInstance()->Add(str_name, $1);
                        $$ = ast->GetString(str_name, $1);
                 }

DEFVAR: TYPE ID '=' EXPR ';' { hash_table->CreateEntry($1, $2); $$ = ast->GetVariableDef($1, $2, $4); }

UNDEFVAR: TYPE ID ';' { hash_table->CreateEntry($1, $2); $$ = ast->GetVariableUndef($1, $2); }

ANYVAR: DEFVAR { $$ = $1; }
        | EVAL { $$ = $1; }
        | ';' { $$ = nullptr; }

LOOP:   FOR '(' ANYVAR  COND ';'  LEVAL ')' ATOM { $$ = ast->GetLoop($3, $4, $6, $8); }
        | FOR '(' ANYVAR  COND ';' LEVAL ')' '{' BODY '}' { $$ = ast->GetLoop($3, $4, $6, $9); }
LEVAL:  ID '=' EXPR { $$ = ast->GetEval($1, $3); }

COND:   VAR { $$ = $1; }
        | VAR '<' COND { $$ = ast->GetLogicExpr('<', $1, $3); }
        | VAR '>' COND { $$ = ast->GetLogicExpr('>', $1, $3); }
        | VAR "==" COND {$$ = ast->GetLogicExpr('=', $1, $3); }

MARK:   ID ':' { if (hash_table->LookupEntry($1) == nullptr) {
                    hash_table->CreateEntry(MARK_TOK, $1); $$ = ast->GetMark($1);
                 } else { yyerror("Identificator already created", $1); $$ = nullptr; } }

GOTO:   JUMP ID ';' { if ($2) { $$ = ast->GetJump($2); } else { $$ = nullptr; } }

IFELSE: IF '(' COND ')' ATOM { $$ = ast->GetIf($3, $5, nullptr); }
        | IF '(' COND ')' '{' BODY '}' { $$ = ast->GetIf($3, $6, nullptr); }
        | IF '(' COND ')' '{' BODY '}' ELSEIF { $$ = ast->GetIf($3, $6, $8); }

ELSEIF: ELSE '{' BODY '}' { $$ = ast->GetElse($3); }
        | ELSE ATOM  { $$ = ast->GetElse($2); }

EVAL: ID '=' EXPR ';'  { if ($1) $$ = ast->GetEval($1, $3); else $$ = nullptr; }

EXPR:   EXPR0 { $$ = $1; }
        | EXPR0 '&' EXPR { $$ = ast->GetBinaryExpr('&', $1, $3); }
        | EXPR0 '^' EXPR { $$ = ast->GetBinaryExpr( '^', $1, $3); }
        | EXPR0 '|' EXPR { $$ = ast->GetBinaryExpr( '|', $1, $3); }


EXPR0:   EXPR1 { $$ = $1; }
        | EXPR1 '+' EXPR0 { $$ = ast->GetBinaryExpr( '+', $1, $3); }
        | EXPR1 '-' EXPR0 { $$ = ast->GetBinaryExpr( '-', $1, $3); }

EXPR1:  EXPR2 { $$ = $1; }
        | EXPR2 '*' EXPR1 { $$ = ast->GetBinaryExpr( '*', $1, $3); }
        | EXPR2 '/' EXPR1 { $$ = ast->GetBinaryExpr( '/', $1, $3); }
        | EXPR2 '%' EXPR1 { $$ = ast->GetBinaryExpr( '%', $1, $3); }

EXPR2:  VAR { $$ = $1; }
        | '(' EXPR ')' { $$ = $2;}
        | '~' EXPR { $$ = ast->GetUnary('~', $2); }
        | '!' EXPR { $$ = ast->GetUnary('!', $2); }

VAR:    CONST { $$ = $1; }
        | ID_TOK { $$ = ast->GetVariableExpr(std::string($1)); }

ID_TOK: ID { if (hash_table->LookupEntry($1) != nullptr) { $$ = $1; } else { yyerror("Var not declaration"); $$ = (char*)""; } }

CONST:  CONST_INT { $$ = ast->GetIntNumberExpr(atoi($1)); }
        | CONST_DOUBLE { $$ = ast->GetDoubleNumberExpr(atof($1)); }

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
    if(argc < 3) {
        fprintf(stderr, "\nNot enough arguments. Please specify filename.\n");
        return EXIT_FAILURE;
    }
    if((yyin = fopen(argv[1], "r")) == nullptr) {
        fprintf(stderr, "\nCannot open file %s.\n", argv[1]);
        return EXIT_FAILURE;
    }
    if(argv[2] == nullptr) {
        fprintf(stderr, "Specify the output file\n");
        return EXIT_FAILURE;
    }
    ast = new AST::Ast();
    ch = 1;
    yylineno = 1;

    yyparse();
    AsmVars* asmVars = Singleton<AsmVars>::getInstance();
    asmVars->setDoubleType(DOUBLE);
    asmVars->setIntType(INT);
    ASM_GEN* AsmGen = new ASM_GEN(argv[2], ast);
    AsmGen->Generate();
    fclose(yyin);
    delete hash_table;
    delete ast;
    delete AsmGen;
    WriteAdapter* writeAdapter = Singleton<WriteAdapter>::getInstance();
    delete writeAdapter;
    return EXIT_SUCCESS;
}