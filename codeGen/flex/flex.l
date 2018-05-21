%option noyywrap yylineno

%{
#include <stdio.h>
#include <string.h>
#include "../inc/Ast.h"
#include "../inc/parser.h"
    int ch = 1;
    HashTable* hash_table = Singleton<HashTable>::getInstance();
%}

DIGIT[0-9]
LETTER[a-zA-Z]
DELIM[();{},:]
WS[ \t]
ID ("_"|{LETTER})("_"|{LETTER}|{DIGIT})*
OP [-+*/|%&]
LOP [<>=!~]
SIGN [-+]

%%

"/*".*"*/" {
    ch += yyleng;
}

"\n" {
    ch = 1;
}

"//"(.|[ \t])*\n {
    ch += yyleng;
}

\".*\" {
    ch += yyleng;
    yylval.str = strdup(yytext);
    return STRING;
}

{OP}"=" {
    ch += yyleng;
    yylval.type = *yytext;
    hash_table->addChildScope();
    return OPME;
}


"++" {
    ch += yyleng;
    return INC;
}

"--" {
    ch += yyleng;
    return DEC;
}

"==" {
    ch += yyleng;
    return EQ;
}

"!=" {
    ch += yyleng;
    return NEQ;
}

"&&" {
    ch += yyleng;
    return AND;
}


">=" {
    ch += yyleng;
    return GEQ;
}

"<=" {
    ch += yyleng;
    return LEQ;
}

"if" {
    ch += yyleng;
    hash_table->addChildScope();
    return IF;
}

"else" {
    ch += yyleng;
    return ELSE;
}

"print" {
    ch += yyleng;
    return PRINT;
}

"for" {
    ch += yyleng;
    hash_table->addChildScope();
    return FOR;
}

"function" {
    ch += yyleng;
    return FUNCTION;

}

"int" {
    ch += yyleng;
    yylval.type = INT;
    return INT;
}

"void" {
    ch += yyleng;
    yylval.type = VOID;
    return VOID;
}

"goto" {
    ch += yyleng;
    return JUMP;
}

{SIGN}?{DIGIT}+ {
    ch += yyleng;
    yylval.str = strdup(yytext);
    return CONST_INT;
}

{ID} {
    yylval.str = strdup(yytext);
    ch += yyleng;
    return ID;
}

{DELIM} {
    ch += yyleng;
    return *yytext;
}

{WS}+ {
    ch += yyleng;
}

{OP} {
    ch += yyleng;
    return *yytext;
}

{LOP} {
    ch += yyleng;
    return *yytext;
}

. {
    printf("Unknown character (%d, %d): %s\n", yylineno, ch, yytext);
    ch += yyleng;
    return LEX_ERROR;
}
%%