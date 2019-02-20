/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 15 "parser.yy" /* yacc.c:339  */

//#define YYDEBUG 1
#include <basic/Dynamic_Array.h>
#include <basic/Iterator.h>
//#include <omega_calc/AST.h>
#include <omega/parser/AST.hh>
#include <omega/hull.h>
#include <omega/closure.h>
#include <omega/reach.h>
#include <string>
#include <iostream>
#include <fstream>
#include "parser.tab.hh"
#include <FlexLexer.h>
//#include <omega_calc/myflex.h>
//#include <stdio.h>

#if defined __USE_POSIX
#include <unistd.h>
#elif defined  __WIN32
#include <io.h>
#endif


#ifndef WIN32
#include <sys/time.h>
#include <sys/resource.h>
#endif
#if !defined(OMIT_GETRUSAGE)
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#endif

#if !defined(OMIT_GETRUSAGE)
#ifdef __sparc__
extern "C" int getrusage (int, struct rusage*);
#endif

#define BUILD_CODEGEN 1
#define OC_BUILD_DATE "10/07/2018"
#define SKIP_MAIN 1

struct rusage start_time;
bool anyTimingDone = false;

void start_clock( void ) {
  getrusage(RUSAGE_SELF, &start_time);
}

int clock_diff( void ) {
  struct rusage current_time;
  getrusage(RUSAGE_SELF, &current_time);
  return (current_time.ru_utime.tv_sec -start_time.ru_utime.tv_sec)*1000000 + (current_time.ru_utime.tv_usec-start_time.ru_utime.tv_usec);
}
#endif


#ifdef BUILD_CODEGEN  
#include <codegen.h>
#endif

extern yyFlexLexer mylexer;
#define yylex mylexer.yylex

  


int omega_calc_debug = 0;

extern bool is_interactive;
extern const char *PROMPT_STRING;
bool simplify = true;
using namespace omega;

extern std::string err_msg;

bool need_coef;

namespace {
  int redundant_conj_level = 2;  // default maximum 2
  int redundant_constr_level = 4;  // default maximum 4
}

std::map<std::string, Relation *> relationMap;
int argCount = 0;
int tuplePos = 0;
Argument_Tuple currentTuple = Input_Tuple;

Relation LexForward(int n);
reachable_information *reachable_info;

void yyerror(const std::string &s);
void flushScanBuffer();


#line 163 "parser.tab.cc" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.tab.hh".  */
#ifndef YY_YY_PARSER_TAB_HH_INCLUDED
# define YY_YY_PARSER_TAB_HH_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    VAR = 258,
    INT = 259,
    COEF = 260,
    STRING = 261,
    OPEN_BRACE = 262,
    CLOSE_BRACE = 263,
    SYMBOLIC = 264,
    NO_SIMPLIFY = 265,
    OR = 266,
    AND = 267,
    NOT = 268,
    ST = 269,
    APPROX = 270,
    IS_ASSIGNED = 271,
    FORALL = 272,
    EXISTS = 273,
    DOMAIN = 274,
    RANGE = 275,
    DIFFERENCE = 276,
    DIFFERENCE_TO_RELATION = 277,
    GIST = 278,
    GIVEN = 279,
    HULL = 280,
    WITHIN = 281,
    MAXIMIZE = 282,
    MINIMIZE = 283,
    AFFINE_HULL = 284,
    VENN = 285,
    CONVEX_COMBINATION = 286,
    POSITIVE_COMBINATION = 287,
    LINEAR_COMBINATION = 288,
    AFFINE_COMBINATION = 289,
    CONVEX_HULL = 290,
    CONIC_HULL = 291,
    LINEAR_HULL = 292,
    QUICK_HULL = 293,
    PAIRWISE_CHECK = 294,
    CONVEX_CHECK = 295,
    CONVEX_REPRESENTATION = 296,
    RECT_HULL = 297,
    SIMPLE_HULL = 298,
    DECOUPLED_CONVEX_HULL = 299,
    MAXIMIZE_RANGE = 300,
    MINIMIZE_RANGE = 301,
    MAXIMIZE_DOMAIN = 302,
    MINIMIZE_DOMAIN = 303,
    LEQ = 304,
    GEQ = 305,
    NEQ = 306,
    GOES_TO = 307,
    COMPOSE = 308,
    JOIN = 309,
    INVERSE = 310,
    COMPLEMENT = 311,
    IN = 312,
    CARRIED_BY = 313,
    TIME = 314,
    TIMECLOSURE = 315,
    UNION = 316,
    INTERSECTION = 317,
    VERTICAL_BAR = 318,
    SUCH_THAT = 319,
    SUBSET = 320,
    CODEGEN = 321,
    DECOUPLED_FARKAS = 322,
    FARKAS = 323,
    MAKE_UPPER_BOUND = 324,
    MAKE_LOWER_BOUND = 325,
    REL_OP = 326,
    RESTRICT_DOMAIN = 327,
    RESTRICT_RANGE = 328,
    SUPERSETOF = 329,
    SUBSETOF = 330,
    SAMPLE = 331,
    SYM_SAMPLE = 332,
    PROJECT_AWAY_SYMBOLS = 333,
    PROJECT_ON_SYMBOLS = 334,
    REACHABLE_FROM = 335,
    REACHABLE_OF = 336,
    ASSERT_UNSAT = 337,
    PARSE_EXPRESSION = 338,
    PARSE_FORMULA = 339,
    PARSE_RELATION = 340,
    p1 = 341,
    p2 = 342,
    p3 = 343,
    p4 = 344,
    p5 = 345,
    p6 = 346,
    p7 = 347,
    p8 = 348,
    p9 = 349,
    p10 = 350
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 112 "parser.yy" /* yacc.c:355  */

  int INT_VALUE;
  omega::coef_t COEF_VALUE;
  Rel_Op REL_OPERATOR;
  char *VAR_NAME;
  std::set<char *> *VAR_LIST;
  ast::Exp *EXP;
  std::set<ast::Exp *> *EXP_LIST;
  AST *ASTP;
  omega::Argument_Tuple ARGUMENT_TUPLE;
  AST_constraints *ASTCP;
  Declaration_Site *DECLARATION_SITE;
  omega::Relation *RELATION;
  tupleDescriptor *TUPLE_DESCRIPTOR;
  std::pair<std::vector<omega::Relation>, std::vector<omega::Relation> > *REL_TUPLE_PAIR;
  omega::Dynamic_Array1<omega::Relation> * RELATION_ARRAY_1D;
  std::string *STRING_VALUE;

#line 318 "parser.tab.cc" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_HH_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 335 "parser.tab.cc" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
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
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
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
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   732

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  108
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  159
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  309

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   350

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      99,   104,    91,    87,   102,    88,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   103,   101,
       2,     2,     2,     2,    92,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   106,     2,   107,   105,     2,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    89,    90,    93,    94,    95,    96,    97,    98,
     100
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   206,   206,   207,   207,   211,   212,   215,   225,   226,
     243,   252,   329,   415,   431,   454,   477,   496,   497,   500,
     516,   530,   547,   564,   564,   572,   582,   583,   596,   610,
     625,   638,   651,   667,   683,   699,   715,   731,   747,   760,
     773,   786,   799,   812,   825,   838,   851,   864,   879,   892,
     905,   918,   931,   944,   957,   970,   983,   996,  1009,  1022,
    1035,  1048,  1061,  1074,  1087,  1102,  1115,  1128,  1141,  1154,
    1169,  1184,  1199,  1213,  1228,  1243,  1258,  1273,  1288,  1303,
    1318,  1331,  1344,  1357,  1370,  1383,  1396,  1397,  1407,  1408,
    1407,  1441,  1462,  1471,  1472,  1475,  1476,  1477,  1480,  1480,
    1485,  1486,  1487,  1490,  1509,  1510,  1514,  1518,  1524,  1525,
    1528,  1536,  1537,  1540,  1541,  1544,  1545,  1551,  1552,  1553,
    1554,  1555,  1556,  1557,  1560,  1561,  1564,  1565,  1566,  1569,
    1570,  1573,  1576,  1579,  1580,  1583,  1584,  1587,  1597,  1597,
    1611,  1614,  1630,  1648,  1649,  1650,  1651,  1652,  1653,  1654,
    1666,  1672,  1690,  1698,  1702,  1710,  1754,  1768,  1793,  1818
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "VAR", "INT", "COEF", "STRING",
  "OPEN_BRACE", "CLOSE_BRACE", "SYMBOLIC", "NO_SIMPLIFY", "OR", "AND",
  "NOT", "ST", "APPROX", "IS_ASSIGNED", "FORALL", "EXISTS", "DOMAIN",
  "RANGE", "DIFFERENCE", "DIFFERENCE_TO_RELATION", "GIST", "GIVEN", "HULL",
  "WITHIN", "MAXIMIZE", "MINIMIZE", "AFFINE_HULL", "VENN",
  "CONVEX_COMBINATION", "POSITIVE_COMBINATION", "LINEAR_COMBINATION",
  "AFFINE_COMBINATION", "CONVEX_HULL", "CONIC_HULL", "LINEAR_HULL",
  "QUICK_HULL", "PAIRWISE_CHECK", "CONVEX_CHECK", "CONVEX_REPRESENTATION",
  "RECT_HULL", "SIMPLE_HULL", "DECOUPLED_CONVEX_HULL", "MAXIMIZE_RANGE",
  "MINIMIZE_RANGE", "MAXIMIZE_DOMAIN", "MINIMIZE_DOMAIN", "LEQ", "GEQ",
  "NEQ", "GOES_TO", "COMPOSE", "JOIN", "INVERSE", "COMPLEMENT", "IN",
  "CARRIED_BY", "TIME", "TIMECLOSURE", "UNION", "INTERSECTION",
  "VERTICAL_BAR", "SUCH_THAT", "SUBSET", "CODEGEN", "DECOUPLED_FARKAS",
  "FARKAS", "MAKE_UPPER_BOUND", "MAKE_LOWER_BOUND", "REL_OP",
  "RESTRICT_DOMAIN", "RESTRICT_RANGE", "SUPERSETOF", "SUBSETOF", "SAMPLE",
  "SYM_SAMPLE", "PROJECT_AWAY_SYMBOLS", "PROJECT_ON_SYMBOLS",
  "REACHABLE_FROM", "REACHABLE_OF", "ASSERT_UNSAT", "PARSE_EXPRESSION",
  "PARSE_FORMULA", "PARSE_RELATION", "p1", "'+'", "'-'", "p2", "p3", "'*'",
  "'@'", "p4", "p5", "p6", "p7", "p8", "p9", "'('", "p10", "';'", "','",
  "':'", "')'", "'^'", "'['", "']'", "$accept", "inputSequence", "$@1",
  "inputItem", "context", "relPairList", "relation", "$@2",
  "builtRelation", "$@3", "$@4", "optionalFormula", "formula_sep",
  "tupleDeclaration", "$@5", "optionalTupleVarList", "tupleVar", "varList",
  "varDecl", "varDeclOptBrackets", "globVarList", "globVar", "formula",
  "start_exists", "exists_sep", "start_forall", "forall_sep", "end_quant",
  "expList", "constraintChain", "simpleExp", "$@6", "argumentList", "exp",
  "reachable", "reachable_of", "nodeNameList", "realNodeNameList",
  "nodeSpecificationList", "realNodeSpecificationList", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,    43,    45,   342,
     343,    42,    64,   344,   345,   346,   347,   348,   349,    40,
     350,    59,    44,    58,    41,    94,    91,    93
};
# endif

