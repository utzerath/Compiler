/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
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

#line 98 "parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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
    WHILE = 279,
    BOOL_LITERAL = 280,
    TYPE_BOOL = 281,
    AND_OP = 282,
    OR_OP = 283,
    NOT_OP = 284,
    IF = 285,
    ELSE = 286,
    AND = 287,
    OR = 288,
    NOT = 289,
    EQUALS = 290,
    NOT_EQUALS = 291,
    LESS_THAN = 292,
    GREATER_THAN = 293,
    LESS_EQUAL = 294,
    GREATER_EQUAL = 295
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

#line 200 "parser.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
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
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   296

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  65
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  127

#define YYUNDEFTOK  2
#define YYMAXUTOK   295


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
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
      35,    36,    37,    38,    39,    40
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    96,    96,   111,   112,   123,   123,   142,   143,   153,
     171,   189,   190,   194,   195,   206,   214,   223,   238,   241,
     244,   254,   258,   263,   285,   292,   310,   313,   318,   328,
     335,   341,   345,   351,   354,   361,   362,   366,   370,   376,
     382,   392,   395,   403,   407,   411,   415,   419,   423,   427,
     431,   435,   439,   443,   447,   451,   455,   462,   469,   475,
     478,   481,   487,   488,   489,   490
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TYPE", "NUMBER", "FLOAT_LITERAL",
  "SEMICOLON", "EQ", "PLUS", "MINUS", "MULTIPLY", "DIVIDE", "WRITE",
  "OPEN_BRACE", "CLOSE_BRACE", "OPEN_PAREN", "CLOSE_PAREN", "ID",
  "OPEN_BRACKET", "CLOSE_BRACKET", "RETURN", "VOID", "COMMA", "MAIN",
  "WHILE", "BOOL_LITERAL", "TYPE_BOOL", "\"&&\"", "\"||\"", "\"!\"", "IF",
  "ELSE", "AND", "OR", "NOT", "EQUALS", "NOT_EQUALS", "LESS_THAN",
  "GREATER_THAN", "LESS_EQUAL", "GREATER_EQUAL", "$accept", "Program",
  "GlobalDeclList", "MainFunc", "$@1", "SetFunctionName", "FuncDecl",
  "ReturnType", "FuncDeclList", "scope_enter", "scope_exit", "Param",
  "ParamList", "ReturnStmt", "VarDecl", "StmtList", "Stmt", "Type",
  "IfStmt", "LValue", "Expr", "FuncCall", "ArgList", "BinOp", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295
};
# endif

