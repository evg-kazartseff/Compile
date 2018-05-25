%option noyywrap yylineno

%{
#include "../inc/Ast.h"
#include "../inc/parser.h"
#include <cstdio>
    int ch = 1;
    HashTable* hash_table = Singleton<HashTable>::getInstance();
    void comment(char end);
%}

DIGIT[0-9]
LETTER[a-zA-Z]
DELIM[();{},:]
WS[ \t]
ID ("_"|{LETTER})("_"|{LETTER}|{DIGIT})*
OP [-+*/|%&]
LOP [<>=!~]
SIGN [-+]

%x COMM

%%

\/(\*|\/) {
   comment(yytext[1]);
    ch += yyleng;
}


\".*\" {
    ch += yyleng;
    yylval.str = strdup(yytext);
    return STRING;
}

"\n" {
    ch = 1;
}

{OP}"=" {
    ch += yyleng;
    yylval.type = *yytext;
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
    return IF;
}

"else" {
    ch += yyleng;
    return ELSE;
}

"for" {
    ch += yyleng;
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

"return" {
    ch += yyleng;
    return RETURN;
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


void comment(char end)
{
	int pos = ch;
	int posl = yylineno;
	int c, c1;
	if (end == '/') {
		while ((c = yyinput()) != '\n' && c != 0) { ++ch; }
		++ch;
		return;
	}

	loop:
	while ((c = yyinput()) != '*' && c != 0) {
		++ch;
		if (c == '\n') ch = 1;
	}

	if ((c1 = yyinput()) != '/' && c != 0) {
		ch += 2;
		unput(c1);
		goto loop;
	}


	if (c == 0) {
		printf("unterminated comment: %d:%d\n", posl, pos);
	}
}