#define YYPACT_NINF -141

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-141)))

#define YYTABLE_NINF -104

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -141,    55,  -141,   168,   -26,    87,  -141,   105,    48,   359,
     359,   359,   359,   359,   359,   359,   359,   359,   359,   359,
     359,   359,   359,   359,   359,   359,   359,   359,   359,   359,
     359,   359,   359,   359,   359,   359,   359,   359,   359,   359,
     359,   359,   278,   359,   359,   359,   359,   359,   359,   359,
     359,   359,   359,    17,   141,   359,   359,  -141,  -141,   391,
      50,  -141,  -141,   359,    53,    68,    79,  -141,  -141,  -141,
      30,    30,    30,    30,    30,   389,    15,    30,    30,    30,
      27,    30,    30,    30,    30,    30,    30,    30,    30,    30,
      30,    30,    30,    30,    30,    30,    30,    30,    30,    30,
      30,   412,   448,   359,   -19,   469,    30,    30,    30,    30,
     167,   167,    30,    30,    30,    30,   151,   153,   106,   615,
     471,   359,   359,   170,   359,   359,   359,   359,   359,   146,
     359,   359,  -141,   359,  -141,   -24,  -141,   524,    77,    -1,
      53,    93,    95,    59,    71,   209,    43,   120,   174,    -2,
      -2,   159,  -141,  -141,   131,   227,  -141,   105,   359,   359,
     359,  -141,  -141,   -19,   359,   359,   140,   359,  -141,    11,
     229,  -141,    17,  -141,    30,    30,  -141,   167,    38,   545,
     627,   627,   359,    38,    38,   566,  -141,  -141,  -141,  -141,
      59,  -141,  -141,  -141,  -141,  -141,    93,    95,    61,   169,
    -141,  -141,  -141,  -141,  -141,  -141,    53,    54,    53,    53,
    -141,   248,   150,  -141,    36,   152,    59,    59,    59,    59,
      59,   149,  -141,    30,    30,    30,   160,   615,   587,  -141,
     615,   251,  -141,     9,    46,   153,  -141,    30,  -141,   259,
     187,  -141,  -141,  -141,   174,   177,  -141,   -20,  -141,     6,
     252,  -141,   156,   267,  -141,  -141,  -141,    53,  -141,    53,
     159,  -141,   186,   186,  -141,  -141,  -141,  -141,   359,  -141,
     277,   359,  -141,   280,  -141,  -141,    64,  -141,    54,  -141,
      59,  -141,  -141,    69,    69,   615,   183,   615,    33,   284,
    -141,    47,  -141,    14,  -141,  -141,  -141,   359,   285,   359,
    -141,  -141,   287,   615,   192,   615,  -141,   359,   615
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     3,     1,     0,     0,    25,    23,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     5,     4,     0,
       0,    86,     7,     0,    98,   116,     0,   114,     6,    25,
      65,    45,    66,    43,    44,     0,    63,    36,    37,    57,
      46,    53,    50,    51,    52,    48,    58,    59,    60,    54,
      55,    56,    61,    62,    49,    33,    32,    35,    34,    67,
      68,     0,     0,     0,    17,    22,    39,    38,    82,    83,
      80,    81,    84,    85,    41,    42,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    27,
       0,    28,    40,     0,    10,     0,    16,     0,   137,   143,
       0,     0,     0,     0,     0,     0,    94,     0,    92,     0,
       0,     0,   119,   145,   134,     0,     8,     0,     0,     0,
       0,    11,    12,    17,     0,     0,     0,     0,   154,     0,
       0,   150,     0,    26,    71,    73,    72,    78,    76,     0,
      75,    74,     0,    77,    79,     0,    31,    30,     9,   138,
       0,   144,   121,   130,   125,   146,   129,   124,     0,   134,
      24,    88,    96,    97,    95,    91,     0,   100,     0,     0,
     109,     0,   110,   111,     0,     0,     0,     0,     0,     0,
       0,     0,   113,    69,    64,    47,     0,    18,    20,    14,
      21,     0,   152,     0,     0,     0,    13,    29,    70,     0,
       0,   120,   140,    98,    93,   137,   104,     0,   101,   105,
     118,   117,     0,     0,   127,   128,   126,     0,   131,     0,
     135,   136,   147,   148,   149,   133,   115,    15,     0,   153,
       0,     0,   155,     0,   151,   142,     0,    89,     0,    99,
       0,   112,   108,     0,     0,    19,     0,   159,     0,     0,
     139,    94,   102,   106,   132,   122,   123,     0,     0,     0,
     141,    90,     0,   158,     0,   156,   107,     0,   157
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -141,  -141,  -141,  -141,   126,   199,    -3,  -141,  -141,  -141,
    -141,    37,  -141,    84,  -141,  -141,    12,  -141,   118,   180,
    -141,   175,   -75,  -141,  -141,  -141,  -141,    51,  -130,   115,
     197,  -141,  -141,  -140,  -141,  -141,   165,  -141,   103,  -141
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     3,    58,   166,   104,   105,    64,   145,   243,
     291,   205,   206,   146,   147,   247,   248,   212,   213,   214,
      66,    67,   148,   149,   257,   150,   259,   295,   151,   152,
     153,   239,   276,   154,    60,    61,   117,   169,   171,   234
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      59,   210,   138,   195,   199,   164,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   159,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     240,   160,   119,   120,   272,     2,   138,   245,   139,   139,
     137,   270,   138,   186,   139,   192,   140,   249,   187,   198,
     141,   142,   208,   209,   138,    62,   139,   262,   263,   264,
     208,   209,   278,   165,   140,   298,   260,   279,   196,   197,
     265,   121,   122,   217,   218,   201,   123,   219,   190,   254,
     255,   217,   218,    63,   211,   219,   202,   203,    65,   280,
     202,   203,   271,   231,   133,   232,   116,   302,   174,   175,
     135,   177,   178,   179,   180,   181,   133,   183,   184,   133,
     185,   244,   135,   250,   251,   135,   299,   133,   249,   256,
     293,   143,   143,   135,   118,   246,   204,   143,   273,    68,
     204,   136,   144,   190,   168,   223,   224,   225,   190,   143,
     170,   227,   228,   172,   230,   241,   289,   155,   290,     4,
     144,     5,   182,   294,   176,     6,   189,     7,     8,   237,
     156,   157,   283,     9,   284,   208,   209,    10,    11,    12,
      13,    14,   193,    15,   194,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,   200,   217,   218,
     121,   122,   219,    38,    39,   123,   207,    40,    41,   125,
     216,   221,   233,   220,    42,    43,    44,    45,    46,   127,
     128,   229,    47,    48,    49,    50,    51,    52,    53,    54,
      55,   210,   253,   266,   269,   258,   217,   218,   131,   132,
     219,   267,   275,   281,   209,   285,   133,    56,   287,    57,
     282,   220,   135,   242,   217,   218,   189,   219,   219,  -103,
     286,    69,   103,   288,  -103,     6,   297,   300,   304,   226,
     292,   242,   306,     9,   303,   307,   305,    10,    11,    12,
      13,    14,   163,    15,   308,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,   277,   301,   252,
     215,   261,   222,    38,    39,   296,   191,   235,   274,     0,
       0,     0,     0,     0,     0,    43,    44,    45,    46,     0,
       0,     0,    47,    48,    49,    50,    51,    52,     0,    54,
      55,     0,    69,     0,     0,     0,     6,     0,     0,     0,
       0,     0,     0,     0,     9,     0,     0,    56,    10,    11,
      12,    13,    14,     0,    15,     0,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,     0,     0,
       0,     0,     0,   158,    38,    39,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    43,    44,    45,    46,
       0,     0,     0,    47,    48,    49,    50,    51,    52,     0,
      54,    55,   121,   122,   121,   122,     0,   123,     0,   123,
     124,   125,   124,   125,     0,     0,   126,     0,    56,     0,
       0,   127,   128,   127,   128,   121,   122,     0,     0,     0,
     123,     0,     0,   124,   125,     0,   129,   130,   129,   130,
     131,   132,   131,   132,   127,   128,     0,     0,   133,     0,
     133,     0,   134,     0,   135,     0,   135,     0,     0,   129,
     130,   121,   122,   131,   132,     0,   123,     0,     0,   124,
     125,   133,     0,   161,     0,     0,     0,   135,     0,     0,
     127,   128,   121,   122,   121,   122,     0,   123,     0,   123,
     124,   125,   124,   125,     0,   129,   130,     0,     0,   131,
     132,   127,   128,   127,   128,     0,     0,   133,     0,   162,
       0,     0,     0,   135,     0,     0,   129,   130,   129,   130,
     131,   132,   131,   132,     0,     0,     0,     0,   133,     0,
     133,     0,   167,     0,   135,   173,   135,   121,   122,     0,
       0,     0,   123,     0,     0,   124,   125,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   127,   128,   121,   122,
       0,     0,     0,   123,     0,     0,   124,   125,     0,     0,
       0,   129,   130,     0,     0,   131,   132,   127,   128,   121,
     122,     0,     0,   133,   123,   188,     0,   124,   125,   135,
       0,     0,   129,   130,     0,     0,   131,   132,   127,   128,
     121,   122,     0,     0,   133,   123,   236,     0,   124,   125,
     135,     0,     0,   129,   130,     0,     0,   131,   132,   127,
     128,     0,     0,     0,     0,   133,     0,     0,   121,   122,
     238,   135,     0,   123,   129,   130,   124,   125,   131,   132,
     121,   122,     0,     0,     0,   123,   133,   127,   128,   125,
     268,     0,   135,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   129,   130,     0,     0,   131,   132,     0,     0,
       0,     0,     0,     0,   133,     0,     0,     0,   131,   132,
     135,     0,     0,     0,     0,     0,   133,     0,     0,     0,
       0,     0,   135
};

