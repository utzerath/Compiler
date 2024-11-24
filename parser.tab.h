/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
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

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 29 "parser.y"

    int number;
    float float_number;  // Add float support
    char character;      // BinOp returns a char
    char* string;
    char* operator;
    struct ASTNode* ast; // Use ASTNode for representing AST parts, including function params and list nodes

#line 106 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
