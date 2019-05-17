%{
	#include "y.tab.h"
	#include <bits/stdc++.h>
	int lineCounter = 1;

extern int printLineNumber(int num);
%}

letter [a-zA-Z]
digit [0-9]
digits {digit}+
relop "=="|"!="|">="|"<="|">"|"<"
inum "-"?{digits}
fnum "-"?{digits}.{digits}("E"{digits}+)?
id {letter}({letter}|{digit})*
arithop "+"|"-"|"*"|"/"


%%
"||" {return OR;}
"&&" {return AND;}
"!"  {return NOT;}
"true" {return TRUE;}
"false" {return FALSE;}
"(" {return LEFT_BRACKET;}
")" {return RIGHTBRACKET;}
";" {return SEMICOLON;}
"int" {return INTWORD;}
"float" {return FLOATWORD;}
"=" {return EQUAL;}
"if" {return IFWORD;}
"while" {return WHILEWORD;}
"{"  {return CURLEFTBRACKET;}
"}"  {return CURRIGHTBRACKET;}
"else" {return ELSEWORD;}
"minus" {return MINUS;}
\n {++lineCounter; printLineNumber(lineCounter);}

{relop} {yylval.charsVal = strdup(yytext); return RELOP;}
{inum}   {yylval.ival  = atoi(yytext);return INT;}
{fnum}    {yylval.fval = atof(yytext); return FLOAT;}
{id}     {yylval.charsVal = strdup(yytext); return ID;}
{arithop}  {yylval.charsVal = strdup(yytext); return ARITHOP;}
%%


int yywrap() {
   // open next reference or source file and start scanning
   return -1;
}