static const yytype_int16 yycheck[] =
{
       3,     3,     3,   143,   144,    24,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    24,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
     190,    24,    55,    56,     8,     0,     3,     3,     5,     5,
      63,    52,     3,    87,     5,   140,    13,   207,    92,   144,
      17,    18,    11,    12,     3,   101,     5,   217,   218,   219,
      11,    12,   102,   102,    13,    52,   216,   107,    17,    18,
     220,    53,    54,    87,    88,    52,    58,    91,    99,    63,
      64,    87,    88,    16,   106,    91,    63,    64,     3,   103,
      63,    64,   103,   102,    99,   104,    99,   103,   121,   122,
     105,   124,   125,   126,   127,   128,    99,   130,   131,    99,
     133,   206,   105,   208,   209,   105,   103,    99,   278,   103,
     280,    88,    88,   105,     3,    91,   103,    88,   102,   101,
     103,   101,    99,    99,     3,   158,   159,   160,    99,    88,
       7,   164,   165,    57,   167,   104,   102,    99,   104,     1,
      99,     3,    26,   104,     4,     7,    99,     9,    10,   182,
     101,   102,   257,    15,   259,    11,    12,    19,    20,    21,
      22,    23,    99,    25,    99,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,     8,    87,    88,
      53,    54,    91,    55,    56,    58,   106,    59,    60,    62,
      71,     4,     3,   102,    66,    67,    68,    69,    70,    72,
      73,   101,    74,    75,    76,    77,    78,    79,    80,    81,
      82,     3,   102,   104,     3,   103,    87,    88,    91,    92,
      91,   101,     3,   107,    12,   268,    99,    99,   271,   101,
       3,   102,   105,   104,    87,    88,    99,    91,    91,   102,
       3,     3,     4,     3,   107,     7,   103,     3,     3,   163,
     278,   104,     5,    15,   297,   103,   299,    19,    20,    21,
      22,    23,   103,    25,   307,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,   243,   291,   211,
     150,   216,   157,    55,    56,   284,   139,   172,   235,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    -1,     3,    -1,    -1,    -1,     7,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    99,    19,    20,
      21,    22,    23,    -1,    25,    -1,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    -1,    -1,
      -1,    -1,    -1,    24,    55,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    74,    75,    76,    77,    78,    79,    -1,
      81,    82,    53,    54,    53,    54,    -1,    58,    -1,    58,
      61,    62,    61,    62,    -1,    -1,    65,    -1,    99,    -1,
      -1,    72,    73,    72,    73,    53,    54,    -1,    -1,    -1,
      58,    -1,    -1,    61,    62,    -1,    87,    88,    87,    88,
      91,    92,    91,    92,    72,    73,    -1,    -1,    99,    -1,
      99,    -1,   101,    -1,   105,    -1,   105,    -1,    -1,    87,
      88,    53,    54,    91,    92,    -1,    58,    -1,    -1,    61,
      62,    99,    -1,   101,    -1,    -1,    -1,   105,    -1,    -1,
      72,    73,    53,    54,    53,    54,    -1,    58,    -1,    58,
      61,    62,    61,    62,    -1,    87,    88,    -1,    -1,    91,
      92,    72,    73,    72,    73,    -1,    -1,    99,    -1,   101,
      -1,    -1,    -1,   105,    -1,    -1,    87,    88,    87,    88,
      91,    92,    91,    92,    -1,    -1,    -1,    -1,    99,    -1,
      99,    -1,   103,    -1,   105,   104,   105,    53,    54,    -1,
      -1,    -1,    58,    -1,    -1,    61,    62,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    53,    54,
      -1,    -1,    -1,    58,    -1,    -1,    61,    62,    -1,    -1,
      -1,    87,    88,    -1,    -1,    91,    92,    72,    73,    53,
      54,    -1,    -1,    99,    58,   101,    -1,    61,    62,   105,
      -1,    -1,    87,    88,    -1,    -1,    91,    92,    72,    73,
      53,    54,    -1,    -1,    99,    58,   101,    -1,    61,    62,
     105,    -1,    -1,    87,    88,    -1,    -1,    91,    92,    72,
      73,    -1,    -1,    -1,    -1,    99,    -1,    -1,    53,    54,
     104,   105,    -1,    58,    87,    88,    61,    62,    91,    92,
      53,    54,    -1,    -1,    -1,    58,    99,    72,    73,    62,
     103,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    91,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,    -1,    -1,    91,    92,
     105,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,    -1,
      -1,    -1,   105
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   109,     0,   110,     1,     3,     7,     9,    10,    15,
      19,    20,    21,    22,    23,    25,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    55,    56,
      59,    60,    66,    67,    68,    69,    70,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    99,   101,   111,   114,
     142,   143,   101,    16,   115,     3,   128,   129,   101,     3,
     114,   114,   114,   114,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   114,   114,   114,   114,   114,
     114,   114,   114,     4,   113,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   114,    99,   144,     3,   114,
     114,    53,    54,    58,    61,    62,    65,    72,    73,    87,
      88,    91,    92,    99,   101,   105,   101,   114,     3,     5,
      13,    17,    18,    88,    99,   116,   121,   122,   130,   131,
     133,   136,   137,   138,   141,    99,   101,   102,    24,    24,
      24,   101,   101,   113,    24,   102,   112,   103,     3,   145,
       7,   146,    57,   104,   114,   114,     4,   114,   114,   114,
     114,   114,    26,   114,   114,   114,    87,    92,   101,    99,
      99,   138,   130,    99,    99,   141,    17,    18,   130,   141,
       8,    52,    63,    64,   103,   119,   120,   106,    11,    12,
       3,   106,   125,   126,   127,   127,    71,    87,    88,    91,
     102,     4,   129,   114,   114,   114,   112,   114,   114,   101,
     114,   102,   104,     3,   147,   144,   101,   114,   104,   139,
     141,   104,   104,   117,   130,     3,    91,   123,   124,   141,
     130,   130,   126,   102,    63,    64,   103,   132,   103,   134,
     136,   137,   141,   141,   141,   136,   104,   101,   103,     3,
      52,   103,     8,   102,   146,     3,   140,   121,   102,   107,
     103,   107,     3,   130,   130,   114,     3,   114,     3,   102,
     104,   118,   124,   141,   104,   135,   135,   103,    52,   103,
       3,   119,   103,   114,     3,   114,     5,   103,   114
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   108,   109,   110,   109,   111,   111,   111,   111,   111,
     111,   111,   111,   111,   111,   111,   111,   112,   112,   113,
     113,   113,   113,   115,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   114,   114,   114,   117,   118,
     116,   116,   116,   119,   119,   120,   120,   120,   122,   121,
     123,   123,   123,   124,   124,   124,   124,   124,   125,   125,
     126,   127,   127,   128,   128,   129,   129,   130,   130,   130,
     130,   130,   130,   130,   131,   131,   132,   132,   132,   133,
     133,   134,   135,   136,   136,   137,   137,   138,   139,   138,
     138,   140,   140,   141,   141,   141,   141,   141,   141,   141,
     142,   143,   144,   145,   145,   146,   147,   147,   147,   147
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     0,     3,     1,     2,     2,     3,     4,
       2,     3,     3,     4,     4,     5,     2,     0,     2,     5,
       3,     3,     1,     0,     4,     1,     3,     2,     2,     4,
       3,     3,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     4,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     4,     2,     2,     2,     2,     4,
       4,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     2,     2,     2,     2,     1,     2,     0,     0,
       6,     2,     1,     2,     0,     1,     1,     1,     0,     4,
       0,     1,     3,     1,     1,     1,     3,     5,     3,     1,
       1,     1,     3,     3,     1,     4,     1,     3,     3,     1,
       3,     2,     5,     5,     2,     2,     1,     1,     1,     2,
       2,     1,     1,     3,     1,     3,     3,     1,     0,     5,
       3,     3,     1,     1,     2,     1,     2,     3,     3,     3,
       3,     5,     3,     3,     1,     3,     5,     7,     5,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
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
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

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
      int yyn = yypact[*yyssp];
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
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
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
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
          yyp++;
          yyformat++;
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
  switch (yytype)
    {
          case 3: /* VAR  */
#line 174 "parser.yy" /* yacc.c:1257  */
      {delete []((*yyvaluep).VAR_NAME);}
#line 1480 "parser.tab.cc" /* yacc.c:1257  */
        break;

    case 6: /* STRING  */
#line 175 "parser.yy" /* yacc.c:1257  */
      {delete ((*yyvaluep).STRING_VALUE);}
#line 1486 "parser.tab.cc" /* yacc.c:1257  */
        break;

    case 112: /* context  */
#line 176 "parser.yy" /* yacc.c:1257  */
      {delete ((*yyvaluep).RELATION);}
#line 1492 "parser.tab.cc" /* yacc.c:1257  */
        break;

    case 113: /* relPairList  */
#line 176 "parser.yy" /* yacc.c:1257  */
      {delete ((*yyvaluep).REL_TUPLE_PAIR);}
#line 1498 "parser.tab.cc" /* yacc.c:1257  */
        break;

    case 114: /* relation  */
#line 176 "parser.yy" /* yacc.c:1257  */
      {delete ((*yyvaluep).RELATION);}
#line 1504 "parser.tab.cc" /* yacc.c:1257  */
        break;

    case 116: /* builtRelation  */
#line 176 "parser.yy" /* yacc.c:1257  */
      {delete ((*yyvaluep).RELATION);}
#line 1510 "parser.tab.cc" /* yacc.c:1257  */
        break;

    case 119: /* optionalFormula  */
#line 176 "parser.yy" /* yacc.c:1257  */
      {delete ((*yyvaluep).ASTP);}
#line 1516 "parser.tab.cc" /* yacc.c:1257  */
        break;

    case 121: /* tupleDeclaration  */
#line 176 "parser.yy" /* yacc.c:1257  */
      {delete ((*yyvaluep).TUPLE_DESCRIPTOR);}
#line 1522 "parser.tab.cc" /* yacc.c:1257  */
        break;

    case 125: /* varList  */
#line 183 "parser.yy" /* yacc.c:1257  */
      {
  for (std::set<char *>::iterator i = ((*yyvaluep).VAR_LIST)->begin(); i != ((*yyvaluep).VAR_LIST)->end(); i++)
    delete []*i;
  delete ((*yyvaluep).VAR_LIST);
 }
#line 1532 "parser.tab.cc" /* yacc.c:1257  */
        break;

    case 126: /* varDecl  */
#line 176 "parser.yy" /* yacc.c:1257  */
      {delete ((*yyvaluep).DECLARATION_SITE);}
#line 1538 "parser.tab.cc" /* yacc.c:1257  */
        break;

    case 127: /* varDeclOptBrackets  */
#line 176 "parser.yy" /* yacc.c:1257  */
      {delete ((*yyvaluep).DECLARATION_SITE);}
#line 1544 "parser.tab.cc" /* yacc.c:1257  */
        break;

    case 130: /* formula  */
#line 176 "parser.yy" /* yacc.c:1257  */
      {delete ((*yyvaluep).ASTP);}
#line 1550 "parser.tab.cc" /* yacc.c:1257  */
        break;

    case 136: /* expList  */
#line 178 "parser.yy" /* yacc.c:1257  */
      {
  for (std::set<ast::Exp *>::iterator i = ((*yyvaluep).EXP_LIST)->begin(); i != ((*yyvaluep).EXP_LIST)->end(); i++)
    delete *i;
  delete ((*yyvaluep).EXP_LIST);
 }
#line 1560 "parser.tab.cc" /* yacc.c:1257  */
        break;

    case 137: /* constraintChain  */
#line 176 "parser.yy" /* yacc.c:1257  */
      {delete ((*yyvaluep).ASTCP);}
#line 1566 "parser.tab.cc" /* yacc.c:1257  */
        break;

    case 138: /* simpleExp  */
#line 177 "parser.yy" /* yacc.c:1257  */
      {delete ((*yyvaluep).EXP);}
#line 1572 "parser.tab.cc" /* yacc.c:1257  */
        break;

    case 141: /* exp  */
#line 177 "parser.yy" /* yacc.c:1257  */
      {delete ((*yyvaluep).EXP);}
#line 1578 "parser.tab.cc" /* yacc.c:1257  */
        break;

    case 142: /* reachable  */
#line 176 "parser.yy" /* yacc.c:1257  */
      {delete ((*yyvaluep).RELATION_ARRAY_1D);}
#line 1584 "parser.tab.cc" /* yacc.c:1257  */
        break;

    case 143: /* reachable_of  */
#line 176 "parser.yy" /* yacc.c:1257  */
      {delete ((*yyvaluep).RELATION);}
#line 1590 "parser.tab.cc" /* yacc.c:1257  */
        break;


      default:
        break;
    }
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
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

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
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
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
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
        case 3:
#line 207 "parser.yy" /* yacc.c:1646  */
    { assert( current_Declaration_Site == globalDecls);}
#line 1854 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 6:
#line 212 "parser.yy" /* yacc.c:1646  */
    {
             simplify = false; 
          }
#line 1862 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 7:
#line 215 "parser.yy" /* yacc.c:1646  */
    {
            flushScanBuffer();
            std::cout << err_msg;
            err_msg.clear();
            current_Declaration_Site = globalDecls;
            need_coef = false;
            std::cout << "...skipping to statement end..." << std::endl;
            delete relationDecl;
            relationDecl = NULL;
          }
#line 1877 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 8:
#line 225 "parser.yy" /* yacc.c:1646  */
    {flushScanBuffer();}
#line 1883 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 9:
#line 226 "parser.yy" /* yacc.c:1646  */
    {
            flushScanBuffer();
            try {
              if(simplify)
              (yyvsp[-1].RELATION)->simplify(redundant_conj_level, redundant_constr_level);
              else
              (yyvsp[-1].RELATION)->simplify();
              Relation *r = relationMap[std::string((yyvsp[-3].VAR_NAME))];
              if (r != NULL) delete r;
              relationMap[std::string((yyvsp[-3].VAR_NAME))] = (yyvsp[-1].RELATION);
            }
             catch(const std::exception &e){
                  std::cout << e.what() << std::endl; 
             } 
           
            delete [](yyvsp[-3].VAR_NAME);
          }
#line 1905 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 10:
#line 243 "parser.yy" /* yacc.c:1646  */
    {
            flushScanBuffer();
            if(simplify)
            (yyvsp[-1].RELATION)->simplify(redundant_conj_level, redundant_constr_level);
            else
            (yyvsp[-1].RELATION)->simplify();
            (yyvsp[-1].RELATION)->print_with_subs(stdout); 
            delete (yyvsp[-1].RELATION);
          }
#line 1919 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 11:
#line 252 "parser.yy" /* yacc.c:1646  */
    {
#if defined(OMIT_GETRUSAGE)
            printf("'time' requires getrusage, but the omega calclator was compiled with OMIT_GETRUSAGE set!\n");
#else
            flushScanBuffer();
            printf("\n");
            int t;
            Relation R;
            bool SKIP_FULL_CHECK = getenv("OC_TIMING_SKIP_FULL_CHECK");
            ((yyvsp[-1].RELATION))->and_with_GEQ();
            start_clock();
            for (t=1;t<=100;t++) {
              R = *(yyvsp[-1].RELATION);
              R.finalize();
            }
            int copyTime = clock_diff();
            start_clock();
            for (t=1;t<=100;t++) {
              R = *(yyvsp[-1].RELATION);
              R.finalize();
              R.simplify();  /* default simplification effort */
            }
            int simplifyTime = clock_diff() -copyTime;
            Relation R2;
            if (!SKIP_FULL_CHECK) {
              start_clock();
              for (t=1;t<=100;t++) {
                R2 = *(yyvsp[-1].RELATION);
                R2.finalize();
                R2.simplify(2,4); /* maximal simplification effort */
              }
            }
            int excessiveTime = clock_diff() - copyTime;
            printf("Times (in microseconds): \n");
            printf("%5d us to copy original set of constraints\n",copyTime/100);
            printf("%5d us to do the default amount of simplification, obtaining: \n\t", simplifyTime/100);
            R.print_with_subs(stdout); 
            printf("\n"); 
            if (!SKIP_FULL_CHECK) {
              printf("%5d us to do the maximum (i.e., excessive) amount of simplification, obtaining: \n\t", excessiveTime/100);
              R2.print_with_subs(stdout); 
              printf("\n");
            }
            if (!anyTimingDone) {
              bool warn = false;
#ifndef SPEED 
              warn =true;
#endif
#ifndef NDEBUG
              warn = true;
#endif
              if (warn) {
                printf("WARNING: The Omega calculator was compiled with options that force\n");
                printf("it to perform additional consistency and error checks\n");
                printf("that may slow it down substantially\n");
                printf("\n");
              }
              printf("NOTE: These times relect the time of the current _implementation_\n");
              printf("of our algorithms. Performance bugs do exist. If you intend to publish or \n");
              printf("report on the performance on the Omega test, we respectfully but strongly \n");
              printf("request that send your test cases to us to allow us to determine if the \n");
              printf("times are appropriate, and if the way you are using the Omega library to \n"); 
              printf("solve your problem is the most effective way.\n");
              printf("\n");

              printf("Also, please be aware that over the past two years, we have focused our \n");
              printf("efforts on the expressive power of the Omega library, sometimes at the\n");
              printf("expensive of raw speed. Our original implementation of the Omega test\n");
              printf("was substantially faster on the limited domain it handled.\n");
              printf("\n");
              printf("  Thanks, \n");
              printf("  the Omega Team \n");
            }
            anyTimingDone = true;
            delete (yyvsp[-1].RELATION);
#endif
          }
#line 2001 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 12:
#line 329 "parser.yy" /* yacc.c:1646  */
    {
#if defined(OMIT_GETRUSAGE)
            printf("'timeclosure' requires getrusage, but the omega calclator was compiled with OMIT_GETRUSAGE set!\n");
#else
            flushScanBuffer();
            try {
              int t;
              Relation R;
              ((yyvsp[-1].RELATION))->and_with_GEQ();
              start_clock();
              for (t=1;t<=100;t++) {
                R = *(yyvsp[-1].RELATION);
                R.finalize();
              }
              int copyTime = clock_diff();
              start_clock();
              for (t=1;t<=100;t++) {
                R = *(yyvsp[-1].RELATION);
                R.finalize();
                R.simplify();
              }
              int simplifyTime = clock_diff() -copyTime;
              Relation Rclosed;
              start_clock();
              for (t=1;t<=100;t++) {
                Rclosed = *(yyvsp[-1].RELATION);
                Rclosed.finalize();
                Rclosed = TransitiveClosure(Rclosed, 1,Relation::Null());
              }
              int closureTime = clock_diff() - copyTime;
              Relation R2;
              start_clock();
              for (t=1;t<=100;t++) {
                R2 = *(yyvsp[-1].RELATION);
                R2.finalize();
                R2.simplify(2,4);
              }
              int excessiveTime = clock_diff() - copyTime;
              printf("Times (in microseconds): \n");
              printf("%5d us to copy original set of constraints\n",copyTime/100);
              printf("%5d us to do the default amount of simplification, obtaining: \n\t", simplifyTime/100);
              R.print_with_subs(stdout); 
              printf("\n"); 
              printf("%5d us to do the maximum (i.e., excessive) amount of simplification, obtaining: \n\t", excessiveTime/100);
              R2.print_with_subs(stdout); 
              printf("%5d us to do the transitive closure, obtaining: \n\t", closureTime/100);
              Rclosed.print_with_subs(stdout);
              printf("\n");
              if (!anyTimingDone) {
                bool warn = false;
#ifndef SPEED 
                warn =true;
#endif
#ifndef NDEBUG
                warn = true;
#endif
                if (warn) {
                  printf("WARNING: The Omega calculator was compiled with options that force\n");
                  printf("it to perform additional consistency and error checks\n");
                  printf("that may slow it down substantially\n");
                  printf("\n");
                }
                printf("NOTE: These times relect the time of the current _implementation_\n");
                printf("of our algorithms. Performance bugs do exist. If you intend to publish or \n");
                printf("report on the performance on the Omega test, we respectfully but strongly \n");
                printf("request that send your test cases to us to allow us to determine if the \n");
                printf("times are appropriate, and if the way you are using the Omega library to \n"); 
                printf("solve your problem is the most effective way.\n");
                printf("\n");
              
                printf("Also, please be aware that over the past two years, we have focused our \n");
                printf("efforts on the expressive power of the Omega library, sometimes at the\n");
                printf("expensive of raw speed. Our original implementation of the Omega test\n");
                printf("was substantially faster on the limited domain it handled.\n");
                printf("\n");
                printf("  Thanks, \n");
                printf("  the Omega Team \n");
              }
              anyTimingDone = true;
            }
            catch (const std::exception &e) {
              std::cout << e.what() << std::endl;
            }
            delete (yyvsp[-1].RELATION);
#endif
          }
#line 2092 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 13:
#line 415 "parser.yy" /* yacc.c:1646  */
    {
            flushScanBuffer();
            try {
              if (Must_Be_Subset(copy(*(yyvsp[-3].RELATION)), copy(*(yyvsp[-1].RELATION))))
                std::cout << "True" << std::endl;
              else if (Might_Be_Subset(copy(*(yyvsp[-3].RELATION)), copy(*(yyvsp[-1].RELATION))))
                std::cout << "Possible" << std::endl;
              else
                std::cout << "False" << std::endl;
            }
            catch (const std::exception &e) {
              std::cout << e.what() << std::endl;
            }
            delete (yyvsp[-3].RELATION);
            delete (yyvsp[-1].RELATION);
          }
#line 2113 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 14:
#line 431 "parser.yy" /* yacc.c:1646  */
    {
            flushScanBuffer();
#ifdef BUILD_CODEGEN
            try {
              CodeGen cg((yyvsp[-2].REL_TUPLE_PAIR)->first, (yyvsp[-2].REL_TUPLE_PAIR)->second, *(yyvsp[-1].RELATION));
              CG_result *cgr = cg.buildAST();
              if (cgr != NULL) {
                std::string s = cgr->printString();
                std::cout << s << std::endl;
                delete cgr;
              }
              else
                std::cout << "/* empty */" << std::endl;
            }
            catch (const std::exception &e) {
              std::cout << e.what() << std::endl;
            }
#else
            std::cout << "CodeGen package not built" << std::endl;
#endif
            delete (yyvsp[-1].RELATION);
            delete (yyvsp[-2].REL_TUPLE_PAIR);
          }
#line 2141 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 15:
#line 454 "parser.yy" /* yacc.c:1646  */
    {
            flushScanBuffer();
#ifdef BUILD_CODEGEN
            try {
              CodeGen cg((yyvsp[-2].REL_TUPLE_PAIR)->first, (yyvsp[-2].REL_TUPLE_PAIR)->second, *(yyvsp[-1].RELATION));
              CG_result *cgr = cg.buildAST((yyvsp[-3].INT_VALUE));
              if (cgr != NULL) {
                std::string s = cgr->printString();
                std::cout << s << std::endl;
                delete cgr;
              }
              else
                std::cout << "/* empty */" << std::endl;
            }
            catch (const std::exception &e) {
              std::cout << e.what() << std::endl;
            }
#else
            std::cout << "CodeGen package not built" << std::endl;
#endif
            delete (yyvsp[-1].RELATION);
            delete (yyvsp[-2].REL_TUPLE_PAIR);
          }
#line 2169 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 16:
#line 477 "parser.yy" /* yacc.c:1646  */
    {
            flushScanBuffer();
            Dynamic_Array1<Relation> &final = *(yyvsp[-1].RELATION_ARRAY_1D);
            bool any_sat = false;
            int i,n_nodes = reachable_info->node_names.size();
            for(i = 1; i <= n_nodes; i++)
              if(final[i].is_upper_bound_satisfiable()) {
                any_sat = true;
                std::cout << "Node " << reachable_info->node_names[i] << ": ";
                final[i].print_with_subs(stdout);
              }
            if(!any_sat)
              std::cout << "No nodes reachable.\n";
            delete (yyvsp[-1].RELATION_ARRAY_1D);
            delete reachable_info;
          }
#line 2190 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 17:
#line 496 "parser.yy" /* yacc.c:1646  */
    {(yyval.RELATION) = new Relation(); *(yyval.RELATION) = Relation::Null();}
#line 2196 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 18:
#line 497 "parser.yy" /* yacc.c:1646  */
    {(yyval.RELATION) = (yyvsp[0].RELATION); }
#line 2202 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 19:
#line 500 "parser.yy" /* yacc.c:1646  */
    {
              try {
                (yyvsp[-4].REL_TUPLE_PAIR)->first.push_back(*(yyvsp[-2].RELATION));
                (yyvsp[-4].REL_TUPLE_PAIR)->second.push_back(*(yyvsp[0].RELATION));
              }
              catch (const std::exception &e) {
                delete (yyvsp[-4].REL_TUPLE_PAIR);
                delete (yyvsp[-2].RELATION);
                delete (yyvsp[0].RELATION);
                yyerror(e.what());
                YYERROR;
              }
              delete (yyvsp[-2].RELATION);
              delete (yyvsp[0].RELATION);
              (yyval.REL_TUPLE_PAIR) = (yyvsp[-4].REL_TUPLE_PAIR);
            }
#line 2223 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 20:
#line 516 "parser.yy" /* yacc.c:1646  */
    {
              try {
                (yyvsp[-2].REL_TUPLE_PAIR)->first.push_back(Identity((yyvsp[0].RELATION)->n_set()));
                (yyvsp[-2].REL_TUPLE_PAIR)->second.push_back(*(yyvsp[0].RELATION));
              }
              catch (const std::exception &e) {
                delete (yyvsp[-2].REL_TUPLE_PAIR);
                delete (yyvsp[0].RELATION);
                yyerror(e.what());
                YYERROR;
              }
              delete (yyvsp[0].RELATION);
              (yyval.REL_TUPLE_PAIR) = (yyvsp[-2].REL_TUPLE_PAIR);
            }
#line 2242 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 21:
#line 530 "parser.yy" /* yacc.c:1646  */
    {
              std::pair<std::vector<Relation>, std::vector<Relation> > *rtp = new std::pair<std::vector<Relation>, std::vector<Relation> >();
              try {
                rtp->first.push_back(*(yyvsp[-2].RELATION));
                rtp->second.push_back(*(yyvsp[0].RELATION));
              }
              catch (const std::exception &e) {
                delete rtp;
                delete (yyvsp[-2].RELATION);
                delete (yyvsp[0].RELATION);
                yyerror(e.what());
                YYERROR;
              }
              delete (yyvsp[-2].RELATION);
              delete (yyvsp[0].RELATION);
              (yyval.REL_TUPLE_PAIR) = rtp;
            }
#line 2264 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 22:
#line 547 "parser.yy" /* yacc.c:1646  */
    {
              std::pair<std::vector<Relation>, std::vector<Relation> > *rtp = new std::pair<std::vector<Relation>, std::vector<Relation> >();
              try {
                rtp->first.push_back(Identity((yyvsp[0].RELATION)->n_set()));
                rtp->second.push_back(*(yyvsp[0].RELATION));
              }
              catch (const std::exception &e) {
                delete rtp;
                delete (yyvsp[0].RELATION);
                yyerror(e.what());
                YYERROR;
              }
              delete (yyvsp[0].RELATION);
              (yyval.REL_TUPLE_PAIR) = rtp;
            }
#line 2284 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 23:
#line 564 "parser.yy" /* yacc.c:1646  */
    {need_coef = true; relationDecl = new Declaration_Site();}
#line 2290 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 24:
#line 565 "parser.yy" /* yacc.c:1646  */
    {
           need_coef = false;
           (yyval.RELATION) = (yyvsp[-1].RELATION); 
           current_Declaration_Site = globalDecls;
           delete relationDecl;
           relationDecl = NULL;
         }
#line 2302 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 25:
#line 572 "parser.yy" /* yacc.c:1646  */
    {
           Relation *r = relationMap[std::string((yyvsp[0].VAR_NAME))];
           if (r == NULL) {
             yyerror(std::string("relation ") + to_string((yyvsp[0].VAR_NAME)) + std::string(" not declared"));
             delete [](yyvsp[0].VAR_NAME);
             YYERROR;
           }
           (yyval.RELATION) = new Relation(*r);
           delete [](yyvsp[0].VAR_NAME);
         }
#line 2317 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 26:
#line 582 "parser.yy" /* yacc.c:1646  */
    {(yyval.RELATION) = (yyvsp[-1].RELATION);}
#line 2323 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 27:
#line 583 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = TransitiveClosure(*(yyvsp[-1].RELATION), 1, Relation::Null());
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[-1].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[-1].RELATION);
         }
