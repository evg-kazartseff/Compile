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
%token JUMP MARK_TOK RETURN OPME
%token DEC INC EQ LEQ GEQ NEQ AND


%union {
    int type;
    char* str;
    AST::BaseAST* expr;
    // TODO Fix unary priority
}

%type <str> ID CONST_INT ID_REC STRING ID_LOC
%type <type> INT VOID FUNCTION FUNC_T OPME
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

BODY: ATOMLLIST { $$ = ast->Getter<AST::BodyAST>($1); }

ATOMLLIST:   ATOM { $$ = ast->Getter<AST::BodyLListAST>(nullptr, $1); }
        | ATOM BODY { $$ = ast->Getter<AST::BodyLListAST>($2, $1); }

ATOM:   DEFVAR ';' { $$ = $1; }
        | UNDEFVAR ';' { $$ = $1; }
        | LOOP { $$ = $1; }
        | GOTO ';' { $$ = $1; }
        | MARK { $$ = $1; }
        | IFELSE { $$ = $1; }
        | EVAL ';' { $$ = $1; }
        | CALL ';' { $$ = $1; }
        | RET ';' { $$ = $1; }
        | PROTO ';' { $$ = $1; }
        | EXPR ';' { $$ = $1; }

PROTO:  FUNCTION FUNC_T ID { hash_table->CreateEntry($2, $3); $$ = ast->Getter<AST::PrototypeFuncAST>($2, $3); }

FUNC_T: INT { $$ = $1; }
        | VOID { $$ = $1; }

RET: RETURN VAR  { $$ = ast->Getter<AST::ReturnAST>($2); }

CALL:   ID_REC  '(' ARGS ')' { $$ = ast->Getter<AST::CallFuncAST>($1, ast->Getter<AST::ArgsAST>($3));}

ARGS:   ARG { $$ = ast->Getter<AST::ArgListAST>(nullptr, $1); }
        | ARG ',' ARGS { $$ = ast->Getter<AST::ArgListAST>($3, $1); }

ARG: EXPR { $$ = $1; }
        | STRING {      std::string str_name = Singleton<StringGenerator>::getInstance()->Generate();
                        Singleton<FormatAcum>::getInstance()->Add(str_name, $1);
                        $$ = ast->Getter<AST::StringAST>(str_name, $1);
                 }
        | EVAL { reinterpret_cast<AST::EvalAST*>($1)->SetNeed(); $$ = $1; }

DEFVAR: INT ID_LOC '=' EXPR  { hash_table->CreateEntry($1, $2); $$ = ast->Getter<AST::VariableDefAST>($1, $2, $4); }

UNDEFVAR: INT ID_LOC  { hash_table->CreateEntry($1, $2); $$ = ast->Getter<AST::VariableUndefAST>($1, $2); }

ANYVAR: DEFVAR { $$ = $1; }
        | EVAL { $$ = $1; }
        | ';' { $$ = nullptr; }

LOOP:   FOR '(' ANYVAR ';' EXPR ';'  LEVAL ')' ATOM { $$ = ast->Getter<AST::LoopAST>($3, $5, $7, $9); hash_table->closeScope(); }
        | FOR '(' ANYVAR ';' EXPR ';' LEVAL ')' '{' BODY '}' { $$ = ast->Getter<AST::LoopAST>($3, $5, $7, $10); hash_table->closeScope(); }

LEVAL:  EVAL { $$ = $1; }
        | EXPR { $$ = $1; }

MARK:   ID ':' { if (hash_table->LookupEntry($1) == nullptr) {
                    hash_table->CreateEntry(MARK_TOK, $1); $$ = ast->Getter<AST::MarkAST>($1);
                 } else { yyerror("Identificator already created", $1); $$ = nullptr; } }

GOTO:   JUMP ID  { if ($2) { $$ = ast->Getter<AST::JumpAST>($2); } else { $$ = nullptr; } }

IFELSE: IF '(' EXPR ')' ATOM { $$ = ast->Getter<AST::IfAST>($3, $5, nullptr); hash_table->closeScope(); }
        | IF '(' EXPR ')' '{' BODY '}' { $$ = ast->Getter<AST::IfAST>($3, $6, nullptr); hash_table->closeScope(); }
        | IF '(' EXPR ')' '{' BODY '}' ELSEIF { $$ = ast->Getter<AST::IfAST>($3, $6, $8);}

ELSEIF: ELSE '{' BODY '}' { $$ = ast->Getter<AST::ElseAST>($3); hash_table->closeScope(); }
        | ELSE ATOM  { $$ = ast->Getter<AST::ElseAST>($2); hash_table->closeScope(); }

