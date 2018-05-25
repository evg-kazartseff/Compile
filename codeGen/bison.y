%{
    #include "../inc/Ast.h"
    #include "../inc/asm/Asm.h"
    #include "../inc/asm/Asm_vars.h"
    #include "../inc/helpers/Singleton.h"
    #include "../inc/helpers/WriteAdapter.h"
    #include "../inc/helpers/StringGenerator.h"
    #include "../inc/HashTable/HashTable.h"

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
%token FOR IF ELSE LEX_ERROR
%token ID CONST_INT CONST_DOUBLE
%token JUMP MARK_TOK RETURN OPME
%token DEC INC EQ LEQ GEQ NEQ AND


%union {
    int type;
    char* str;
    AST::BaseAST* expr;
}

%type <str> ID CONST_INT id_rec STRING id_loc
%type <type> INT VOID FUNCTION func_type OPME
%type <expr> expr2 expr1 expr0 expr const defvar undefvar var eval
%type <expr> mark goto atom call args arg ret
%type <expr> anyvar loop atomlist condition else body
%type <expr> prototype loop_inc operatos var_only

%start start
%%

start: body { ast->AddToLink($1); }
        ;

body:   atomlist { $$ = ast->Getter<AST::BodyAST>($1); }
        ;

atomlist:   atom { $$ = ast->Getter<AST::BodyLListAST>(nullptr, $1); }
            | atom body { $$ = ast->Getter<AST::BodyLListAST>($2, $1); }
            ;

atom:   defvar ';' { $$ = $1; }
        | undefvar ';' { $$ = $1; }
        | goto ';' { $$ = $1; }
        | mark { $$ = $1; }
        | operatos { $$ = $1; hash_table->closeScope(); }
        | eval ';' { $$ = $1; }
        | call ';' { $$ = $1; }
        | ret ';' { $$ = $1; }
        | prototype ';' { $$ = $1; }
        | expr ';' { $$ = $1; }
        ;

prototype:  FUNCTION func_type id_loc { if ($3) { hash_table->CreateEntry($2, std::string($3));
                                $$ = ast->Getter<AST::PrototypeFuncAST>($2, std::string($3)); }
                            else $$ = nullptr; free($3); }
        ;

func_type: INT { $$ = $1; }
        | VOID { $$ = $1; }
        ;

ret:    RETURN var  { $$ = ast->Getter<AST::ReturnAST>($2); }
        ;

call:   id_rec  '(' args ')' { if ($1) $$ = ast->Getter<AST::CallFuncAST>(std::string($1), ast->Getter<AST::ArgsAST>($3)); else $$ = nullptr; free($1); }
        ;

args:   arg { $$ = ast->Getter<AST::ArgListAST>(nullptr, $1); }
        | arg ',' args { $$ = ast->Getter<AST::ArgListAST>($3, $1); }
        ;

arg: expr { $$ = $1; }
        | STRING {      std::string str_name = Singleton<StringGenerator>::getInstance()->Generate();
                        Singleton<FormatAcum>::getInstance()->Add(str_name, std::string($1));
                        $$ = ast->Getter<AST::StringAST>(str_name, std::string($1));
                        free($1);
                 }
        | eval { ($1)->setNeed(); $$ = $1; }
        ;

defvar: INT id_loc '=' expr  { if ($2) { hash_table->CreateEntry($1, std::string($2)); $$ = ast->Getter<AST::VariableDefAST>($1, std::string($2), $4); } else $$ = nullptr; free($2); }
        ;

undefvar: INT id_loc  { if ($2) { hash_table->CreateEntry($1, std::string($2)); $$ = ast->Getter<AST::VariableUndefAST>($1, std::string($2)); } else $$ = nullptr; free($2); }
        ;

anyvar: defvar { $$ = $1; }
        | eval { $$ = $1; }
        | ';' { $$ = nullptr; }
        ;

loop_inc:  eval { $$ = $1; }
        | expr { $$ = $1; }
        ;

mark:   id_rec ':' { if ($1) { hash_table->CreateEntry(MARK_TOK, std::string($1)); $$ = ast->Getter<AST::MarkAST>(std::string($1)); } else $$ = nullptr; free($1); }
        ;

goto:   JUMP id_rec  { if ($2) { $$ = ast->Getter<AST::JumpAST>(std::string($2)); } else { $$ = nullptr; } free($2); }
        ;

operatos:  condition { $$ = $1; }
        | loop { $$ = $1; }
        ;

condition: if_start '(' expr ')' '{' body '}' { $$ = ast->Getter<AST::IfAST>($3, $6, nullptr); }
        | if_start '(' expr ')' '{' body '}' else { $$ = ast->Getter<AST::IfAST>($3, $6, $8); }
        ;

if_start:   IF { hash_table->addChildScope(); }
        ;

else:   else_start '{' body '}' { $$ = ast->Getter<AST::ElseAST>($3); }
        ;

else_start: ELSE { hash_table->closeScope(); hash_table->addChildScope(); }
        ;

loop:   loop_start '(' anyvar ';' expr ';' loop_inc ')' '{' body '}' { $$ = ast->Getter<AST::LoopAST>($3, $5, $7, $10); }
        ;

loop_start: FOR { hash_table->addChildScope(); }
        ;

eval:   id_rec '=' expr   { if ($1) $$ = ast->Getter<AST::EvalAST>($1, $3); else $$ = nullptr; free($1); }
        | id_rec OPME expr { if ($1) $$ = ast->Getter<AST::EvalAST>(std::string($1),
                ast->Getter<AST::BinaryExprAST>($2, ast->Getter<AST::VariableExprAST>(std::string($1)), $3));
            else $$ = nullptr; free($1); }
        ;

expr:   expr0 { $$ = $1; }
        | expr0 '&' expr { $$ = ast->Getter<AST::BinaryExprAST>('&', $1, $3); }
        | expr0 '^' expr { $$ = ast->Getter<AST::BinaryExprAST>('^', $1, $3); }
        | expr0 '|' expr { $$ = ast->Getter<AST::BinaryExprAST>('|', $1, $3); }
        | expr0 '<' expr { $$ = ast->Getter<AST::LogicExprAST>('<', $1, $3); }
        | expr0 LEQ expr { $$ = ast->Getter<AST::LogicExprAST>(oLEQ, $1, $3); }
        | expr0 GEQ expr { $$ = ast->Getter<AST::LogicExprAST>(oGEQ, $1, $3); }
        | expr0 '>' expr { $$ = ast->Getter<AST::LogicExprAST>('>', $1, $3); }
        | expr0 EQ expr { $$ = ast->Getter<AST::LogicExprAST>('=', $1, $3); }
        | expr0 NEQ expr { $$ = ast->Getter<AST::LogicExprAST>('!', $1, $3); }
        ;

expr0:   expr1 { $$ = $1; }
        | expr1 '+' expr0 { $$ = ast->Getter<AST::BinaryExprAST>('+', $1, $3); }
        | expr1 '-' expr0 { $$ = ast->Getter<AST::BinaryExprAST>('-', $1, $3); }
        ;

expr1:  expr2 { $$ = $1; }
        | expr2 '*' expr1 { $$ = ast->Getter<AST::BinaryExprAST>('*', $1, $3); }
        | expr2 '/' expr1 { $$ = ast->Getter<AST::BinaryExprAST>('/', $1, $3); }
        | expr2 '%' expr1 { $$ = ast->Getter<AST::BinaryExprAST>('%', $1, $3); }
        | expr2 AND expr1 { $$ = ast->Getter<AST::LogicExprAST>('&', $1, $3); }
        ;

expr2:  var { $$ = $1; }
        | '(' expr ')' { $$ = $2;}
        | '~' expr2 { $$ = ast->Getter<AST::UnaryAST>('~', $2); }
        | '-' expr2 { $$ = ast->Getter<AST::UnaryAST>('-', $2); }
        | '!' expr2  { $$ = ast->Getter<AST::UnaryAST>('!', $2); }
        | '*' expr2  { $$ = ast->Getter<AST::UnaryAST>('*', $2); }
        | '&' var_only  { ($2)->setNeed(); $$ = $2; }
        | var_only INC { ($1)->setNeed(); $$ = ast->Getter<AST::UnaryAST>(oINC,  $1); }
        | INC var_only { ($2)->setNeed(); $$ = ast->Getter<AST::UnaryAST>(oIINC,  $2); }
        | var_only  DEC { ($1)->setNeed(); $$ = ast->Getter<AST::UnaryAST>(oDEC, $1); }
        | DEC var_only  { ($2)->setNeed(); $$ = ast->Getter<AST::UnaryAST>(oIDEC, $2); }
        | call { $$ = $1; }
        ;

var_only:   id_rec { if ($1) $$ = ast->Getter<AST::VariableExprAST>(std::string($1)); else $$ = nullptr; free($1); }
        ;

var:    const { $$ = $1; }
        | id_rec { if ($1) $$ = ast->Getter<AST::VariableExprAST>(std::string($1)); else $$ = nullptr; free($1); }
        ;

id_rec: ID { if (hash_table->LookupEntry($1) != nullptr) { $$ = $1; } else { yyerror("Var not declaration", $1); $$ = nullptr; free($1); } }
        ;

id_loc: ID { if (hash_table->LookupEntryNotRecur($1) == nullptr) { $$ = $1; } else { yyerror("Var already has definition", $1); $$ = nullptr; free($1); } }
        ;

const:  CONST_INT { $$ = ast->Getter<AST::IntNumberExprAST>(atoi($1)); free($1); }
        ;
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