#line 2341 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 28:
#line 596 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             int vars = (yyvsp[-1].RELATION)->n_inp();
             *(yyval.RELATION) = Union(Identity(vars), TransitiveClosure(*(yyvsp[-1].RELATION), 1, Relation::Null()));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[-1].RELATION);
             yyerror(e.what());
             YYERROR;
           }           
           delete (yyvsp[-1].RELATION);
         }
#line 2360 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 29:
#line 610 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION)= TransitiveClosure(*(yyvsp[-3].RELATION), 1, *(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[-3].RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }           
           delete (yyvsp[-3].RELATION);
           delete (yyvsp[0].RELATION);
         }
#line 2380 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 30:
#line 625 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = ApproxClosure(*(yyvsp[-2].RELATION)); 
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[-2].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[-2].RELATION);
         }
#line 2398 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 31:
#line 638 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = calculateTransitiveClosure(*(yyvsp[-2].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[-2].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[-2].RELATION);
         }
#line 2416 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 32:
#line 651 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             Relation o(*(yyvsp[0].RELATION));
             Relation r(*(yyvsp[0].RELATION));
             r = Join(r,LexForward((yyvsp[0].RELATION)->n_out()));
             *(yyval.RELATION) = Difference(o,r);
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }           
           delete (yyvsp[0].RELATION);
         }
