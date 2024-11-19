/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TYPE = 258,
     NUMBER = 259,
     FLOAT_LITERAL = 260,
     SEMICOLON = 261,
     EQ = 262,
     PLUS = 263,
     MINUS = 264,
     MULTIPLY = 265,
     DIVIDE = 266,
     WRITE = 267,
     OPEN_BRACE = 268,
     CLOSE_BRACE = 269,
     OPEN_PAREN = 270,
     CLOSE_PAREN = 271,
     ID = 272,
     OPEN_BRACKET = 273,
     CLOSE_BRACKET = 274,
     RETURN = 275,
     VOID = 276,
     COMMA = 277,
     MAIN = 278,
     BOOL_LITERAL = 279,
     TYPE_BOOL = 280,
     AND_OP = 281,
     OR_OP = 282,
     NOT_OP = 283,
     IF = 284,
     ELSE = 285,
     AND = 286,
     OR = 287,
     NOT = 288,
     EQUALS = 289,
     NOT_EQUALS = 290,
     LESS_THAN = 291,
     GREATER_THAN = 292,
     LESS_EQUAL = 293,
     GREATER_EQUAL = 294
   };
#endif
/* Tokens.  */
#define TYPE 258
#define NUMBER 259
#define FLOAT_LITERAL 260
#define SEMICOLON 261
#define EQ 262
#define PLUS 263
#define MINUS 264
#define MULTIPLY 265
#define DIVIDE 266
#define WRITE 267
#define OPEN_BRACE 268
#define CLOSE_BRACE 269
#define OPEN_PAREN 270
#define CLOSE_PAREN 271
#define ID 272
#define OPEN_BRACKET 273
#define CLOSE_BRACKET 274
#define RETURN 275
#define VOID 276
#define COMMA 277
#define MAIN 278
#define BOOL_LITERAL 279
#define TYPE_BOOL 280
#define AND_OP 281
#define OR_OP 282
#define NOT_OP 283
#define IF 284
#define ELSE 285
#define AND 286
#define OR 287
#define NOT 288
#define EQUALS 289
#define NOT_EQUALS 290
#define LESS_THAN 291
#define GREATER_THAN 292
#define LESS_EQUAL 293
#define GREATER_EQUAL 294




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 29 "parser.y"
{
    int number;
    float float_number;  // Add float support
    char character;      // BinOp returns a char
    char* string;
    char* operator;
    struct ASTNode* ast; // Use ASTNode for representing AST parts, including function params and list nodes
}
/* Line 1529 of yacc.c.  */
#line 136 "parser.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