EVAL:   ID_REC '=' EXPR   { if ($1) $$ = ast->Getter<AST::EvalAST>($1, $3); else $$ = nullptr; }
        | ID_REC OPME EXPR { if ($1) $$ = ast->Getter<AST::EvalAST>($1,
                ast->Getter<AST::BinaryExprAST>($2, ast->Getter<AST::VariableExprAST>(std::string($1)), $3));
            else $$ = nullptr; }

EXPR:   EXPR0 { $$ = $1; }
        | EXPR0 '&' EXPR { $$ = ast->Getter<AST::BinaryExprAST>('&', $1, $3); }
        | EXPR0 '^' EXPR { $$ = ast->Getter<AST::BinaryExprAST>('^', $1, $3); }
        | EXPR0 '|' EXPR { $$ = ast->Getter<AST::BinaryExprAST>('|', $1, $3); }
        | EXPR0 '<' EXPR { $$ = ast->Getter<AST::LogicExprAST>('<', $1, $3); }
        | EXPR0 LEQ EXPR { $$ = ast->Getter<AST::LogicExprAST>(oLEQ, $1, $3); }
        | EXPR0 GEQ EXPR { $$ = ast->Getter<AST::LogicExprAST>(oGEQ, $1, $3); }
        | EXPR0 '>' EXPR { $$ = ast->Getter<AST::LogicExprAST>('>', $1, $3); }
        | EXPR0 EQ EXPR { $$ = ast->Getter<AST::LogicExprAST>('=', $1, $3); }
        | EXPR0 NEQ EXPR { $$ = ast->Getter<AST::LogicExprAST>('!', $1, $3); }


EXPR0:   EXPR1 { $$ = $1; }
        | EXPR1 '+' EXPR0 { $$ = ast->Getter<AST::BinaryExprAST>('+', $1, $3); }
        | EXPR1 '-' EXPR0 { $$ = ast->Getter<AST::BinaryExprAST>('-', $1, $3); }

EXPR1:  EXPR2 { $$ = $1; }
        | EXPR2 '*' EXPR1 { $$ = ast->Getter<AST::BinaryExprAST>('*', $1, $3); }
        | EXPR2 '/' EXPR1 { $$ = ast->Getter<AST::BinaryExprAST>('/', $1, $3); }
        | EXPR2 '%' EXPR1 { $$ = ast->Getter<AST::BinaryExprAST>('%', $1, $3); }
        | EXPR2 AND EXPR1 { $$ = ast->Getter<AST::LogicExprAST>('&', $1, $3); }

EXPR2:  VAR { $$ = $1; }
        | '(' EXPR ')' { $$ = $2;}
        | '~' EXPR2 { $$ = ast->Getter<AST::UnaryAST>('~', $2); }
        | '-' EXPR2 { $$ = ast->Getter<AST::UnaryAST>('-', $2); }
        | '!' EXPR2  { $$ = ast->Getter<AST::UnaryAST>('!', $2); }
        | '&' VAR  { reinterpret_cast<AST::VariableExprAST*>($2)->setAddr(); $$ = $2; }
        | VAR INC { reinterpret_cast<AST::VariableExprAST*>($1)->setAddr(); $$ = ast->Getter<AST::UnaryAST>(oINC,  $1); }
        | INC VAR { reinterpret_cast<AST::VariableExprAST*>($2)->setAddr(); $$ = ast->Getter<AST::UnaryAST>(oIINC,  $2); }
        | VAR  DEC { reinterpret_cast<AST::VariableExprAST*>($1)->setAddr(); $$ = ast->Getter<AST::UnaryAST>(oDEC, $1); }
        | DEC VAR  { reinterpret_cast<AST::VariableExprAST*>($2)->setAddr(); $$ = ast->Getter<AST::UnaryAST>(oIDEC, $2); }
        | CALL { $$ = $1; }


VAR:    CONST { $$ = $1; }
        | ID_REC { $$ = ast->Getter<AST::VariableExprAST>(std::string($1)); }

ID_REC: ID { if (hash_table->LookupEntry($1) != nullptr) { $$ = $1; } else { yyerror("Var not declaration", $1); $$ = (char*)""; } }

ID_LOC: ID { if (hash_table->LookupEntryNotRecur($1) == nullptr) { $$ = $1; } else { yyerror("Var already has definition", $1); $$ = (char*)""; } }

CONST:  CONST_INT { $$ = ast->Getter<AST::IntNumberExprAST>(atoi($1)); }

%%

void yyerror(const char *errmsg)
{
    fprintf(stderr, "Position (%d, %d): [%s] %s\n", yylineno, ch, yytext, errmsg);
    valid = false;
}

void yyerror(const char *errmsg, const char *msg)
{
    fprintf(stderr, ">Position (%d, %d): [%s] %s\n", yylineno, ch, msg, errmsg);
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