#line 2437 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 33:
#line 667 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             Relation o(*(yyvsp[0].RELATION));
             Relation r(*(yyvsp[0].RELATION));
             r = Join(r,Inverse(LexForward((yyvsp[0].RELATION)->n_out())));
             *(yyval.RELATION) = Difference(o,r);
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 2458 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 34:
#line 683 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             Relation o(*(yyvsp[0].RELATION));
             Relation r(*(yyvsp[0].RELATION));
             r = Join(LexForward((yyvsp[0].RELATION)->n_inp()),r);
             *(yyval.RELATION) = Difference(o,r);
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 2479 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 35:
#line 699 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             Relation o(*(yyvsp[0].RELATION));
             Relation r(*(yyvsp[0].RELATION));
             r = Join(Inverse(LexForward((yyvsp[0].RELATION)->n_inp())),r);
             *(yyval.RELATION) = Difference(o,r);
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 2500 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 36:
#line 715 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             Relation c(*(yyvsp[0].RELATION));
             Relation r(*(yyvsp[0].RELATION));
             *(yyval.RELATION) = Cross_Product(Relation(*(yyvsp[0].RELATION)),c);
             *(yyval.RELATION) = Difference(r,Domain(Intersection(*(yyval.RELATION),LexForward((yyval.RELATION)->n_inp()))));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 2521 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 37:
#line 731 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             Relation c(*(yyvsp[0].RELATION));
             Relation r(*(yyvsp[0].RELATION));
             *(yyval.RELATION) = Cross_Product(Relation(*(yyvsp[0].RELATION)),c);
             *(yyval.RELATION) = Difference(r,Range(Intersection(*(yyval.RELATION),LexForward((yyval.RELATION)->n_inp()))));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);       
         }
#line 2542 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 38:
#line 747 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Farkas(*(yyvsp[0].RELATION), Basic_Farkas);
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 2560 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 39:
#line 760 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Farkas(*(yyvsp[0].RELATION), Decoupled_Farkas);
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 2578 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 40:
#line 773 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = ConicClosure(*(yyvsp[-1].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[-1].RELATION);
             yyerror(e.what());
             YYERROR;
           }             
           delete (yyvsp[-1].RELATION);
         }
#line 2596 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 41:
#line 786 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Project_Sym(*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 2614 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 42:
#line 799 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Project_On_Sym(*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 2632 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 43:
#line 812 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Deltas(*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 2650 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 44:
#line 825 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = DeltasToRelation(*(yyvsp[0].RELATION),(yyvsp[0].RELATION)->n_set(),(yyvsp[0].RELATION)->n_set());
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 2668 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 45:
#line 838 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Domain(*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 2686 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 46:
#line 851 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = VennDiagramForm(*(yyvsp[0].RELATION),Relation::True(*(yyvsp[0].RELATION)));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 2704 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 47:
#line 864 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = VennDiagramForm(*(yyvsp[-2].RELATION),*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[-2].RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[-2].RELATION);
           delete (yyvsp[0].RELATION);
         }
#line 2724 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 48:
#line 879 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = ConvexHull(*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 2742 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 49:
#line 892 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = DecoupledConvexHull(*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 2760 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 50:
#line 905 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Farkas(*(yyvsp[0].RELATION),Positive_Combination_Farkas);
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 2778 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 51:
#line 918 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Farkas(*(yyvsp[0].RELATION),Linear_Combination_Farkas);
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 2796 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 52:
#line 931 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Farkas(*(yyvsp[0].RELATION),Affine_Combination_Farkas);
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 2814 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 53:
#line 944 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Farkas(*(yyvsp[0].RELATION),Convex_Combination_Farkas);
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }           
           delete (yyvsp[0].RELATION);
         }
#line 2832 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 54:
#line 957 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = CheckForConvexRepresentation(CheckForConvexPairs(*(yyvsp[0].RELATION)));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 2850 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 55:
#line 970 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = CheckForConvexRepresentation(*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 2868 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 56:
#line 983 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = ConvexRepresentation(*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 2886 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 57:
#line 996 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = AffineHull(*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 2904 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 58:
#line 1009 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = ConicHull(*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 2922 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 59:
#line 1022 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = LinearHull(*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 2940 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 60:
#line 1035 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = QuickHull(*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 2958 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 61:
#line 1048 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = RectHull(*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 2976 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 62:
#line 1061 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = SimpleHull(*(yyvsp[0].RELATION), true, true);
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 2994 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 63:
#line 1074 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Hull(*(yyvsp[0].RELATION),true,1,Relation::Null());
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 3012 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 64:
#line 1087 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Hull(*(yyvsp[-2].RELATION),true,1,*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[-2].RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[-2].RELATION);
           delete (yyvsp[0].RELATION);
         }
#line 3032 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 65:
#line 1102 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Approximate(*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 3050 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 66:
#line 1115 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Range(*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 3068 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 67:
#line 1128 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Inverse(*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 3086 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 68:
#line 1141 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Complement(*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 3104 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 69:
#line 1154 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Gist(*(yyvsp[-2].RELATION),*(yyvsp[0].RELATION),1);
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[-2].RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[-2].RELATION);
           delete (yyvsp[0].RELATION);
         }