#define YYPACT_NINF (-75)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       1,   -75,   -75,    17,   266,     1,    19,   -75,   233,   266,
     -10,     6,    22,   266,    18,   266,   -75,    33,    35,   -75,
      20,   -75,   -75,   233,     4,   -75,   233,    23,   -75,    60,
     233,   233,   233,   233,   -75,    34,    -1,   -75,   233,   -75,
     -75,    73,    92,   233,   -75,   -75,   -75,   -75,   -75,   -75,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   -75,
      43,    70,   141,   126,   159,    72,   -75,   -75,    -1,    71,
     -75,    58,    86,   -75,   174,   225,   207,    62,    62,    62,
      62,    62,    62,   192,   233,   -75,   -75,   109,   109,    76,
     -75,   -75,    74,   -75,   100,   -75,   -75,   -75,    78,   -75,
     -75,   -75,   109,   -75,   112,   -75,    98,   103,   101,   122,
     266,   -75,   112,   140,   142,   -75,   266,   -75,   134,   -75,
     251,   143,   -75,   108,   -75,   -75,   -75
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,    35,    36,     0,    26,     3,     0,     1,     0,    26,
      39,     0,     0,    26,     0,    26,    33,     0,     0,     4,
       0,    53,    54,     0,    55,    52,     0,     0,    41,     0,
      59,     0,     0,     0,    28,     0,    13,    27,     0,    31,
      24,     0,     0,     0,    45,    30,    62,    63,    64,    65,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    32,
      60,     0,     0,     0,     0,     0,    12,    11,    13,     0,
       2,     0,     0,    57,     0,    43,    44,    46,    47,    48,
      49,    50,    51,    42,    59,    58,    40,     0,     0,     0,
      14,     9,     0,    29,     0,    56,    61,    34,    37,     5,
      15,    25,     0,    15,    18,    38,     0,     0,    19,     0,
      26,    17,    18,     0,     0,    20,    26,    16,    23,     6,
       0,     0,    22,     0,    16,    21,    10
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -75,   -75,   150,   -75,   -75,   -75,   -75,   -75,   104,    68,
      63,   -75,    77,   -75,    11,    -6,   -74,   -75,   -75,   -75,
      -8,    -3,   102,   -75
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     3,     4,    36,   103,    92,    68,    69,    70,   104,
     119,   108,   109,   121,    13,    14,    15,     6,    16,    17,
      60,    28,    61,    58
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      27,    18,    66,    29,     1,    30,    18,    34,    31,    37,
      18,     5,    18,    97,    98,    42,     5,     7,    44,    30,
      67,    32,    43,    62,    63,    64,    40,     2,   105,    45,
      71,    46,    47,    48,    49,    74,    20,    33,    41,    35,
      38,    39,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    46,    47,    48,    49,    50,    51,    65,    52,    53,
      54,    55,    56,    57,    93,    84,    46,    47,    48,    49,
      46,    47,    48,    49,    59,    50,    51,    72,    52,    53,
      54,    55,    56,    57,    18,    18,    85,    89,    91,   100,
      50,    51,    99,    52,    53,    54,    55,    56,    57,    18,
      46,    47,    48,    49,   114,    94,   101,    18,    73,   102,
     118,   110,   123,    18,   125,   107,    46,    47,    48,    49,
     111,     8,     9,   112,    50,    51,    10,    52,    53,    54,
      55,    56,    57,    11,    46,    47,    48,    49,   113,    12,
      50,    51,    87,    52,    53,    54,    55,    56,    57,    46,
      47,    48,    49,   116,   120,    19,   117,   124,    50,    51,
      86,    52,    53,    54,    55,    56,    57,    46,    47,    48,
      49,   106,    90,    50,    51,    88,    52,    53,    54,    55,
      56,    57,    46,    47,    48,    49,    96,   126,     0,   115,
       0,    50,    51,    95,    52,    53,    54,    55,    56,    57,
      46,    47,    48,    49,     0,     0,    50,    51,     0,    52,
      53,    54,    55,    56,    57,    46,    47,    48,    49,     0,
       0,     0,     0,     0,    50,    51,     0,    52,    53,    54,
      55,    56,    57,    46,    47,    48,    49,    21,    22,    50,
       0,     0,    52,    53,    54,    55,    56,    57,    23,     0,
      24,     0,     0,     0,     0,    21,    22,   122,    25,     0,
      52,    53,    54,    55,    56,    57,    23,    26,    24,     1,
       0,     0,     0,     0,     0,     0,    25,     0,     8,     9,
       0,     0,     0,    10,     0,    26,     0,     0,     0,     0,
      11,     0,     2,     0,     0,     0,    12
};

