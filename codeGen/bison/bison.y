%{
    #include <stdio.h>
    #include "../inc/HashTable.h"
    #include "../inc/Ast.h"
    #include "../inc/Asm.h"
    #include "../inc/Singleton.h"
    #include "../inc/WriteAdapter.h"
    #include "../inc/ASM_VARS.h"
    #include "../inc/StringGenerator.h"

    extern FILE *yyin;
    extern int yylineno;
    extern int ch;
    extern char *yytext;
    extern void yyerror(const char *);
    extern void yyerror(const char *, const char *);
    extern HashTable* hash_table;
    extern int yylex();
    AST::Ast* ast;
    bool valid = true;
%}

%token INT VOID STRING FUNCTION
%token IF ELSE PRINT FOR LEX_ERROR
%token ID CONST_INT CONST_DOUBLE
%token JUMP MARK_TOK RETURN
%token DEC INC EQ LEQ GEQ NEQ AND


%union {
    int type;
    char* str;
    AST::BaseAST* expr;
    // TODO Fix unary priority
}

%type <str> ID CONST_INT ID_REC STRING ID_LOC
%type <type> INT VOID FUNCTION FUNC_T
%type <expr> EXPR2 EXPR1 EXPR0 EXPR CONST DEFVAR UNDEFVAR VAR EVAL
%type <expr> MARK GOTO ATOM CALL ARGS ARG RET
%type <expr> ANYVAR LOOP ATOMLLIST IFELSE ELSEIF BODY
%type <expr> PROTO LEVAL

%left '+' '-'
%left '*' '/' '%'
%left uminus
%left uplus

%%
START:  ATOM { ast->AddToLink($1); }
        | START ATOM { ast->AddToLink($2); }

BODY: ATOMLLIST { $$ = ast->GetBody($1); }

ATOMLLIST:   ATOM { $$ = ast->GetBodyLList(nullptr, $1); }
        | ATOM BODY { $$ = ast->GetBodyLList($2, $1); }

ATOM:   DEFVAR ';' { $$ = $1; }
        | UNDEFVAR ';' { $$ = $1; }
        | LOOP { $$ = $1; hash_table->gotoParent(); }
        | GOTO ';' { $$ = $1; }
        | MARK { $$ = $1; }
        | IFELSE { $$ = $1; hash_table->gotoParent(); }
        | EVAL ';' { $$ = $1; }
        | CALL ';' { $$ = $1; }
        | RET ';' { $$ = $1; }
        | PROTO ';' { $$ = $1; }
        | EXPR ';' { $$ = $1; }

PROTO:  FUNCTION FUNC_T ID { hash_table->CreateEntry($2, $3); $$ = ast->GetPrototypeFunc($2, $3); }

FUNC_T: INT { $$ = $1; }
        | VOID { $$ = $1; }

RET: RETURN VAR  { $$ = ast->GetReturn($2); }

CALL:   ID_REC  '(' ARGS ')' { $$ = ast->GetCallFunc($1, ast->GetArgs($3));}

ARGS:   ARG { $$ = ast->GetArgList(nullptr, $1); }
        | ARG ',' ARGS { $$ = ast->GetArgList($3, $1); }

ARG: EXPR { $$ = $1; }
        | '&' VAR { reinterpret_cast<AST::VariableExprAST*>($2)->setAddr(); $$ = $2; }
        | STRING {      std::string str_name = Singleton<StringGenerator>::getInstance()->Generate();
                        Singleton<FormatAcum>::getInstance()->Add(str_name, $1);
                        $$ = ast->GetString(str_name, $1);
                 }

DEFVAR: INT ID_LOC '=' EXPR  { hash_table->CreateEntry($1, $2); $$ = ast->GetVariableDef($1, $2, $4); }

UNDEFVAR: INT ID_LOC  { hash_table->CreateEntry($1, $2); $$ = ast->GetVariableUndef($1, $2); }

ANYVAR: DEFVAR { $$ = $1; }
        | EVAL { $$ = $1; }
        | ';' { $$ = nullptr; }

LOOP:   FOR '(' ANYVAR ';' EXPR ';'  LEVAL ')' ATOM { $$ = ast->GetLoop($3, $5, $7, $9); }
        | FOR '(' ANYVAR ';' EXPR ';' LEVAL ')' '{' BODY '}' { $$ = ast->GetLoop($3, $5, $7, $10); }

LEVAL:  EVAL { $$ = $1; }
        | EXPR { $$ =$1; }

MARK:   ID ':' { if (hash_table->LookupEntry($1) == nullptr) {
                    hash_table->CreateEntry(MARK_TOK, $1); $$ = ast->GetMark($1);
                 } else { yyerror("Identificator already created", $1); $$ = nullptr; } }

GOTO:   JUMP ID  { if ($2) { $$ = ast->GetJump($2); } else { $$ = nullptr; } }