#line 3124 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 70:
#line 1169 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Composition(*(yyvsp[-3].RELATION),*(yyvsp[-1].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[-3].RELATION);
             delete (yyvsp[-1].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[-3].RELATION);
           delete (yyvsp[-1].RELATION);
         }
#line 3144 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 71:
#line 1184 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Composition(*(yyvsp[-2].RELATION),*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[-2].RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[-2].RELATION);
           delete (yyvsp[0].RELATION);
         }
#line 3164 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 72:
#line 1199 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = After(*(yyvsp[-2].RELATION),(yyvsp[0].INT_VALUE),(yyvsp[0].INT_VALUE));
             (*(yyval.RELATION)).prefix_print(stdout);
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[-2].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[-2].RELATION);
         }
#line 3183 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 73:
#line 1213 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Composition(*(yyvsp[0].RELATION),*(yyvsp[-2].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[-2].RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[-2].RELATION);
           delete (yyvsp[0].RELATION);
         }
#line 3203 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 74:
#line 1228 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Restrict_Range(*(yyvsp[-2].RELATION),*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[-2].RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[-2].RELATION);
           delete (yyvsp[0].RELATION);
         }
#line 3223 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 75:
#line 1243 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Restrict_Domain(*(yyvsp[-2].RELATION),*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[-2].RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[-2].RELATION);
           delete (yyvsp[0].RELATION);
         }
#line 3243 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 76:
#line 1258 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Intersection(*(yyvsp[-2].RELATION),*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[-2].RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[-2].RELATION);
           delete (yyvsp[0].RELATION);
         }
#line 3263 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 77:
#line 1273 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Difference(*(yyvsp[-2].RELATION),*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[-2].RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[-2].RELATION);
           delete (yyvsp[0].RELATION);
         }
#line 3283 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 78:
#line 1288 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Union(*(yyvsp[-2].RELATION),*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[-2].RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[-2].RELATION);
           delete (yyvsp[0].RELATION);
         }
#line 3303 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 79:
#line 1303 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Cross_Product(*(yyvsp[-2].RELATION),*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[-2].RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[-2].RELATION);
           delete (yyvsp[0].RELATION);
         }
#line 3323 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 80:
#line 1318 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Union(*(yyvsp[0].RELATION), Relation::Unknown(*(yyvsp[0].RELATION)));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 3341 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 81:
#line 1331 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Intersection(*(yyvsp[0].RELATION), Relation::Unknown(*(yyvsp[0].RELATION)));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 3359 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 82:
#line 1344 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Upper_Bound(*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 3377 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 83:
#line 1357 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Lower_Bound(*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 3395 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 84:
#line 1370 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Sample_Solution(*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 3413 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 85:
#line 1383 "parser.yy" /* yacc.c:1646  */
    {
           (yyval.RELATION) = new Relation();
           try {
             *(yyval.RELATION) = Symbolic_Solution(*(yyvsp[0].RELATION));
           }
           catch (const std::exception &e) {
             delete (yyval.RELATION);
             delete (yyvsp[0].RELATION);
             yyerror(e.what());
             YYERROR;
           }
           delete (yyvsp[0].RELATION);
         }
#line 3431 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 86:
#line 1396 "parser.yy" /* yacc.c:1646  */
    { (yyval.RELATION) = (yyvsp[0].RELATION); }
#line 3437 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 87:
#line 1397 "parser.yy" /* yacc.c:1646  */
    {
           if (((yyvsp[0].RELATION))->is_satisfiable()) {
             fprintf(stderr,"assert_unsatisfiable failed on ");
             ((yyvsp[0].RELATION))->print_with_subs(stderr);
             exit(1);
           }
           (yyval.RELATION)=(yyvsp[0].RELATION);
         }
#line 3450 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 88:
#line 1407 "parser.yy" /* yacc.c:1646  */
    {currentTuple = Output_Tuple;}
#line 3456 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 89:
#line 1408 "parser.yy" /* yacc.c:1646  */
    {currentTuple = Input_Tuple;}
#line 3462 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 90:
#line 1408 "parser.yy" /* yacc.c:1646  */
    {
                 Relation * r = new Relation((yyvsp[-5].TUPLE_DESCRIPTOR)->size,(yyvsp[-2].TUPLE_DESCRIPTOR)->size);
                 resetGlobals();
                 F_And *f = r->add_and();
                 for(int i = 1; i <= (yyvsp[-5].TUPLE_DESCRIPTOR)->size; i++) {
                   (yyvsp[-5].TUPLE_DESCRIPTOR)->vars[i-1]->vid = r->input_var(i);
                   if (!(yyvsp[-5].TUPLE_DESCRIPTOR)->vars[i-1]->anonymous) 
                     r->name_input_var(i, (yyvsp[-5].TUPLE_DESCRIPTOR)->vars[i-1]->stripped_name);
                 }
                 for(int i = 1; i <= (yyvsp[-2].TUPLE_DESCRIPTOR)->size; i++) {
                   (yyvsp[-2].TUPLE_DESCRIPTOR)->vars[i-1]->vid = r->output_var(i);
                   if (!(yyvsp[-2].TUPLE_DESCRIPTOR)->vars[i-1]->anonymous) 
                     r->name_output_var(i, (yyvsp[-2].TUPLE_DESCRIPTOR)->vars[i-1]->stripped_name);
                 }
                 r->setup_names();
                 for (std::set<ast::Exp *>::iterator i = (yyvsp[-5].TUPLE_DESCRIPTOR)->eq_constraints.begin(); i != (yyvsp[-5].TUPLE_DESCRIPTOR)->eq_constraints.end(); i++)
                   install_eq(f, *i, 0);
                 for (std::set<ast::Exp *>::iterator i = (yyvsp[-5].TUPLE_DESCRIPTOR)->geq_constraints.begin(); i != (yyvsp[-5].TUPLE_DESCRIPTOR)->geq_constraints.end(); i++)
                   install_geq(f, *i, 0);
                 for (std::set<strideConstraint *>::iterator i = (yyvsp[-5].TUPLE_DESCRIPTOR)->stride_constraints.begin(); i != (yyvsp[-5].TUPLE_DESCRIPTOR)->stride_constraints.end(); i++)
                   install_stride(f, *i);
                 for (std::set<ast::Exp *>::iterator i = (yyvsp[-2].TUPLE_DESCRIPTOR)->eq_constraints.begin(); i != (yyvsp[-2].TUPLE_DESCRIPTOR)->eq_constraints.end(); i++)
                   install_eq(f, *i, 0);
                 for (std::set<ast::Exp *>::iterator i = (yyvsp[-2].TUPLE_DESCRIPTOR)->geq_constraints.begin(); i != (yyvsp[-2].TUPLE_DESCRIPTOR)->geq_constraints.end(); i++)
                   install_geq(f, *i, 0);
                 for (std::set<strideConstraint *>::iterator i = (yyvsp[-2].TUPLE_DESCRIPTOR)->stride_constraints.begin(); i != (yyvsp[-2].TUPLE_DESCRIPTOR)->stride_constraints.end(); i++)
                   install_stride(f, *i);
                 if ((yyvsp[0].ASTP)) (yyvsp[0].ASTP)->install(f);
                 delete (yyvsp[-5].TUPLE_DESCRIPTOR);
                 delete (yyvsp[-2].TUPLE_DESCRIPTOR);
                 delete (yyvsp[0].ASTP);
                 (yyval.RELATION) = r;
               }
#line 3500 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 91:
#line 1441 "parser.yy" /* yacc.c:1646  */
    {
                 Relation * r = new Relation((yyvsp[-1].TUPLE_DESCRIPTOR)->size);
                 resetGlobals();
                 F_And *f = r->add_and();
                 for(int i = 1; i <= (yyvsp[-1].TUPLE_DESCRIPTOR)->size; i++) {
                   (yyvsp[-1].TUPLE_DESCRIPTOR)->vars[i-1]->vid = r->set_var(i);
                   if (!(yyvsp[-1].TUPLE_DESCRIPTOR)->vars[i-1]->anonymous) 
                     r->name_set_var(i, (yyvsp[-1].TUPLE_DESCRIPTOR)->vars[i-1]->stripped_name);
                 }
                 r->setup_names();
                 for (std::set<ast::Exp *>::iterator i = (yyvsp[-1].TUPLE_DESCRIPTOR)->eq_constraints.begin(); i != (yyvsp[-1].TUPLE_DESCRIPTOR)->eq_constraints.end(); i++)
                   install_eq(f, *i, 0);
                 for (std::set<ast::Exp *>::iterator i = (yyvsp[-1].TUPLE_DESCRIPTOR)->geq_constraints.begin(); i != (yyvsp[-1].TUPLE_DESCRIPTOR)->geq_constraints.end(); i++)
                   install_geq(f, *i, 0);
                 for (std::set<strideConstraint *>::iterator i = (yyvsp[-1].TUPLE_DESCRIPTOR)->stride_constraints.begin(); i != (yyvsp[-1].TUPLE_DESCRIPTOR)->stride_constraints.end(); i++)
                   install_stride(f, *i);
                 if ((yyvsp[0].ASTP)) (yyvsp[0].ASTP)->install(f);
                 delete (yyvsp[-1].TUPLE_DESCRIPTOR);
                 delete (yyvsp[0].ASTP);
                 (yyval.RELATION) = r;
               }
#line 3526 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 92:
#line 1462 "parser.yy" /* yacc.c:1646  */
    {
                 Relation * r = new Relation(0,0);
                 F_And *f = r->add_and();
                 (yyvsp[0].ASTP)->install(f);
                 delete (yyvsp[0].ASTP);
                 (yyval.RELATION) = r;
               }
#line 3538 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 93:
#line 1471 "parser.yy" /* yacc.c:1646  */
    {(yyval.ASTP) = (yyvsp[0].ASTP);}
#line 3544 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 94:
#line 1472 "parser.yy" /* yacc.c:1646  */
    {(yyval.ASTP) = 0;}
#line 3550 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 98:
#line 1480 "parser.yy" /* yacc.c:1646  */
    {currentTupleDescriptor = new tupleDescriptor; tuplePos = 1;}
#line 3556 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 99:
#line 1482 "parser.yy" /* yacc.c:1646  */
    {(yyval.TUPLE_DESCRIPTOR) = currentTupleDescriptor; tuplePos = 0;}
#line 3562 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 103:
#line 1490 "parser.yy" /* yacc.c:1646  */
    {
           Declaration_Site *ds = defined((yyvsp[0].VAR_NAME));
           if (!ds)
             currentTupleDescriptor->extend((yyvsp[0].VAR_NAME),currentTuple,tuplePos);
           else {
             Variable_Ref *v = lookupScalar((yyvsp[0].VAR_NAME));
             if (v == NULL) {
               yyerror(std::string("cannot find declaration for variable ") + to_string((yyvsp[0].VAR_NAME)));
               delete [](yyvsp[0].VAR_NAME);
               YYERROR;
             }
             if (ds != globalDecls)
               currentTupleDescriptor->extend((yyvsp[0].VAR_NAME), new ast::Exp(v));
             else
               currentTupleDescriptor->extend(new ast::Exp(v));
           }
           tuplePos++;
           delete [](yyvsp[0].VAR_NAME);
         }
