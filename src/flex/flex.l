%option noyywrap yylineno

%{
#include <stdio.h>
#include <string.h>
#include "AST.h"
#include "parser.h"
#include "HashTable.h"

#define HASHTABLESIZE 128

    HashTable* hash_table = new HashTable(HASHTABLESIZE);
    int ch = 1;
%}

DIGIT[0-9]
LETTER[a-zA-Z]
DELIM[();{},]
WS[ \t]
ID ("_"|{LETTER})("_"|{LETTER}|{DIGIT})*
OP [-+*/<>?!%=~]
SIGN [-+]

%%
"\n" {
    ch = 1;
}

"/*".*"*/" {
    ch += yyleng;
}

"//"(.|[ \t])*\n {
    ch += yyleng;
}

"return" {
    ch += yyleng;
    //return 0; //RETURN;
}

"if" {
    ch += yyleng;
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
    return FOR;
}

"int" {
    ch += yyleng;
    yylval.type = INT;
    return INT;
}

"char" {
    ch += yyleng;
    yylval.type = CHAR;
    return CHAR;
}

"double" {
    ch += yyleng;
    yylval.type = DOUBLE;
    return DOUBLE;
}

{SIGN}?{DIGIT}+ {
    ch += yyleng;
    yylval.str = strdup(yytext);
    return CONST_INT;
}

{SIGN}?({DIGIT}+\.{DIGIT}+|{DIGIT}+\.|{DIGIT}+)([eE]{SIGN}?{DIGIT}+)? {
    ch += yyleng;
    yylval.str = strdup(yytext);
    return CONST_DOUBLE;
}

{ID} {
    /*hash_table->CreateEntry(yytext);
    HashNode* node;
    node = hash_table->LookupEntry(yytext);
    printf("###IDENTIFIER (%d, %d): %s\n", yylineno, ch, node->getValue().c_str());*/
    yylval.str = strdup(yytext);
    ch += yyleng;
    return ID;
}

{DELIM} {
    //printf("DELIMITER (%d, %d): %s\n", yylineno, ch, yytext);
    ch += yyleng;
}

{WS}+ {
    ch += yyleng;
}

{OP} {
    ch += yyleng;
    return *yytext;
}

. {
    printf("Unknown character (%d, %d): %s\n", yylineno, ch, yytext);
    ch += yyleng;
}
%%