IFELSE: IF '(' EXPR ')' ATOM { $$ = ast->GetIf($3, $5, nullptr); }
        | IF '(' EXPR ')' '{' BODY '}' { $$ = ast->GetIf($3, $6, nullptr); }
        | IF '(' EXPR ')' '{' BODY '}' ELSEIF { $$ = ast->GetIf($3, $6, $8); }

ELSEIF: ELSE '{' BODY '}' { $$ = ast->GetElse($3); }
        | ELSE ATOM  { $$ = ast->GetElse($2); }

EVAL: ID '=' EXPR   { if ($1) $$ = ast->GetEval($1, $3); else $$ = nullptr; }

EXPR:   EXPR0 { $$ = $1; }
        | EXPR0 '&' EXPR { $$ = ast->GetBinaryExpr('&', $1, $3); }
        | EXPR0 '^' EXPR { $$ = ast->GetBinaryExpr('^', $1, $3); }
        | EXPR0 '|' EXPR { $$ = ast->GetBinaryExpr('|', $1, $3); }
        | EXPR0 '<' EXPR { $$ = ast->GetLogicExpr('<', $1, $3); }
        | EXPR0 LEQ EXPR { $$ = ast->GetLogicExpr(oLEQ, $1, $3); }
        | EXPR0 GEQ EXPR { $$ = ast->GetLogicExpr(oGEQ, $1, $3); }
        | EXPR0 '>' EXPR { $$ = ast->GetLogicExpr('>', $1, $3); }
        | EXPR0 EQ EXPR { $$ = ast->GetLogicExpr('=', $1, $3); }
        | EXPR0 NEQ EXPR { $$ = ast->GetLogicExpr('!', $1, $3); }


EXPR0:   EXPR1 { $$ = $1; }
        | EXPR1 '+' EXPR0 { $$ = ast->GetBinaryExpr('+', $1, $3); }
        | EXPR1 '-' EXPR0 { $$ = ast->GetBinaryExpr('-', $1, $3); }

EXPR1:  EXPR2 { $$ = $1; }
        | EXPR2 '*' EXPR1 { $$ = ast->GetBinaryExpr('*', $1, $3); }
        | EXPR2 '/' EXPR1 { $$ = ast->GetBinaryExpr('/', $1, $3); }
        | EXPR2 '%' EXPR1 { $$ = ast->GetBinaryExpr('%', $1, $3); }
        | EXPR2 AND EXPR1 { $$ = ast->GetLogicExpr('&', $1, $3); }

EXPR2:  VAR { $$ = $1; }
        | '(' EXPR ')' { $$ = $2;}
        | '~' EXPR %prec uminus { $$ = ast->GetUnary('~', $2); }
        | '-' EXPR %prec uminus { $$ = ast->GetUnary('-', $2); }
        | '!' EXPR %prec uminus { $$ = ast->GetUnary('!', $2); }
        | VAR INC { reinterpret_cast<AST::VariableExprAST*>($1)->setAddr(); $$ = ast->GetUnary(oINC,  $1); }
        | INC VAR { reinterpret_cast<AST::VariableExprAST*>($2)->setAddr(); $$ = ast->GetUnary(oIINC,  $2); }
        | VAR  DEC { reinterpret_cast<AST::VariableExprAST*>($1)->setAddr(); $$ = ast->GetUnary(oDEC, $1); }
        | DEC VAR  { reinterpret_cast<AST::VariableExprAST*>($2)->setAddr(); $$ = ast->GetUnary(oIDEC, $2); }
        | CALL { $$ = $1; }


VAR:    CONST { $$ = $1; }
        | ID_REC { $$ = ast->GetVariableExpr(std::string($1)); }

ID_REC: ID { if (hash_table->LookupEntry($1) != nullptr) { $$ = $1; } else { yyerror("Var not declaration", $1); $$ = (char*)""; } }

ID_LOC: ID { if (hash_table->LookupEntryNotRecur($1) == nullptr) { $$ = $1; } else { yyerror("Var already has definition", $1); $$ = (char*)""; } }

CONST:  CONST_INT { $$ = ast->GetIntNumberExpr(atoi($1)); }

%%

void yyerror(const char *errmsg)
{
    fprintf(stderr, "Position (%d, %d): [%s] %s\n", yylineno, ch, yytext, errmsg);
    valid = false;
}

void yyerror(const char *errmsg, const char *msg)
{
    fprintf(stderr, "Position (%d, %d): [%s] %s\n", yylineno, ch, msg, errmsg);
    valid = false;
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
    AsmVars* asmVars = Singleton<AsmVars>::getInstance();
    asmVars->setIntType(INT);
    yylineno = 1;

    yyparse();

    ASM_GEN* AsmGen = new ASM_GEN(argv[2], ast);
    if (valid)
        AsmGen->Generate();

    fclose(yyin);
    delete hash_table;
    delete ast;
    delete AsmGen;
    WriteAdapter* writeAdapter = Singleton<WriteAdapter>::getInstance();
    delete writeAdapter;

    return EXIT_SUCCESS;
}