#line 3586 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 104:
#line 1509 "parser.yy" /* yacc.c:1646  */
    {currentTupleDescriptor->extend(); tuplePos++;}
#line 3592 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 105:
#line 1510 "parser.yy" /* yacc.c:1646  */
    {
             currentTupleDescriptor->extend((yyvsp[0].EXP));
             tuplePos++;
         }
#line 3601 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 106:
#line 1514 "parser.yy" /* yacc.c:1646  */
    {
             currentTupleDescriptor->extend((yyvsp[-2].EXP),(yyvsp[0].EXP));
             tuplePos++;
         }
#line 3610 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 107:
#line 1518 "parser.yy" /* yacc.c:1646  */
    {
             currentTupleDescriptor->extend((yyvsp[-4].EXP),(yyvsp[-2].EXP),(yyvsp[0].COEF_VALUE));
             tuplePos++;
         }
#line 3619 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 108:
#line 1524 "parser.yy" /* yacc.c:1646  */
    {(yyval.VAR_LIST) = (yyvsp[-2].VAR_LIST); (yyval.VAR_LIST)->insert((yyvsp[0].VAR_NAME)); (yyvsp[0].VAR_NAME) = NULL;}
#line 3625 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 109:
#line 1525 "parser.yy" /* yacc.c:1646  */
    {(yyval.VAR_LIST) = new std::set<char *>(); (yyval.VAR_LIST)->insert((yyvsp[0].VAR_NAME)); (yyvsp[0].VAR_NAME) = NULL;}
#line 3631 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 110:
#line 1528 "parser.yy" /* yacc.c:1646  */
    {
          (yyval.DECLARATION_SITE) = current_Declaration_Site = new Declaration_Site((yyvsp[0].VAR_LIST));
          for (std::set<char *>::iterator i = (yyvsp[0].VAR_LIST)->begin(); i != (yyvsp[0].VAR_LIST)->end(); i++)
            delete [](*i);
          delete (yyvsp[0].VAR_LIST);
        }
#line 3642 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 111:
#line 1536 "parser.yy" /* yacc.c:1646  */
    {(yyval.DECLARATION_SITE) = (yyvsp[0].DECLARATION_SITE);}
#line 3648 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 112:
#line 1537 "parser.yy" /* yacc.c:1646  */
    {(yyval.DECLARATION_SITE) = (yyvsp[-1].DECLARATION_SITE);}
#line 3654 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 115:
#line 1544 "parser.yy" /* yacc.c:1646  */
    {globalDecls->extend_both_tuples((yyvsp[-3].VAR_NAME), (yyvsp[-1].INT_VALUE)); delete [](yyvsp[-3].VAR_NAME);}
#line 3660 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 116:
#line 1545 "parser.yy" /* yacc.c:1646  */
    {
          globalDecls->extend((yyvsp[0].VAR_NAME));
          delete [](yyvsp[0].VAR_NAME);
        }
#line 3669 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 117:
#line 1551 "parser.yy" /* yacc.c:1646  */
    {(yyval.ASTP) = new AST_And((yyvsp[-2].ASTP),(yyvsp[0].ASTP));}
#line 3675 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 118:
#line 1552 "parser.yy" /* yacc.c:1646  */
    {(yyval.ASTP) = new AST_Or((yyvsp[-2].ASTP),(yyvsp[0].ASTP));}
#line 3681 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 119:
#line 1553 "parser.yy" /* yacc.c:1646  */
    {(yyval.ASTP) = (yyvsp[0].ASTCP);}
#line 3687 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 120:
#line 1554 "parser.yy" /* yacc.c:1646  */
    {(yyval.ASTP) = (yyvsp[-1].ASTP);}
#line 3693 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 121:
#line 1555 "parser.yy" /* yacc.c:1646  */
    {(yyval.ASTP) = new AST_Not((yyvsp[0].ASTP));}
#line 3699 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 122:
#line 1556 "parser.yy" /* yacc.c:1646  */
    {(yyval.ASTP) = new AST_exists((yyvsp[-3].DECLARATION_SITE),(yyvsp[-1].ASTP));}
#line 3705 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 123:
#line 1557 "parser.yy" /* yacc.c:1646  */
    {(yyval.ASTP) = new AST_forall((yyvsp[-3].DECLARATION_SITE),(yyvsp[-1].ASTP));}
#line 3711 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 132:
#line 1576 "parser.yy" /* yacc.c:1646  */
    {popScope();}
#line 3717 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 133:
#line 1579 "parser.yy" /* yacc.c:1646  */
    {(yyval.EXP_LIST) = (yyvsp[0].EXP_LIST); (yyval.EXP_LIST)->insert((yyvsp[-2].EXP));}
#line 3723 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 134:
#line 1580 "parser.yy" /* yacc.c:1646  */
    {(yyval.EXP_LIST) = new std::set<ast::Exp *>(); (yyval.EXP_LIST)->insert((yyvsp[0].EXP));}
#line 3729 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 135:
#line 1583 "parser.yy" /* yacc.c:1646  */
    {(yyval.ASTCP) = new AST_constraints((yyvsp[-2].EXP_LIST),(yyvsp[-1].REL_OPERATOR),(yyvsp[0].EXP_LIST));}
#line 3735 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 136:
#line 1584 "parser.yy" /* yacc.c:1646  */
    {(yyval.ASTCP) = new AST_constraints((yyvsp[-2].EXP_LIST),(yyvsp[-1].REL_OPERATOR),(yyvsp[0].ASTCP));}
#line 3741 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 137:
#line 1587 "parser.yy" /* yacc.c:1646  */
    {
            Variable_Ref *v = lookupScalar((yyvsp[0].VAR_NAME));
            if (v == NULL) {
              yyerror(std::string("cannot find declaration for variable ") + to_string((yyvsp[0].VAR_NAME)));
              delete [](yyvsp[0].VAR_NAME);
              YYERROR;
            }
            (yyval.EXP) = new ast::Exp(v);
            delete [](yyvsp[0].VAR_NAME);
          }
#line 3756 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 138:
#line 1597 "parser.yy" /* yacc.c:1646  */
    {argCount = 1;}
#line 3762 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 139:
#line 1597 "parser.yy" /* yacc.c:1646  */
    {
            Variable_Ref *v;
            if ((yyvsp[-1].ARGUMENT_TUPLE) == Input_Tuple)
              v = functionOfInput[(yyvsp[-4].VAR_NAME)];
            else
              v = functionOfOutput[(yyvsp[-4].VAR_NAME)];
            if (v == NULL) {
              yyerror(std::string("Function ") + to_string((yyvsp[-4].VAR_NAME)) + std::string(" not declared"));
              delete [](yyvsp[-4].VAR_NAME);
              YYERROR;
            }
            (yyval.EXP) = new ast::Exp(v);
            delete [](yyvsp[-4].VAR_NAME);
          }
#line 3781 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 140:
#line 1611 "parser.yy" /* yacc.c:1646  */
    { (yyval.EXP) = (yyvsp[-1].EXP); }
#line 3787 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 141:
#line 1614 "parser.yy" /* yacc.c:1646  */
    {
               Variable_Ref *v = lookupScalar((yyvsp[0].VAR_NAME));
               if (v == NULL) {
                 yyerror(std::string("cannot find declaration for variable ") + to_string((yyvsp[-2].ARGUMENT_TUPLE)));
                 delete [](yyvsp[0].VAR_NAME);
                 YYERROR;
               }
               if (v->pos != argCount || v->of != (yyvsp[-2].ARGUMENT_TUPLE) || (v->of != Input_Tuple && v->of != Output_Tuple)) {
                 yyerror("arguments to function must be prefix of input or output tuple");
                 delete [](yyvsp[0].VAR_NAME);
                 YYERROR;
               }
               (yyval.ARGUMENT_TUPLE) = v->of;
               argCount++;
               delete [](yyvsp[0].VAR_NAME);
             }
#line 3808 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 142:
#line 1630 "parser.yy" /* yacc.c:1646  */
    {
               Variable_Ref *v = lookupScalar((yyvsp[0].VAR_NAME));
               if (v == NULL) {
                 yyerror(std::string("cannot find declaration for variable ") + to_string((yyvsp[0].VAR_NAME)));
                 delete [](yyvsp[0].VAR_NAME);
                 YYERROR;
               }
               if (v->pos != argCount || (v->of != Input_Tuple && v->of != Output_Tuple)) {
                 yyerror("arguments to function must be prefix of input or output tuple");
                 delete [](yyvsp[0].VAR_NAME);
                 YYERROR;
               }
               (yyval.ARGUMENT_TUPLE) = v->of;
               argCount++;
               delete [](yyvsp[0].VAR_NAME);
             }
#line 3829 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 143:
#line 1648 "parser.yy" /* yacc.c:1646  */
    {(yyval.EXP) = new ast::Exp((yyvsp[0].COEF_VALUE));}
#line 3835 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 144:
#line 1649 "parser.yy" /* yacc.c:1646  */
    {(yyval.EXP) = multiply((yyvsp[-1].COEF_VALUE),(yyvsp[0].EXP));}
#line 3841 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 145:
#line 1650 "parser.yy" /* yacc.c:1646  */
    {(yyval.EXP) = (yyvsp[0].EXP); }
#line 3847 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 146:
#line 1651 "parser.yy" /* yacc.c:1646  */
    {(yyval.EXP) = negate((yyvsp[0].EXP));}
#line 3853 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 147:
#line 1652 "parser.yy" /* yacc.c:1646  */
    {(yyval.EXP) = add((yyvsp[-2].EXP),(yyvsp[0].EXP));}
#line 3859 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 148:
#line 1653 "parser.yy" /* yacc.c:1646  */
    {(yyval.EXP) = subtract((yyvsp[-2].EXP),(yyvsp[0].EXP));}
#line 3865 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 149:
#line 1654 "parser.yy" /* yacc.c:1646  */
    {
      try {
        (yyval.EXP) = multiply((yyvsp[-2].EXP),(yyvsp[0].EXP));
      }
      catch (const std::exception &e) {
        yyerror(e.what());
        YYERROR;
      }
    }
#line 3879 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 150:
#line 1666 "parser.yy" /* yacc.c:1646  */
    {
            Dynamic_Array1<Relation> *final = Reachable_Nodes(reachable_info);
            (yyval.RELATION_ARRAY_1D) = final;
          }
#line 3888 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 151:
#line 1672 "parser.yy" /* yacc.c:1646  */
    {
               Dynamic_Array1<Relation> *final = Reachable_Nodes(reachable_info);
               int index = reachable_info->node_names.index(std::string((yyvsp[-3].VAR_NAME)));
               if (index == 0) {
                 yyerror(std::string("no such node ") + to_string((yyvsp[-3].VAR_NAME)));
                 delete [](yyvsp[-3].VAR_NAME);
                 delete final;
                 delete reachable_info;
                 YYERROR;
               }
               (yyval.RELATION) = new Relation; 
               *(yyval.RELATION) = (*final)[index];
               delete final;
               delete reachable_info;
               delete [](yyvsp[-3].VAR_NAME);
             }
#line 3909 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 152:
#line 1690 "parser.yy" /* yacc.c:1646  */
    {
               int sz = reachable_info->node_names.size();
               reachable_info->node_arity.reallocate(sz);
               reachable_info->transitions.resize(sz+1,sz+1);
               reachable_info->start_nodes.resize(sz+1);
             }