static const yytype_int8 yycheck[] =
{
       8,     4,     3,     9,     3,    15,     9,    13,    18,    15,
      13,     0,    15,    87,    88,    23,     5,     0,    26,    15,
      21,    15,    18,    31,    32,    33,     6,    26,   102,     6,
      38,     8,     9,    10,    11,    43,    17,    15,    18,    21,
       7,     6,    50,    51,    52,    53,    54,    55,    56,    57,
      58,     8,     9,    10,    11,    32,    33,    23,    35,    36,
      37,    38,    39,    40,     6,    22,     8,     9,    10,    11,
       8,     9,    10,    11,    14,    32,    33,     4,    35,    36,
      37,    38,    39,    40,    87,    88,    16,    15,    17,    15,
      32,    33,    16,    35,    36,    37,    38,    39,    40,   102,
       8,     9,    10,    11,   110,    19,     6,   110,    16,    31,
     116,    13,   120,   116,     6,     3,     8,     9,    10,    11,
      17,    12,    13,    22,    32,    33,    17,    35,    36,    37,
      38,    39,    40,    24,     8,     9,    10,    11,    16,    30,
      32,    33,    16,    35,    36,    37,    38,    39,    40,     8,
       9,    10,    11,    13,    20,     5,    14,    14,    32,    33,
      19,    35,    36,    37,    38,    39,    40,     8,     9,    10,
      11,   103,    68,    32,    33,    16,    35,    36,    37,    38,
      39,    40,     8,     9,    10,    11,    84,   124,    -1,   112,
      -1,    32,    33,    19,    35,    36,    37,    38,    39,    40,
       8,     9,    10,    11,    -1,    -1,    32,    33,    -1,    35,
      36,    37,    38,    39,    40,     8,     9,    10,    11,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    35,    36,    37,
      38,    39,    40,     8,     9,    10,    11,     4,     5,    32,
      -1,    -1,    35,    36,    37,    38,    39,    40,    15,    -1,
      17,    -1,    -1,    -1,    -1,     4,     5,     6,    25,    -1,
      35,    36,    37,    38,    39,    40,    15,    34,    17,     3,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    12,    13,
      -1,    -1,    -1,    17,    -1,    34,    -1,    -1,    -1,    -1,
      24,    -1,    26,    -1,    -1,    -1,    30
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    26,    42,    43,    55,    58,     0,    12,    13,
      17,    24,    30,    55,    56,    57,    59,    60,    62,    43,
      17,     4,     5,    15,    17,    25,    34,    61,    62,    56,
      15,    18,    15,    15,    56,    21,    44,    56,     7,     6,
       6,    18,    61,    18,    61,     6,     8,     9,    10,    11,
      32,    33,    35,    36,    37,    38,    39,    40,    64,    14,
      61,    63,    61,    61,    61,    23,     3,    21,    47,    48,
      49,    61,     4,    16,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    22,    16,    19,    16,    16,    15,
      49,    17,    46,     6,    19,    19,    63,    57,    57,    16,
      15,     6,    31,    45,    50,    57,    50,     3,    52,    53,
      13,    17,    22,    16,    56,    53,    13,    14,    56,    51,
      20,    54,     6,    61,    14,     6,    51
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    41,    42,    43,    43,    45,    44,    43,    43,    46,
      47,    48,    48,    49,    49,    50,    51,    52,    53,    53,
      53,    54,    54,    54,    55,    55,    56,    56,    56,    57,
      57,    57,    57,    57,    57,    58,    58,    59,    59,    60,
      60,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    62,    63,
      63,    63,    64,    64,    64,    64
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     4,     0,     2,     0,    10,     0,     2,     1,
      11,     1,     1,     0,     2,     0,     0,     2,     0,     1,
       3,     3,     2,     0,     3,     6,     0,     2,     2,     4,
       3,     2,     3,     1,     5,     1,     1,     5,     7,     1,
       4,     1,     3,     3,     3,     2,     3,     3,     3,     3,
       3,     3,     1,     1,     1,     1,     4,     3,     4,     0,
       1,     3,     1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

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
#ifndef YYINITDEPTH
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
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
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
            else
              goto append;

          append:
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

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

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
#line 96 "parser.y"
                                                  {
        printf("DEBUG: Parsed complete program with global declarations, statements, main function, and other functions.\n");
        (yyval.ast) = createNode(NodeType_Program);
        (yyval.ast)->program.varDeclList = (yyvsp[-3].ast);
        (yyval.ast)->program.stmtList = (yyvsp[-2].ast);
        (yyval.ast)->program.mainFunc = (yyvsp[-1].ast);
        (yyval.ast)->program.funcDeclList = (yyvsp[0].ast);
        
        root = (yyval.ast);  // Set root to point to the top-level AST node
    }
#line 1504 "parser.tab.c"
    break;

  case 3:
#line 111 "parser.y"
                { (yyval.ast) = NULL; }
#line 1510 "parser.tab.c"
    break;

  case 4:
#line 112 "parser.y"
                             {
        printf("DEBUG: Parsed global variable declaration: %s\n", (yyvsp[-1].ast)->varDecl.varName);
        ASTNode* varDeclListNode = createNode(NodeType_VarDeclList);
        varDeclListNode->varDeclList.varDecl = (yyvsp[-1].ast);
        varDeclListNode->varDeclList.varDeclList = (yyvsp[0].ast);
        (yyval.ast) = varDeclListNode;
    }
#line 1522 "parser.tab.c"
    break;

  case 5:
#line 123 "parser.y"
                                     { currentFunctionName = strdup("main"); }
#line 1528 "parser.tab.c"
    break;

  case 6:
#line 123 "parser.y"
                                                                                                                                      {
        printf("PARSER: Recognized main function with body.\n");

        // Add `main` as a function with `Is Function: 1`
        addSymbol(symTab, currentFunctionName, "void", 1, NULL, NULL);

        // Create the AST node for main
        (yyval.ast) = createFuncDeclNode("void", currentFunctionName, NULL, (yyvsp[-2].ast), NULL);

        // Clean up
        free(currentFunctionName);
        currentFunctionName = NULL;
    }
#line 1546 "parser.tab.c"
    break;

  case 7:
#line 142 "parser.y"
                { (yyval.ast) = NULL; }
#line 1552 "parser.tab.c"
    break;

  case 8:
#line 143 "parser.y"
                             { 
        printf("DEBUG: Parsed global variable declaration: %s\n", (yyvsp[-1].ast)->varDecl.varName);
        ASTNode* varDeclListNode = createNode(NodeType_VarDeclList);
        varDeclListNode->varDeclList.varDecl = (yyvsp[-1].ast);  
        varDeclListNode->varDeclList.varDeclList = (yyvsp[0].ast);  
        (yyval.ast) = varDeclListNode; // Return the new list node
    }
#line 1564 "parser.tab.c"
    break;

  case 9:
#line 153 "parser.y"
       {
        // Create a new AST node for the function declaration
        (yyval.ast) = createNode(NodeType_FuncDecl); // Create a node of type function declaration

        // Assuming 'name' and other necessary fields are part of your FuncDecl structure
        // Populate the AST node with the function name and other relevant data
        if ((yyval.ast)) {
            (*(yyval.ast)).funcDecl.name = strdup((yyvsp[0].string)); // Set the function name
            printf("DEBUG: Set currentFunctionName to %s\n", (*(yyval.ast)).funcDecl.name);
        }

        // Optionally, set the current function name if needed for other contexts
        currentFunctionName = strdup((yyvsp[0].string)); 
    }
#line 1583 "parser.tab.c"
    break;

  case 10:
#line 171 "parser.y"
                                                                                                                                  {
        printf("PARSER: Recognized function declaration with body: %s of return type %s\n", currentFunctionName, (yyvsp[-10].string));

        // Add the function itself with `Is Function: 1`
        addSymbol(symTab, currentFunctionName, (yyvsp[-10].string), 1, NULL, NULL);

        // Create function declaration node in AST
        (yyval.ast) = createFuncDeclNode((yyvsp[-10].string), currentFunctionName, (yyvsp[-6].ast), (yyvsp[-3].ast), (yyvsp[-2].ast));

        // Clear function name after use
        free(currentFunctionName);
        currentFunctionName = NULL;
    }
#line 1601 "parser.tab.c"
    break;

  case 11:
#line 189 "parser.y"
         { (yyval.string) = "void"; }
#line 1607 "parser.tab.c"
    break;

  case 12:
#line 190 "parser.y"
           { (yyval.string) = (yyvsp[0].string); }
#line 1613 "parser.tab.c"
    break;

  case 13:
#line 194 "parser.y"
                { (yyval.ast) = NULL; }
#line 1619 "parser.tab.c"
    break;

  case 14:
#line 195 "parser.y"
                            {
        printf("DEBUG: Parsed function declaration: %s\n", (yyvsp[-1].ast)->funcDecl.name);
        ASTNode* node = createNode(NodeType_FuncDeclList);
        node->funcDeclList.funcDecl = (yyvsp[-1].ast);
        node->funcDeclList.funcDeclList = (yyvsp[0].ast);
        (yyval.ast) = node;
    }
#line 1631 "parser.tab.c"
    break;

  case 15:
#line 206 "parser.y"
    { 
        enterScope(symTab); 
        printf("Entering scope level %d in function %s\n", symTab->currentScopeLevel, currentFunctionName); // Debugging output
        (yyval.ast) = NULL; 
    }
#line 1641 "parser.tab.c"
    break;

  case 16:
#line 214 "parser.y"
    { 
        printf("Exiting scope level %d in function %s\n", symTab->currentScopeLevel, currentFunctionName); // Debugging output
        exitScope(symTab); 
        (yyval.ast) = NULL; 
    }
#line 1651 "parser.tab.c"
    break;

  case 17:
#line 223 "parser.y"
            {
        printf("PARSER: Recognized parameter: %s of type %s in function %s\n", (yyvsp[0].string), (yyvsp[-1].string), currentFunctionName);

        // Directly add the parameter to the symbol table with correct order
        addSymbol(symTab, (yyvsp[0].string), (yyvsp[-1].string), 1, NULL, currentFunctionName);  // `Is Function` set to 1 to match your setup

        // Create an AST node for the parameter
        (yyval.ast) = createParamNode((yyvsp[-1].string), (yyvsp[0].string));
    }
#line 1665 "parser.tab.c"
    break;

  case 18:
#line 238 "parser.y"
                {
        (yyval.ast) = NULL;  // No parameters
    }
#line 1673 "parser.tab.c"
    break;

  case 19:
#line 241 "parser.y"
            {
        (yyval.ast) = createParamListNode((yyvsp[0].ast), NULL);  // Single parameter
    }
#line 1681 "parser.tab.c"
    break;

  case 20:
#line 244 "parser.y"
                            {
        (yyval.ast) = createParamListNode((yyvsp[-2].ast), (yyvsp[0].ast));  // Append current parameter to the end of the list
    }
#line 1689 "parser.tab.c"
    break;

  case 21:
#line 254 "parser.y"
                          { 
        printf("PARSER: Recognized return statement with value\n");
        (yyval.ast) = createReturnNode((yyvsp[-1].ast)); // Create return node with expression
    }
#line 1698 "parser.tab.c"
    break;

  case 22:
#line 258 "parser.y"
                       { 
        printf("PARSER: Invalid return statement in non-void function\n");
        yyerror("Cannot return without a value in a non-void function."); // Handle error
        (yyval.ast) = NULL; // Set to NULL if error occurs
    }
#line 1708 "parser.tab.c"
    break;

  case 23:
#line 263 "parser.y"
                  { 
        (yyval.ast) = NULL; // No return statement for void functions
    }
#line 1716 "parser.tab.c"
    break;

  case 24:
#line 285 "parser.y"
                      {
        printf("PARSER: Recognized variable declaration: %s of type %s in function %s\n", (yyvsp[-1].string), (yyvsp[-2].string), currentFunctionName ? currentFunctionName : "global");
        addSymbol(symTab, (yyvsp[-1].string), (yyvsp[-2].string), 0, NULL, currentFunctionName); // Use currentFunctionName for local variables, NULL for global
        (yyval.ast) = createNode(NodeType_VarDecl);  // AST node creation
        (yyval.ast)->varDecl.varType = strdup((yyvsp[-2].string));
        (yyval.ast)->varDecl.varName = strdup((yyvsp[-1].string));
    }
#line 1728 "parser.tab.c"
    break;

  case 25:
#line 292 "parser.y"
                                                          {
        printf("PARSER: Recognized array declaration: %s[%d]\n", (yyvsp[-4].string), (yyvsp[-2].number));
        symbol = lookupSymbol(symTab, (yyvsp[-4].string), 1);
        if (symbol != NULL) {
            printf("PARSER: Array %s at line %d has already been declared - COMPILATION HALTED\n", (yyvsp[-4].string), yylineno);
            exit(0);
        } else {
            (yyval.ast) = malloc(sizeof(ASTNode));
            (yyval.ast)->type = NodeType_ArrayDecl;
            (yyval.ast)->arrayDecl.varType = strdup((yyvsp[-5].string));
            (yyval.ast)->arrayDecl.varName = strdup((yyvsp[-4].string));
            (yyval.ast)->arrayDecl.size = (yyvsp[-2].number);
            addSymbol(symTab, (yyvsp[-4].string), (yyvsp[-5].string), 0, NULL, currentFunctionName); // Handle array declarations appropriately
        }
    }
#line 1748 "parser.tab.c"
    break;

  case 26:
#line 310 "parser.y"
                { 
        (yyval.ast) = NULL;  // If there are no statements, make the statement list NULL instead of creating an empty node
    }
#line 1756 "parser.tab.c"
    break;

  case 27:
#line 313 "parser.y"
                    {
        (yyval.ast) = createNode(NodeType_StmtList); // Create a new StmtList node
        (yyval.ast)->stmtList.stmt = (yyvsp[-1].ast);              // Set the current statement
        (yyval.ast)->stmtList.stmtList = (yyvsp[0].ast);          // Link to the rest of the statements
    }
#line 1766 "parser.tab.c"
    break;

  case 28:
#line 318 "parser.y"
                       { 
        (yyval.ast) = createNode(NodeType_StmtList); // Create a new StmtList node for variable declarations
        (yyval.ast)->stmtList.stmt = (yyvsp[-1].ast);              // Add VarDecl to the statement list
        (yyval.ast)->stmtList.stmtList = (yyvsp[0].ast);          // Link to the rest of the statements
    }
#line 1776 "parser.tab.c"
    break;

  case 29:
#line 328 "parser.y"
                             {
        printf("PARSER: Recognized assignment statement\n");
        (yyval.ast) = malloc(sizeof(ASTNode));
        (yyval.ast)->type = NodeType_AssignStmt;
        (yyval.ast)->assignStmt.lvalue = (yyvsp[-3].ast);
        (yyval.ast)->assignStmt.expr = (yyvsp[-1].ast);
    }
#line 1788 "parser.tab.c"
    break;

  case 30:
#line 335 "parser.y"
                           { 
        printf("PARSER: Recognized write statement\n");
        (yyval.ast) = malloc(sizeof(ASTNode));
        (yyval.ast)->type = NodeType_WriteStmt;
        (yyval.ast)->writeStmt.expr = (yyvsp[-1].ast);
    }
#line 1799 "parser.tab.c"
    break;

  case 31:
#line 341 "parser.y"
                         {
        printf("PARSER: Recognized function call statement\n");
        (yyval.ast) = (yyvsp[-1].ast);
    }
#line 1808 "parser.tab.c"
    break;

  case 32:
#line 345 "parser.y"
                                      {
        printf("PARSER: Recognized a block statement\n");
        (yyval.ast) = malloc(sizeof(ASTNode));
        (yyval.ast)->type = NodeType_BlockStmt;
        (yyval.ast)->blockStmt.stmtList = (yyvsp[-1].ast);
    }
#line 1819 "parser.tab.c"
    break;

  case 33:
#line 351 "parser.y"
             { 
        (yyval.ast) = (yyvsp[0].ast);  // Link `IfStmt` rule to `Stmt` for conditional handling
    }
#line 1827 "parser.tab.c"
    break;

  case 34:
#line 354 "parser.y"
                                             {  // Adding the while loop handling
        printf("PARSER: Recognized while loop\n");
        (yyval.ast) = createWhileNode((yyvsp[-2].ast), (yyvsp[0].ast)); // $3 is the condition, $5 is the body of the loop
    }
#line 1836 "parser.tab.c"
    break;

  case 35:
#line 361 "parser.y"
                 { (yyval.string) = (yyvsp[0].string); }
#line 1842 "parser.tab.c"
    break;

  case 36:
#line 362 "parser.y"
                 { (yyval.string) = "bool"; }
#line 1848 "parser.tab.c"
    break;

  case 37:
#line 366 "parser.y"
                                          {
          printf("PARSER: Recognized if statement\n");
          (yyval.ast) = createIfNode((yyvsp[-2].ast), (yyvsp[0].ast), NULL);  // AST node with condition, true branch, and NULL for else
      }
#line 1857 "parser.tab.c"
    break;

  case 38:
#line 370 "parser.y"
                                                    {
          printf("PARSER: Recognized if-else statement\n");
          (yyval.ast) = createIfNode((yyvsp[-4].ast), (yyvsp[-2].ast), (yyvsp[0].ast));  // AST node with condition, true branch, and else branch
      }
#line 1866 "parser.tab.c"
    break;

  case 39:
#line 376 "parser.y"
           {
    printf("PARSER: Recognized LValue ID: %s\n", (yyvsp[0].string));
    (yyval.ast) = malloc(sizeof(ASTNode));
    (yyval.ast)->type = NodeType_SimpleID;
    (yyval.ast)->simpleID.name = strdup((yyvsp[0].string));
}
#line 1877 "parser.tab.c"
    break;

  case 40:
#line 382 "parser.y"
                                     {
    printf("PARSER: Recognized LValue array access: %s[...]\n", (yyvsp[-3].string));
    (yyval.ast) = malloc(sizeof(ASTNode));
    (yyval.ast)->type = NodeType_ArrayAccess;
    (yyval.ast)->arrayAccess.arrayName = strdup((yyvsp[-3].string));
    (yyval.ast)->arrayAccess.index = (yyvsp[-1].ast);
}
#line 1889 "parser.tab.c"
    break;

  case 41:
#line 392 "parser.y"
               { 
          (yyval.ast) = (yyvsp[0].ast);  // Treat the function call as an expression
      }
#line 1897 "parser.tab.c"
    break;

  case 42:
#line 395 "parser.y"
                      {
          printf("PARSER: Recognized binary expression\n");
          (yyval.ast) = malloc(sizeof(ASTNode));
          (yyval.ast)->type = NodeType_BinOp;
          (yyval.ast)->binOp.left = (yyvsp[-2].ast);
          (yyval.ast)->binOp.right = (yyvsp[0].ast);
          (yyval.ast)->binOp.operator = (yyvsp[-1].character);
      }
#line 1910 "parser.tab.c"
    break;

  case 43:
#line 403 "parser.y"
                    {
          printf("PARSER: Recognized logical AND\n");
          (yyval.ast) = createLogicalNode("&&", (yyvsp[-2].ast), (yyvsp[0].ast)); // Create logical AND AST node
      }
#line 1919 "parser.tab.c"
    break;

  case 44:
#line 407 "parser.y"
                   {
          printf("PARSER: Recognized logical OR\n");
          (yyval.ast) = createLogicalNode("||", (yyvsp[-2].ast), (yyvsp[0].ast)); // Create logical OR AST node
      }
#line 1928 "parser.tab.c"
    break;

  case 45:
#line 411 "parser.y"
               {
          printf("PARSER: Recognized logical NOT\n");
          (yyval.ast) = createLogicalNode("!", (yyvsp[0].ast), NULL); // Create logical NOT AST node
      }
#line 1937 "parser.tab.c"
    break;

  case 46:
#line 415 "parser.y"
                       {
          printf("PARSER: Recognized equality check\n");
          (yyval.ast) = createComparisonNode("==", (yyvsp[-2].ast), (yyvsp[0].ast));
      }
#line 1946 "parser.tab.c"
    break;

  case 47:
#line 419 "parser.y"
                           {
          printf("PARSER: Recognized inequality check\n");
          (yyval.ast) = createComparisonNode("!=", (yyvsp[-2].ast), (yyvsp[0].ast));
      }
#line 1955 "parser.tab.c"
    break;

  case 48:
#line 423 "parser.y"
                          {
          printf("PARSER: Recognized less-than comparison\n");
          (yyval.ast) = createComparisonNode("<", (yyvsp[-2].ast), (yyvsp[0].ast));
      }
#line 1964 "parser.tab.c"
    break;

  case 49:
#line 427 "parser.y"
                             {
          printf("PARSER: Recognized greater-than comparison\n");
          (yyval.ast) = createComparisonNode(">", (yyvsp[-2].ast), (yyvsp[0].ast));
      }
#line 1973 "parser.tab.c"
    break;

  case 50:
#line 431 "parser.y"
                           {
          printf("PARSER: Recognized less-than-or-equal comparison\n");
          (yyval.ast) = createComparisonNode("<=", (yyvsp[-2].ast), (yyvsp[0].ast));
      }
#line 1982 "parser.tab.c"
    break;

  case 51:
#line 435 "parser.y"
                              {
          printf("PARSER: Recognized greater-than-or-equal comparison\n");
          (yyval.ast) = createComparisonNode(">=", (yyvsp[-2].ast), (yyvsp[0].ast));
      }
#line 1991 "parser.tab.c"
    break;

  case 52:
#line 439 "parser.y"
                   {
          printf("PARSER: Recognized boolean literal: %s\n", (yyvsp[0].number) ? "true" : "false");
          (yyval.ast) = createBoolNode((yyvsp[0].number)); // Add boolean literal node (true or false)
      }
#line 2000 "parser.tab.c"
    break;

  case 53:
#line 443 "parser.y"
             {
          printf("PARSER: Recognized integer\n");
          (yyval.ast) = createIntNode((yyvsp[0].number));
      }
#line 2009 "parser.tab.c"
    break;

  case 54:
#line 447 "parser.y"
                    {
          printf("PARSER: Recognized float\n");
          (yyval.ast) = createFloatNode((yyvsp[0].float_number));
      }
#line 2018 "parser.tab.c"
    break;

  case 55:
#line 451 "parser.y"
         {
          printf("PARSER: Recognized id\n");
          (yyval.ast) = createIDNode((yyvsp[0].string));
      }
#line 2027 "parser.tab.c"
    break;

  case 56:
#line 455 "parser.y"
                                         {
          printf("PARSER: Recognized array access: %s[...]\n", (yyvsp[-3].string));
          (yyval.ast) = malloc(sizeof(ASTNode));
          (yyval.ast)->type = NodeType_ArrayAccess;
          (yyval.ast)->arrayAccess.arrayName = strdup((yyvsp[-3].string));
          (yyval.ast)->arrayAccess.index = (yyvsp[-1].ast);
      }
#line 2039 "parser.tab.c"
    break;

  case 57:
#line 462 "parser.y"
                                  {
          printf("PARSER: Recognized parenthesized expression\n");
          (yyval.ast) = (yyvsp[-1].ast);
      }
#line 2048 "parser.tab.c"
    break;

  case 58:
#line 469 "parser.y"
                                            {
    printf("PARSER: Recognized function call: %s\n", (yyvsp[-3].string));
    (yyval.ast) = createFuncCallNode((yyvsp[-3].string), (yyvsp[-1].ast));
}
#line 2057 "parser.tab.c"
    break;

  case 59:
#line 475 "parser.y"
                     {
    (yyval.ast) = NULL;
}
#line 2065 "parser.tab.c"
    break;

  case 60:
#line 478 "parser.y"
       {
    (yyval.ast) = createArgList((yyvsp[0].ast), NULL);  // Single argument
}
#line 2073 "parser.tab.c"
    break;

  case 61:
#line 481 "parser.y"
                     {
    (yyval.ast) = createArgList((yyvsp[-2].ast), (yyvsp[0].ast));  // Multiple arguments
}
#line 2081 "parser.tab.c"
    break;

  case 62:
#line 487 "parser.y"
              { (yyval.character) = '+'; }
#line 2087 "parser.tab.c"
    break;

  case 63:
#line 488 "parser.y"
              { (yyval.character) = '-'; }
#line 2093 "parser.tab.c"
    break;

  case 64:
#line 489 "parser.y"
                { (yyval.character) = '*'; }
#line 2099 "parser.tab.c"
    break;

  case 65:
#line 490 "parser.y"
               { (yyval.character) = '/'; }
#line 2105 "parser.tab.c"
    break;


#line 2109 "parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
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

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
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
  return yyresult;
}
#line 493 "parser.y"


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
        optimizeTAC(&tacHead);
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
