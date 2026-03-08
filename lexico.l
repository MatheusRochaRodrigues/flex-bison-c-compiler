%{
	#include <string.h>
	#include <stdio.h>
	#include "sintatico.tab.h"
	#include "symbolTable.h"
	void yyerror(char *); 
%}

ws [ \t]
letter [A-Za-z]
digit [0-9]



%%

"//".*\n  {
	cont_lines++;
}

"var"	{
	printf("Lexic: var found\n");
	strcpy(yylval.c.str, yytext);
	return VAR;
}

"int"	{
	printf("Lexic: int found\n");
	strcpy(yylval.c.str, yytext);
	return INT;
}

"float"	{
	printf("Lexic: float found\n");
	strcpy(yylval.c.str, yytext);
	return FLOAT;
}

"string"	{
	printf("Lexic: STRING found\n");
	strcpy(yylval.c.str, yytext);
	return STRING;
}

"read"	{
	printf("Lexic: read found\n");
	strcpy(yylval.c.str, yytext);
	return READ;
}

"write"	{
	printf("Lexic: write found\n");
	strcpy(yylval.c.str, yytext);
	return WRITE;
}


"if"	{
	printf("Lexic: int found\n");
	strcpy(yylval.c.str, yytext);
	return IF;
}


"then"	{
	printf("Lexic: int found\n");
	strcpy(yylval.c.str, yytext);
	return THEN;
}


"else"	{
	printf("Lexic: int found\n");
	strcpy(yylval.c.str, yytext);
	return ELSE;
}

"while"	{
	printf("Lexic: int found\n");
	strcpy(yylval.c.str, yytext);
	return WHILE;
}

"do"	{
	printf("Lexic: int found\n");
	strcpy(yylval.c.str, yytext);
	return DO;
}


">=" {
	printf("Lexic: %c found\n", *yytext);
	// strcpy(yylval.str, yytext);
	return GE;
}

"==" {
	printf("Lexic: %c found\n", *yytext);
	// strcpy(yylval.str, yytext);
	return EQ;
}

"!=" {
	printf("Lexic: %c found\n", *yytext);
	// strcpy(yylval.str, yytext);
	return NE;
}

"<=" {
	printf("Lexic: %c found\n", *yytext);
	// strcpy(yylval.str, yytext);
	return LE;
}


{digit}+  {
	strcpy(yylval.c.str, yytext);
	return NUM;
}


{letter}({letter}|{digit}|_)*	{
	
	strcpy(yylval.c.str, yytext);
	return ID;
}



[-;:={}+*()<>/%] {
	printf("Lexic: %c found\n", *yytext);
	// strcpy(yylval.str, yytext);
	return *yytext;
}


\"[^\"\n]*\" {
    printf("Lexic: Literal String found\n");
    strcpy(yylval.c.str, yytext);
    return LITERAL_STR;
}



{ws}           { }

[\n]          { cont_lines++; }
[\r]+

. { yyerror("Invalid character"); }

%%

int yywrap (void)
{
 	return 1;
}
