/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 1 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AST.h"
#include "symbolTable.h"
#include "semantic.h"
#include "codeGenerator.h"
#include "optimizer.h"
#define YYDEBUG 1
#define TABLE_SIZE 100

extern int yylex();   // Declare yylex, the lexer function
extern int yyparse(); // Declare yyparse, the parser function
extern FILE* yyin;    // Declare yyin, the file pointer for the input file
extern int yylineno;  // Declare yylineno, the line number counter
extern TAC* tacHead;  // Declare the head of the linked list of TAC entries

void yyerror(const char* s);
char* currentFunctionName = NULL;
char* tempFunctionName = NULL;  // Temporary variable for holding the function name for parameters


ASTNode* root = NULL; 
SymbolTable* symTab = NULL;
Symbol* symbol = NULL;


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

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
/* Line 193 of yacc.c.  */
#line 211 "parser.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 224 "parser.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   310

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  64
/* YYNRULES -- Number of states.  */
#define YYNSTATES  122

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   294

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     8,     9,    12,    13,    24,    25,    28,
      30,    42,    44,    46,    47,    50,    51,    52,    55,    56,
      58,    62,    66,    69,    70,    74,    81,    82,    85,    88,
      93,    97,   100,   104,   106,   108,   110,   116,   124,   126,
     131,   133,   137,   141,   145,   148,   152,   156,   160,   164,
     168,   172,   174,   176,   178,   180,   185,   189,   194,   195,
     197,   201,   203,   205,   207
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      41,     0,    -1,    42,    55,    43,    48,    -1,    -1,    54,
      42,    -1,    -1,    21,    23,    15,    16,    44,    49,    13,
      55,    14,    50,    -1,    -1,    54,    42,    -1,    17,    -1,
      47,    45,    15,    49,    52,    16,    13,    55,    53,    14,
      50,    -1,    21,    -1,     3,    -1,    -1,    46,    48,    -1,
      -1,    -1,     3,    17,    -1,    -1,    51,    -1,    51,    22,
      52,    -1,    20,    60,     6,    -1,    20,     6,    -1,    -1,
      57,    17,     6,    -1,    57,    17,    18,     4,    19,     6,
      -1,    -1,    56,    55,    -1,    54,    55,    -1,    59,     7,
      60,     6,    -1,    12,    60,     6,    -1,    61,     6,    -1,
      13,    55,    14,    -1,    58,    -1,     3,    -1,    25,    -1,
      29,    15,    60,    16,    56,    -1,    29,    15,    60,    16,
      56,    30,    56,    -1,    17,    -1,    17,    18,    60,    19,
      -1,    61,    -1,    60,    63,    60,    -1,    60,    31,    60,
      -1,    60,    32,    60,    -1,    33,    60,    -1,    60,    34,
      60,    -1,    60,    35,    60,    -1,    60,    36,    60,    -1,
      60,    37,    60,    -1,    60,    38,    60,    -1,    60,    39,
      60,    -1,    24,    -1,     4,    -1,     5,    -1,    17,    -1,
      17,    18,    60,    19,    -1,    15,    60,    16,    -1,    17,
      15,    62,    16,    -1,    -1,    60,    -1,    60,    22,    62,
      -1,     8,    -1,     9,    -1,    10,    -1,    11,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    94,    94,   109,   110,   121,   121,   140,   141,   151,
     169,   187,   188,   192,   193,   204,   212,   221,   236,   239,
     242,   252,   256,   261,   283,   290,   308,   313,   318,   327,
     334,   340,   344,   350,   355,   356,   360,   364,   370,   376,
     386,   389,   397,   401,   405,   409,   413,   417,   421,   425,
     429,   433,   437,   441,   445,   449,   456,   463,   469,   472,
     475,   481,   482,   483,   484
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TYPE", "NUMBER", "FLOAT_LITERAL",
  "SEMICOLON", "EQ", "PLUS", "MINUS", "MULTIPLY", "DIVIDE", "WRITE",
  "OPEN_BRACE", "CLOSE_BRACE", "OPEN_PAREN", "CLOSE_PAREN", "ID",
  "OPEN_BRACKET", "CLOSE_BRACKET", "RETURN", "VOID", "COMMA", "MAIN",
  "BOOL_LITERAL", "TYPE_BOOL", "\"&&\"", "\"||\"", "\"!\"", "IF", "ELSE",
  "AND", "OR", "NOT", "EQUALS", "NOT_EQUALS", "LESS_THAN", "GREATER_THAN",
  "LESS_EQUAL", "GREATER_EQUAL", "$accept", "Program", "GlobalDeclList",
  "MainFunc", "@1", "SetFunctionName", "FuncDecl", "ReturnType",
  "FuncDeclList", "scope_enter", "scope_exit", "Param", "ParamList",
  "ReturnStmt", "VarDecl", "StmtList", "Stmt", "Type", "IfStmt", "LValue",
  "Expr", "FuncCall", "ArgList", "BinOp", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    40,    41,    42,    42,    44,    43,    42,    42,    45,
      46,    47,    47,    48,    48,    49,    50,    51,    52,    52,
      52,    53,    53,    53,    54,    54,    55,    55,    55,    56,
      56,    56,    56,    56,    57,    57,    58,    58,    59,    59,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    61,    62,    62,
      62,    63,    63,    63,    63
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     0,     2,     0,    10,     0,     2,     1,
      11,     1,     1,     0,     2,     0,     0,     2,     0,     1,
       3,     3,     2,     0,     3,     6,     0,     2,     2,     4,
       3,     2,     3,     1,     1,     1,     5,     7,     1,     4,
       1,     3,     3,     3,     2,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     4,     3,     4,     0,     1,
       3,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,    34,    35,     0,    26,     3,     0,     1,     0,    26,
      38,     0,    26,     0,    26,    33,     0,     0,     4,     0,
      52,    53,     0,    54,    51,     0,     0,    40,     0,    58,
       0,     0,    28,     0,    13,    27,     0,    31,    24,     0,
       0,     0,    44,    30,    61,    62,    63,    64,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    32,    59,     0,
       0,     0,     0,    12,    11,    13,     0,     2,     0,     0,
      56,     0,    42,    43,    45,    46,    47,    48,    49,    50,
      41,    58,    57,    39,     0,     0,    14,     9,     0,    29,
       0,    55,    60,    36,     5,    15,    25,     0,    15,    18,
      37,     0,     0,    19,     0,    26,    17,    18,     0,     0,
      20,    26,    16,    23,     6,     0,     0,    22,     0,    16,
      21,    10
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     3,     4,    34,    98,    88,    65,    66,    67,    99,
     114,   103,   104,   116,    12,    13,    14,     6,    15,    16,
      58,    27,    59,    56
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -64
static const yytype_int16 yypact[] =
{
       0,   -64,   -64,    12,   281,     0,    -4,   -64,   272,   281,
      36,     3,   281,    -1,   281,   -64,    17,    13,   -64,    -2,
     -64,   -64,   272,    60,   -64,   272,    21,   -64,    25,   272,
     272,   272,   -64,    26,     5,   -64,   272,   -64,   -64,    46,
     121,   272,   -64,   -64,   -64,   -64,   -64,   -64,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   -64,   153,    55,
     162,   194,    61,   -64,   -64,     5,    62,   -64,    78,    58,
     -64,   203,    27,   244,    59,    59,    59,    59,    59,    59,
     235,   272,   -64,   -64,   166,    64,   -64,   -64,    67,   -64,
      84,   -64,   -64,    63,   -64,   -64,   -64,   166,   -64,    88,
     -64,    82,    79,    75,    83,   281,   -64,    88,    87,    89,
     -64,   281,   -64,    85,   -64,    68,    92,   -64,   112,   -64,
     -64,   -64
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -64,   -64,   106,   -64,   -64,   -64,   -64,   -64,    54,    28,
       6,   -64,    20,   -64,    10,    -7,   -63,   -64,   -64,   -64,
      -8,    -3,    43,   -64
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      26,    17,    28,     1,    38,    32,    17,    35,    63,    17,
       5,    17,     7,    19,    40,     5,    39,    42,    31,    37,
      33,    93,    60,    61,    36,     2,    64,    43,    68,    44,
      45,    46,    47,    71,   100,    44,    45,    46,    47,    57,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    62,
      69,    29,    48,    49,    30,    50,    51,    52,    53,    54,
      55,    50,    51,    52,    53,    54,    55,    44,    45,    46,
      47,    82,    20,    21,   117,    29,    85,    90,    41,    87,
      94,    17,    95,    22,    89,    23,    44,    45,    46,    47,
      96,   102,    24,    97,    17,   105,   106,   107,   109,   108,
     111,    25,    17,   112,   113,   115,   119,   118,    17,    48,
      49,    18,    50,    51,    52,    53,    54,    55,   120,    86,
      44,    45,    46,    47,    92,   121,   101,   110,     0,    44,
      45,    46,    47,     0,     0,     0,     0,    70,     0,     0,
       0,     0,     0,    48,    49,     0,    50,    51,    52,    53,
      54,    55,    48,    49,     0,    50,    51,    52,    53,    54,
      55,    44,    45,    46,    47,     0,     0,     0,     0,     0,
      44,    45,    46,    47,     0,    81,     0,     0,     8,     9,
       0,    83,     0,    10,    48,    49,     0,    50,    51,    52,
      53,    54,    55,    48,    49,    11,    50,    51,    52,    53,
      54,    55,    44,    45,    46,    47,     0,     0,     0,     0,
      84,    44,    45,    46,    47,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,    48,    49,     0,    50,    51,
      52,    53,    54,    55,    48,    49,     0,    50,    51,    52,
      53,    54,    55,    44,    45,    46,    47,     0,     0,     0,
       0,     0,    44,    45,    46,    47,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,     0,    50,
      51,    52,    53,    54,    55,    48,    20,    21,    50,    51,
      52,    53,    54,    55,     1,     0,     0,    22,     0,    23,
       0,     0,     0,     8,     9,     0,    24,     0,    10,     0,
       0,     0,     0,     0,     0,    25,     2,     0,     0,     0,
      11
};