#line 3920 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 153:
#line 1698 "parser.yy" /* yacc.c:1646  */
    {
                   reachable_info->node_names.append(std::string((yyvsp[0].VAR_NAME)));
                   delete [](yyvsp[0].VAR_NAME);
                 }
#line 3929 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 154:
#line 1702 "parser.yy" /* yacc.c:1646  */
    {
                   reachable_info = new reachable_information;
                   reachable_info->node_names.append(std::string((yyvsp[0].VAR_NAME)));
                   delete [](yyvsp[0].VAR_NAME);
                 }
#line 3939 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 155:
#line 1710 "parser.yy" /* yacc.c:1646  */
    {  
                        int i,j;
                        int n_nodes = reachable_info->node_names.size();
                        Tuple<int> &arity = reachable_info->node_arity;
                        Dynamic_Array2<Relation> &transitions = reachable_info->transitions;

                        /* fixup unspecified transitions to be false */
                        /* find arity */
                        for(i = 1; i <= n_nodes; i++) arity[i] = -1;
                        for(i = 1; i <= n_nodes; i++)
                          for(j = 1; j <= n_nodes; j++)
                            if(! transitions[i][j].is_null()) {
                              int in_arity = transitions[i][j].n_inp();
                              int out_arity = transitions[i][j].n_out();
                              if(arity[i] < 0) arity[i] = in_arity;
                              if(arity[j] < 0) arity[j] = out_arity;
                              if(in_arity != arity[i] || out_arity != arity[j]) {
                                yyerror(std::string("arity mismatch in node transition: ") + to_string(reachable_info->node_names[i]) + std::string(" -> ") + to_string(reachable_info->node_names[j]));
                                delete reachable_info;
                                YYERROR;
                              }
                            }
                        for(i = 1; i <= n_nodes; i++) 
                          if(arity[i] < 0) arity[i] = 0;
                        /* Fill in false relations */
                        for(i = 1; i <= n_nodes; i++)
                          for(j = 1; j <= n_nodes; j++)
                            if(transitions[i][j].is_null())
                              transitions[i][j] = Relation::False(arity[i],arity[j]);

                        /* fixup unused start node positions */
                        Dynamic_Array1<Relation> &nodes = reachable_info->start_nodes;
                        for(i = 1; i <= n_nodes; i++) 
                          if(nodes[i].is_null()) 
                            nodes[i] = Relation::False(arity[i]);
                          else
                            if(nodes[i].n_set() != arity[i]){
                              yyerror(std::string("arity mismatch in start node ") + to_string(reachable_info->node_names[i]));
                              delete reachable_info;
                              YYERROR;
                            }
                   }
#line 3986 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 156:
#line 1754 "parser.yy" /* yacc.c:1646  */
    {
                            int n_nodes = reachable_info->node_names.size();
                            int index = reachable_info->node_names.index((yyvsp[-2].VAR_NAME));
                            if (!(index > 0 && index <= n_nodes)) {
                              yyerror(std::string("no such node ")+to_string((yyvsp[-2].VAR_NAME)));
                              delete (yyvsp[0].RELATION);
                              delete [](yyvsp[-2].VAR_NAME);
                              delete reachable_info;
                              YYERROR;
                            }
                            reachable_info->start_nodes[index] = *(yyvsp[0].RELATION);
                            delete (yyvsp[0].RELATION);
                            delete [](yyvsp[-2].VAR_NAME);
                          }
#line 4005 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 157:
#line 1768 "parser.yy" /* yacc.c:1646  */
    {
                            int n_nodes = reachable_info->node_names.size();
                            int from_index = reachable_info->node_names.index((yyvsp[-4].VAR_NAME));
                            if (!(from_index > 0 && from_index <= n_nodes)) {
                              yyerror(std::string("no such node ")+to_string((yyvsp[-4].VAR_NAME)));
                              delete (yyvsp[0].RELATION);
                              delete [](yyvsp[-4].VAR_NAME);
                              delete [](yyvsp[-2].VAR_NAME);
                              delete reachable_info;
                              YYERROR;
                            }
                            int to_index = reachable_info->node_names.index((yyvsp[-2].VAR_NAME));
                            if (!(to_index > 0 && to_index <= n_nodes)) {
                              yyerror(std::string("no such node ")+to_string((yyvsp[-2].VAR_NAME)));
                              delete (yyvsp[0].RELATION);
                              delete [](yyvsp[-4].VAR_NAME);
                              delete [](yyvsp[-2].VAR_NAME);
                              delete reachable_info;
                              YYERROR;
                            }
                            reachable_info->transitions[from_index][to_index] = *(yyvsp[0].RELATION);
                            delete (yyvsp[0].RELATION);
                            delete [](yyvsp[-4].VAR_NAME);
                            delete [](yyvsp[-2].VAR_NAME);
                          }
#line 4035 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 158:
#line 1793 "parser.yy" /* yacc.c:1646  */
    {
                            int n_nodes = reachable_info->node_names.size();
                            int from_index = reachable_info->node_names.index((yyvsp[-4].VAR_NAME));
                            if (!(from_index > 0 && from_index <= n_nodes)) {
                              yyerror(std::string("no such node ")+to_string((yyvsp[-4].VAR_NAME)));
                              delete (yyvsp[0].RELATION);
                              delete [](yyvsp[-4].VAR_NAME);
                              delete [](yyvsp[-2].VAR_NAME);
                              delete reachable_info;
                              YYERROR;
                            }
                            int to_index = reachable_info->node_names.index((yyvsp[-2].VAR_NAME));
                            if (!(to_index > 0 && to_index <= n_nodes)) {
                              yyerror(std::string("no such node ")+to_string((yyvsp[-2].VAR_NAME)));
                              delete (yyvsp[0].RELATION);
                              delete [](yyvsp[-4].VAR_NAME);
                              delete [](yyvsp[-2].VAR_NAME);
                              delete reachable_info;
                              YYERROR;
                            }
                            reachable_info->transitions[from_index][to_index] = *(yyvsp[0].RELATION);
                            delete (yyvsp[0].RELATION);
                            delete [](yyvsp[-4].VAR_NAME);
                            delete [](yyvsp[-2].VAR_NAME);
                          }
#line 4065 "parser.tab.cc" /* yacc.c:1646  */
    break;

  case 159:
#line 1818 "parser.yy" /* yacc.c:1646  */
    {
                            int n_nodes = reachable_info->node_names.size();
                            int index = reachable_info->node_names.index((yyvsp[-2].VAR_NAME));
                            if (!(index > 0 && index <= n_nodes)) {
                              yyerror(std::string("no such node ")+to_string((yyvsp[-2].VAR_NAME)));
                              delete (yyvsp[0].RELATION);
                              delete [](yyvsp[-2].VAR_NAME);
                              delete reachable_info;
                              YYERROR;
                            }
                            reachable_info->start_nodes[index] = *(yyvsp[0].RELATION);
                            delete (yyvsp[0].RELATION);
                            delete [](yyvsp[-2].VAR_NAME);
                          }
#line 4084 "parser.tab.cc" /* yacc.c:1646  */
    break;


#line 4088 "parser.tab.cc" /* yacc.c:1646  */
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

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

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
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

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
  return yyresult;
}
#line 1834 "parser.yy" /* yacc.c:1906  */


void yyerror(const std::string &s) {
  std::stringstream ss;
  if (is_interactive)
    ss << s << "\n";
  else
    ss << s << " at line " << mylexer.lineno() << "\n";
  err_msg = ss.str();
}

int omega_run(std::istream *is, std::ostream *os) {
    yy_buffer_state *bs = mylexer.yy_create_buffer(is, 8092);
    mylexer.yypush_buffer_state(bs);

    // Backup stream buffer of cout and redirect it to output stream
    auto cout_buff = std::cout.rdbuf();
    std::cout.rdbuf(os->rdbuf());

    //yydebug = 1;
    is_interactive = false;
    need_coef = false;
    current_Declaration_Site = globalDecls = new Global_Declaration_Site();

    if (yyparse() != 0) {
        std::cout << "syntax error at the end of the file, missing ';'" << std::endl;
        delete relationDecl;
        relationDecl = NULL;
    }

    // Redirect cout back to screen
    std::cout.rdbuf(cout_buff);

    for (auto i = relationMap.begin(); i != relationMap.end(); i++) {
        //std::cerr << "Deleting relation '" << (*i).first << std::endl;
        delete (*i).second;
    }

    relationMap.clear();
    delete globalDecls;
    current_Declaration_Site = globalDecls = NULL;

    return 0;
}

#ifndef SKIP_MAIN
int main(int argc, char **argv) {
  if (argc > 2){
    fprintf(stderr, "Usage: %s [script_file]\n", argv[0]);
    exit(1);
  }

  if (argc == 2) {
    std::ifstream *ifs = new std::ifstream;
    ifs->open(argv[1], std::ifstream::in);
    if (!ifs->is_open()) {
        fprintf(stderr, "can't open input file %s\n", argv[1]);
        exit(1);
    }
    yy_buffer_state *bs = mylexer.yy_create_buffer(ifs, 8092);
    mylexer.yypush_buffer_state(bs);
  }

  //yydebug = 1;
  is_interactive = false;
  if (argc == 1) {
#if defined __USE_POSIX  
    if (isatty((int)fileno(stdin)))
      is_interactive = true;
#elif defined  __WIN32
    if (_isatty(_fileno(stdin)))
      is_interactive = true;
#endif
  }

  if (is_interactive) {
#ifdef BUILD_CODEGEN
    std::cout << "Omega+ and CodeGen+ ";
#else
    std::cout << "Omega+ ";
#endif
    std::cout << "v2.2.3 (built on " << OC_BUILD_DATE << ")" << std::endl;
    std::cout << "Copyright (C) 1994-2000 the Omega Project Team" << std::endl;
    std::cout << "Copyright (C) 2005-2011 Chun Chen" << std::endl;
    std::cout << "Copyright (C) 2011-2012 University of Utah" << std::endl;
    std::cout << PROMPT_STRING << ' ';
    std::cout.flush();
  }

  need_coef = false;  
  current_Declaration_Site = globalDecls = new Global_Declaration_Site();

  if (yyparse() != 0) {
    if (!is_interactive)
      std::cout << "syntax error at the end of the file, missing ';'" << std::endl;
    else
      std::cout << std::endl;
    delete relationDecl;
    relationDecl = NULL;
  }
  else {
    if (is_interactive)
      std::cout << std::endl;
  }

  for (std::map<std::string, Relation *>::iterator i = relationMap.begin(); i != relationMap.end(); i++)
    delete (*i).second;
  delete globalDecls;  
  
  return 0;
}
#endif

Relation LexForward(int n) {
  Relation r(n,n);
  F_Or *f = r.add_or();
  for (int i=1; i <= n; i++) {
    F_And *g = f->add_and();
    for(int j=1;j<i;j++) {
      EQ_Handle e = g->add_EQ();
      e.update_coef(r.input_var(j),-1);
      e.update_coef(r.output_var(j),1);
      e.finalize();
    }
    GEQ_Handle e = g->add_GEQ();
    e.update_coef(r.input_var(i),-1);
    e.update_coef(r.output_var(i),1);
    e.update_const(-1);
    e.finalize();
  }
  r.finalize();
  return r;
}
