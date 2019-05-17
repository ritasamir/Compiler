/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 49 "grammer.y" /* yacc.c:1909  */

	#include <vector>
	#include <string>
	using namespace std;

#line 50 "y.tab.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    OR = 258,
    AND = 259,
    NOT = 260,
    TRUE = 261,
    FALSE = 262,
    LEFT_BRACKET = 263,
    RIGHTBRACKET = 264,
    SEMICOLON = 265,
    INTWORD = 266,
    FLOATWORD = 267,
    EQUAL = 268,
    IFWORD = 269,
    WHILEWORD = 270,
    CURLEFTBRACKET = 271,
    CURRIGHTBRACKET = 272,
    ELSEWORD = 273,
    RELOP = 274,
    ID = 275,
    ARITHOP = 276,
    INT = 277,
    FLOAT = 278
  };
#endif
/* Tokens.  */
#define OR 258
#define AND 259
#define NOT 260
#define TRUE 261
#define FALSE 262
#define LEFT_BRACKET 263
#define RIGHTBRACKET 264
#define SEMICOLON 265
#define INTWORD 266
#define FLOATWORD 267
#define EQUAL 268
#define IFWORD 269
#define WHILEWORD 270
#define CURLEFTBRACKET 271
#define CURRIGHTBRACKET 272
#define ELSEWORD 273
#define RELOP 274
#define ID 275
#define ARITHOP 276
#define INT 277
#define FLOAT 278

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 57 "grammer.y" /* yacc.c:1909  */

char * charsVal;
struct {
vector<int> *trueList,*falseList;
    }bexpr;
	struct {
vector<int> *nextList;
    }stmt;
	int ival;
	float fval;
	int sType;

#line 121 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