static const yytype_int8 yycheck[] =
{
       8,     4,     9,     3,     6,    12,     9,    14,     3,    12,
       0,    14,     0,    17,    22,     5,    18,    25,    15,     6,
      21,    84,    30,    31,     7,    25,    21,     6,    36,     8,
       9,    10,    11,    41,    97,     8,     9,    10,    11,    14,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    23,
       4,    15,    31,    32,    18,    34,    35,    36,    37,    38,
      39,    34,    35,    36,    37,    38,    39,     8,     9,    10,
      11,    16,     4,     5,     6,    15,    15,    19,    18,    17,
      16,    84,    15,    15,     6,    17,     8,     9,    10,    11,
       6,     3,    24,    30,    97,    13,    17,    22,   105,    16,
      13,    33,   105,    14,   111,    20,    14,   115,   111,    31,
      32,     5,    34,    35,    36,    37,    38,    39,     6,    65,
       8,     9,    10,    11,    81,   119,    98,   107,    -1,     8,
       9,    10,    11,    -1,    -1,    -1,    -1,    16,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    34,    35,    36,    37,
      38,    39,    31,    32,    -1,    34,    35,    36,    37,    38,
      39,     8,     9,    10,    11,    -1,    -1,    -1,    -1,    -1,
       8,     9,    10,    11,    -1,    22,    -1,    -1,    12,    13,
      -1,    19,    -1,    17,    31,    32,    -1,    34,    35,    36,
      37,    38,    39,    31,    32,    29,    34,    35,    36,    37,
      38,    39,     8,     9,    10,    11,    -1,    -1,    -1,    -1,
      16,     8,     9,    10,    11,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    31,    32,    -1,    34,    35,
      36,    37,    38,    39,    31,    32,    -1,    34,    35,    36,
      37,    38,    39,     8,     9,    10,    11,    -1,    -1,    -1,
      -1,    -1,     8,     9,    10,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    34,
      35,    36,    37,    38,    39,    31,     4,     5,    34,    35,
      36,    37,    38,    39,     3,    -1,    -1,    15,    -1,    17,
      -1,    -1,    -1,    12,    13,    -1,    24,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    25,    -1,    -1,    -1,
      29
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    25,    41,    42,    54,    57,     0,    12,    13,
      17,    29,    54,    55,    56,    58,    59,    61,    42,    17,
       4,     5,    15,    17,    24,    33,    60,    61,    55,    15,
      18,    15,    55,    21,    43,    55,     7,     6,     6,    18,
      60,    18,    60,     6,     8,     9,    10,    11,    31,    32,
      34,    35,    36,    37,    38,    39,    63,    14,    60,    62,
      60,    60,    23,     3,    21,    46,    47,    48,    60,     4,
      16,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    22,    16,    19,    16,    15,    48,    17,    45,     6,
      19,    19,    62,    56,    16,    15,     6,    30,    44,    49,
      56,    49,     3,    51,    52,    13,    17,    22,    16,    55,
      52,    13,    14,    55,    50,    20,    53,     6,    60,    14,
       6,    50
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 94 "parser.y"
    {
        printf("DEBUG: Parsed complete program with global declarations, statements, main function, and other functions.\n");
        (yyval.ast) = createNode(NodeType_Program);
        (yyval.ast)->program.varDeclList = (yyvsp[(1) - (4)].ast);
        (yyval.ast)->program.stmtList = (yyvsp[(2) - (4)].ast);
        (yyval.ast)->program.mainFunc = (yyvsp[(3) - (4)].ast);
        (yyval.ast)->program.funcDeclList = (yyvsp[(4) - (4)].ast);
        
        root = (yyval.ast);  // Set root to point to the top-level AST node
    ;}
    break;

  case 3:
#line 109 "parser.y"
    { (yyval.ast) = NULL; ;}
    break;

  case 4:
#line 110 "parser.y"
    {
        printf("DEBUG: Parsed global variable declaration: %s\n", (yyvsp[(1) - (2)].ast)->varDecl.varName);
        ASTNode* varDeclListNode = createNode(NodeType_VarDeclList);
        varDeclListNode->varDeclList.varDecl = (yyvsp[(1) - (2)].ast);
        varDeclListNode->varDeclList.varDeclList = (yyvsp[(2) - (2)].ast);
        (yyval.ast) = varDeclListNode;
    ;}
    break;

  case 5:
#line 121 "parser.y"
    { currentFunctionName = strdup("main"); ;}
    break;

  case 6:
#line 121 "parser.y"
    {
        printf("PARSER: Recognized main function with body.\n");

        // Add `main` as a function with `Is Function: 1`
        addSymbol(symTab, currentFunctionName, "void", 1, NULL, NULL);

        // Create the AST node for main
        (yyval.ast) = createFuncDeclNode("void", currentFunctionName, NULL, (yyvsp[(8) - (10)].ast), NULL);

        // Clean up
        free(currentFunctionName);
        currentFunctionName = NULL;
    ;}
    break;

  case 7:
#line 140 "parser.y"
    { (yyval.ast) = NULL; ;}
    break;

  case 8:
#line 141 "parser.y"
    { 
        printf("DEBUG: Parsed global variable declaration: %s\n", (yyvsp[(1) - (2)].ast)->varDecl.varName);
        ASTNode* varDeclListNode = createNode(NodeType_VarDeclList);
        varDeclListNode->varDeclList.varDecl = (yyvsp[(1) - (2)].ast);  
        varDeclListNode->varDeclList.varDeclList = (yyvsp[(2) - (2)].ast);  
        (yyval.ast) = varDeclListNode; // Return the new list node
    ;}
    break;

  case 9:
#line 151 "parser.y"
    {
        // Create a new AST node for the function declaration
        (yyval.ast) = createNode(NodeType_FuncDecl); // Create a node of type function declaration

        // Assuming 'name' and other necessary fields are part of your FuncDecl structure
        // Populate the AST node with the function name and other relevant data
        if ((yyval.ast)) {
            (*(yyval.ast)).funcDecl.name = strdup((yyvsp[(1) - (1)].string)); // Set the function name
            printf("DEBUG: Set currentFunctionName to %s\n", (*(yyval.ast)).funcDecl.name);
        }

        // Optionally, set the current function name if needed for other contexts
        currentFunctionName = strdup((yyvsp[(1) - (1)].string)); 
    ;}
    break;

  case 10:
#line 169 "parser.y"
    {
        printf("PARSER: Recognized function declaration with body: %s of return type %s\n", currentFunctionName, (yyvsp[(1) - (11)].string));

        // Add the function itself with `Is Function: 1`
        addSymbol(symTab, currentFunctionName, (yyvsp[(1) - (11)].string), 1, NULL, NULL);

        // Create function declaration node in AST
        (yyval.ast) = createFuncDeclNode((yyvsp[(1) - (11)].string), currentFunctionName, (yyvsp[(5) - (11)].ast), (yyvsp[(8) - (11)].ast), (yyvsp[(9) - (11)].ast));

        // Clear function name after use
        free(currentFunctionName);
        currentFunctionName = NULL;
    ;}
    break;

  case 11:
#line 187 "parser.y"
    { (yyval.string) = "void"; ;}
    break;

  case 12:
#line 188 "parser.y"
    { (yyval.string) = (yyvsp[(1) - (1)].string); ;}
    break;

  case 13:
#line 192 "parser.y"
    { (yyval.ast) = NULL; ;}
    break;

  case 14:
#line 193 "parser.y"
    {
        printf("DEBUG: Parsed function declaration: %s\n", (yyvsp[(1) - (2)].ast)->funcDecl.name);
        ASTNode* node = createNode(NodeType_FuncDeclList);
        node->funcDeclList.funcDecl = (yyvsp[(1) - (2)].ast);
        node->funcDeclList.funcDeclList = (yyvsp[(2) - (2)].ast);
        (yyval.ast) = node;
    ;}
    break;

  case 15:
#line 204 "parser.y"
    { 
        enterScope(symTab); 
        printf("Entering scope level %d in function %s\n", symTab->currentScopeLevel, currentFunctionName); // Debugging output
        (yyval.ast) = NULL; 
    ;}
    break;

  case 16:
#line 212 "parser.y"
    { 
        printf("Exiting scope level %d in function %s\n", symTab->currentScopeLevel, currentFunctionName); // Debugging output
        exitScope(symTab); 
        (yyval.ast) = NULL; 
    ;}
    break;

  case 17:
#line 221 "parser.y"
    {
        printf("PARSER: Recognized parameter: %s of type %s in function %s\n", (yyvsp[(2) - (2)].string), (yyvsp[(1) - (2)].string), currentFunctionName);

        // Directly add the parameter to the symbol table with correct order
        addSymbol(symTab, (yyvsp[(2) - (2)].string), (yyvsp[(1) - (2)].string), 1, NULL, currentFunctionName);  // `Is Function` set to 1 to match your setup

        // Create an AST node for the parameter
        (yyval.ast) = createParamNode((yyvsp[(1) - (2)].string), (yyvsp[(2) - (2)].string));
    ;}
    break;

  case 18:
#line 236 "parser.y"
    {
        (yyval.ast) = NULL;  // No parameters
    ;}
    break;

  case 19:
#line 239 "parser.y"
    {
        (yyval.ast) = createParamListNode((yyvsp[(1) - (1)].ast), NULL);  // Single parameter
    ;}
    break;

  case 20:
#line 242 "parser.y"
    {
        (yyval.ast) = createParamListNode((yyvsp[(1) - (3)].ast), (yyvsp[(3) - (3)].ast));  // Append current parameter to the end of the list
    ;}
    break;

  case 21:
#line 252 "parser.y"
    { 
        printf("PARSER: Recognized return statement with value\n");
        (yyval.ast) = createReturnNode((yyvsp[(2) - (3)].ast)); // Create return node with expression
    ;}
    break;

  case 22:
#line 256 "parser.y"
    { 
        printf("PARSER: Invalid return statement in non-void function\n");
        yyerror("Cannot return without a value in a non-void function."); // Handle error
        (yyval.ast) = NULL; // Set to NULL if error occurs
    ;}
    break;

  case 23:
#line 261 "parser.y"
    { 
        (yyval.ast) = NULL; // No return statement for void functions
    ;}
    break;

  case 24:
#line 283 "parser.y"
    {
        printf("PARSER: Recognized variable declaration: %s of type %s in function %s\n", (yyvsp[(2) - (3)].string), (yyvsp[(1) - (3)].string), currentFunctionName ? currentFunctionName : "global");
        addSymbol(symTab, (yyvsp[(2) - (3)].string), (yyvsp[(1) - (3)].string), 0, NULL, currentFunctionName); // Use currentFunctionName for local variables, NULL for global
        (yyval.ast) = createNode(NodeType_VarDecl);  // AST node creation
        (yyval.ast)->varDecl.varType = strdup((yyvsp[(1) - (3)].string));
        (yyval.ast)->varDecl.varName = strdup((yyvsp[(2) - (3)].string));
    ;}
    break;

  case 25:
#line 290 "parser.y"
    {
        printf("PARSER: Recognized array declaration: %s[%d]\n", (yyvsp[(2) - (6)].string), (yyvsp[(4) - (6)].number));
        symbol = lookupSymbol(symTab, (yyvsp[(2) - (6)].string), 1);
        if (symbol != NULL) {
            printf("PARSER: Array %s at line %d has already been declared - COMPILATION HALTED\n", (yyvsp[(2) - (6)].string), yylineno);
            exit(0);
        } else {
            (yyval.ast) = malloc(sizeof(ASTNode));
            (yyval.ast)->type = NodeType_ArrayDecl;
            (yyval.ast)->arrayDecl.varType = strdup((yyvsp[(1) - (6)].string));
            (yyval.ast)->arrayDecl.varName = strdup((yyvsp[(2) - (6)].string));
            (yyval.ast)->arrayDecl.size = (yyvsp[(4) - (6)].number);
            addSymbol(symTab, (yyvsp[(2) - (6)].string), (yyvsp[(1) - (6)].string), 0, NULL, currentFunctionName); // Handle array declarations appropriately
        }
    ;}
    break;

  case 26:
#line 308 "parser.y"
    {
        (yyval.ast) = createNode(NodeType_StmtList); // Create an empty StmtList node
        (yyval.ast)->stmtList.stmt = NULL;
        (yyval.ast)->stmtList.stmtList = NULL;
    ;}
    break;

  case 27:
#line 313 "parser.y"
    {
        (yyval.ast) = createNode(NodeType_StmtList); // Create a new StmtList node
        (yyval.ast)->stmtList.stmt = (yyvsp[(1) - (2)].ast);              // Set the current statement
        (yyval.ast)->stmtList.stmtList = (yyvsp[(2) - (2)].ast);          // Link to the rest of the statements
    ;}
    break;

  case 28:
#line 318 "parser.y"
    { 
        (yyval.ast) = createNode(NodeType_StmtList); // Create a new StmtList node for variable declarations
        (yyval.ast)->stmtList.stmt = (yyvsp[(1) - (2)].ast);              // Add VarDecl to the statement list
        (yyval.ast)->stmtList.stmtList = (yyvsp[(2) - (2)].ast);          // Link to the rest of the statements
    ;}
    break;

  case 29:
#line 327 "parser.y"
    {
          printf("PARSER: Recognized assignment statement\n");
          (yyval.ast) = malloc(sizeof(ASTNode));
          (yyval.ast)->type = NodeType_AssignStmt;
          (yyval.ast)->assignStmt.lvalue = (yyvsp[(1) - (4)].ast);
          (yyval.ast)->assignStmt.expr = (yyvsp[(3) - (4)].ast);
      ;}
    break;

  case 30:
#line 334 "parser.y"
    { 
          printf("PARSER: Recognized write statement\n");
          (yyval.ast) = malloc(sizeof(ASTNode));
          (yyval.ast)->type = NodeType_WriteStmt;
          (yyval.ast)->writeStmt.expr = (yyvsp[(2) - (3)].ast);
      ;}
    break;

  case 31:
#line 340 "parser.y"
    {
          printf("PARSER: Recognized function call statement\n");
          (yyval.ast) = (yyvsp[(1) - (2)].ast);
      ;}
    break;

  case 32:
#line 344 "parser.y"
    {
          printf("PARSER: Recognized a block statement\n");
          (yyval.ast) = malloc(sizeof(ASTNode));
          (yyval.ast)->type = NodeType_BlockStmt;
          (yyval.ast)->blockStmt.stmtList = (yyvsp[(2) - (3)].ast);
      ;}
    break;

  case 33:
#line 350 "parser.y"
    { 
          (yyval.ast) = (yyvsp[(1) - (1)].ast);  // Link `IfStmt` rule to `Stmt` for conditional handling
      ;}
    break;

  case 34:
#line 355 "parser.y"
    { (yyval.string) = (yyvsp[(1) - (1)].string); ;}
    break;

  case 35:
#line 356 "parser.y"
    { (yyval.string) = "bool"; ;}
    break;

  case 36:
#line 360 "parser.y"
    {
          printf("PARSER: Recognized if statement\n");
          (yyval.ast) = createIfNode((yyvsp[(3) - (5)].ast), (yyvsp[(5) - (5)].ast), NULL);  // AST node with condition, true branch, and NULL for else
      ;}
    break;

  case 37:
#line 364 "parser.y"
    {
          printf("PARSER: Recognized if-else statement\n");
          (yyval.ast) = createIfNode((yyvsp[(3) - (7)].ast), (yyvsp[(5) - (7)].ast), (yyvsp[(7) - (7)].ast));  // AST node with condition, true branch, and else branch
      ;}
    break;

  case 38:
#line 370 "parser.y"
    {
    printf("PARSER: Recognized LValue ID: %s\n", (yyvsp[(1) - (1)].string));
    (yyval.ast) = malloc(sizeof(ASTNode));
    (yyval.ast)->type = NodeType_SimpleID;
    (yyval.ast)->simpleID.name = strdup((yyvsp[(1) - (1)].string));
;}
    break;

  case 39:
#line 376 "parser.y"
    {
    printf("PARSER: Recognized LValue array access: %s[...]\n", (yyvsp[(1) - (4)].string));
    (yyval.ast) = malloc(sizeof(ASTNode));
    (yyval.ast)->type = NodeType_ArrayAccess;
    (yyval.ast)->arrayAccess.arrayName = strdup((yyvsp[(1) - (4)].string));
    (yyval.ast)->arrayAccess.index = (yyvsp[(3) - (4)].ast);
;}
    break;

  case 40:
#line 386 "parser.y"
    { 
          (yyval.ast) = (yyvsp[(1) - (1)].ast);  // Treat the function call as an expression
      ;}
    break;

  case 41:
#line 389 "parser.y"
    {
          printf("PARSER: Recognized binary expression\n");
          (yyval.ast) = malloc(sizeof(ASTNode));
          (yyval.ast)->type = NodeType_BinOp;
          (yyval.ast)->binOp.left = (yyvsp[(1) - (3)].ast);
          (yyval.ast)->binOp.right = (yyvsp[(3) - (3)].ast);
          (yyval.ast)->binOp.operator = (yyvsp[(2) - (3)].character);
      ;}
    break;

  case 42:
#line 397 "parser.y"
    {
          printf("PARSER: Recognized logical AND\n");
          (yyval.ast) = createLogicalNode("&&", (yyvsp[(1) - (3)].ast), (yyvsp[(3) - (3)].ast)); // Create logical AND AST node
      ;}
    break;

  case 43:
#line 401 "parser.y"
    {
          printf("PARSER: Recognized logical OR\n");
          (yyval.ast) = createLogicalNode("||", (yyvsp[(1) - (3)].ast), (yyvsp[(3) - (3)].ast)); // Create logical OR AST node
      ;}
    break;

  case 44:
#line 405 "parser.y"
    {
          printf("PARSER: Recognized logical NOT\n");
          (yyval.ast) = createLogicalNode("!", (yyvsp[(2) - (2)].ast), NULL); // Create logical NOT AST node
      ;}
    break;

  case 45:
#line 409 "parser.y"
    {
          printf("PARSER: Recognized equality check\n");
          (yyval.ast) = createComparisonNode("==", (yyvsp[(1) - (3)].ast), (yyvsp[(3) - (3)].ast));
      ;}
    break;

  case 46:
#line 413 "parser.y"
    {
          printf("PARSER: Recognized inequality check\n");
          (yyval.ast) = createComparisonNode("!=", (yyvsp[(1) - (3)].ast), (yyvsp[(3) - (3)].ast));
      ;}
    break;

  case 47:
#line 417 "parser.y"
    {
          printf("PARSER: Recognized less-than comparison\n");
          (yyval.ast) = createComparisonNode("<", (yyvsp[(1) - (3)].ast), (yyvsp[(3) - (3)].ast));
      ;}
    break;

  case 48:
#line 421 "parser.y"
    {
          printf("PARSER: Recognized greater-than comparison\n");
          (yyval.ast) = createComparisonNode(">", (yyvsp[(1) - (3)].ast), (yyvsp[(3) - (3)].ast));
      ;}
    break;

  case 49:
#line 425 "parser.y"
    {
          printf("PARSER: Recognized less-than-or-equal comparison\n");
          (yyval.ast) = createComparisonNode("<=", (yyvsp[(1) - (3)].ast), (yyvsp[(3) - (3)].ast));
      ;}
    break;

  case 50:
#line 429 "parser.y"
    {
          printf("PARSER: Recognized greater-than-or-equal comparison\n");
          (yyval.ast) = createComparisonNode(">=", (yyvsp[(1) - (3)].ast), (yyvsp[(3) - (3)].ast));
      ;}
    break;

  case 51:
#line 433 "parser.y"
    {
          printf("PARSER: Recognized boolean literal: %s\n", (yyvsp[(1) - (1)].number) ? "true" : "false");
          (yyval.ast) = createBoolNode((yyvsp[(1) - (1)].number)); // Add boolean literal node (true or false)
      ;}
    break;

  case 52:
#line 437 "parser.y"
    {
          printf("PARSER: Recognized integer\n");
          (yyval.ast) = createIntNode((yyvsp[(1) - (1)].number));
      ;}
    break;

  case 53:
#line 441 "parser.y"
    {
          printf("PARSER: Recognized float\n");
          (yyval.ast) = createFloatNode((yyvsp[(1) - (1)].float_number));
      ;}
    break;

  case 54:
#line 445 "parser.y"
    {
          printf("PARSER: Recognized id\n");
          (yyval.ast) = createIDNode((yyvsp[(1) - (1)].string));
      ;}
    break;

  case 55:
#line 449 "parser.y"
    {
          printf("PARSER: Recognized array access: %s[...]\n", (yyvsp[(1) - (4)].string));
          (yyval.ast) = malloc(sizeof(ASTNode));
          (yyval.ast)->type = NodeType_ArrayAccess;
          (yyval.ast)->arrayAccess.arrayName = strdup((yyvsp[(1) - (4)].string));
          (yyval.ast)->arrayAccess.index = (yyvsp[(3) - (4)].ast);
      ;}
    break;

  case 56:
#line 456 "parser.y"
    {
          printf("PARSER: Recognized parenthesized expression\n");
          (yyval.ast) = (yyvsp[(2) - (3)].ast);
      ;}
    break;

  case 57:
#line 463 "parser.y"
    {
    printf("PARSER: Recognized function call: %s\n", (yyvsp[(1) - (4)].string));
    (yyval.ast) = createFuncCallNode((yyvsp[(1) - (4)].string), (yyvsp[(3) - (4)].ast));
;}
    break;

  case 58:
#line 469 "parser.y"
    {
    (yyval.ast) = NULL;
;}
    break;

  case 59:
#line 472 "parser.y"
    {
    (yyval.ast) = createArgList((yyvsp[(1) - (1)].ast), NULL);  // Single argument
;}
    break;

  case 60:
#line 475 "parser.y"
    {
    (yyval.ast) = createArgList((yyvsp[(1) - (3)].ast), (yyvsp[(3) - (3)].ast));  // Multiple arguments
;}
    break;

  case 61:
#line 481 "parser.y"
    { (yyval.character) = '+'; ;}
    break;

  case 62:
#line 482 "parser.y"
    { (yyval.character) = '-'; ;}
    break;

  case 63:
#line 483 "parser.y"
    { (yyval.character) = '*'; ;}
    break;

  case 64:
#line 484 "parser.y"
    { (yyval.character) = '/'; ;}
    break;


/* Line 1267 of yacc.c.  */
#line 2111 "parser.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 487 "parser.y"


// Main function

int main() {
    yyin = fopen("testProg.cmm", "r");

    symTab = createSymbolTable(TABLE_SIZE);
    if (symTab == NULL) {
        return EXIT_FAILURE;
    }
    symbol = malloc(sizeof(Symbol));
    initializeTempVars();

    if (yyparse() == 0) {
        printf("Parsing successful!\n");

        if (root == NULL) {
        fprintf(stderr, "Error: AST root node is NULL after parsing.\n");
        return EXIT_FAILURE;
        }
        traverseAST(root, 0);
        printSymbolTable(symTab);
        printf("\n ===SEMANTIC ANALYSIS===\n");
        semanticAnalysis(root, symTab);
        printf("\n=== TAC GENERATION ===\n");
        printTACToFile("TAC.ir", tacHead);
        printf("\n=== CODE OPTIMIZATION ===\n");
        //optimizeTAC(&tacHead);
        printOptimizedTAC("TACOptimized.ir", tacHead);
        printf("\n=== CODE GENERATION ===\n");
        initCodeGenerator("output.s");
        generateMIPS(tacHead);
        finalizeCodeGenerator("output.s");


        freeAST(root);
        freeSymbolTable(symTab);
    } else {
        fprintf(stderr, "Parsing failed\n");
    }

    fclose(yyin);
    return 0;
}

// Error handling function
void yyerror(const char* s) {
    if (strstr(s, "syntax error")) {
        fprintf(stderr, "Syntax error: %s at line %d. Did you forget a semicolon or a closing brace?\n", s, yylineno);
    } else {
        fprintf(stderr, "Error: %s at line %d\n", s, yylineno);
    }
}

