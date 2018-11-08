/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 18 "parser/parser.y" /* yacc.c:339  */

   extern int yylex ();
   #include <parser/parser.h>
   #include <iostream>
   #include <list>
   #include <stack>
   #define YYERROR_VERBOSE
   #define YYDEBUG 1

   //Required for yyerror below
   void yyerror(const char*);
   extern char *yytext;
   extern int yylineno;

   using namespace iegenlib;


#line 84 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:339  */

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
   by #include "gen_parser.hh".  */
#ifndef YY_YY_HOME_KING_TOOLS_BRANCH_ISLUPDATE_SRC_PARSER_GEN_PARSER_HH_INCLUDED
# define YY_YY_HOME_KING_TOOLS_BRANCH_ISLUPDATE_SRC_PARSER_GEN_PARSER_HH_INCLUDED
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
    LBRACE = 258,
    RBRACE = 259,
    LBRACKET = 260,
    RBRACKET = 261,
    LPAREN = 262,
    RPAREN = 263,
    COMMA = 264,
    COLON = 265,
    LT = 266,
    LTE = 267,
    GT = 268,
    GTE = 269,
    SEMI = 270,
    OR = 271,
    UNION = 272,
    INVERSE = 273,
    EXISTS = 274,
    EQ = 275,
    ARROW = 276,
    ID = 277,
    INT = 278,
    INVALID_ID = 279,
    PLUS = 280,
    DASH = 281,
    STAR = 282,
    UMINUS = 283,
    AND = 284,
    WAND = 285
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 36 "parser/parser.y" /* yacc.c:355  */

   std::string*                           sval;
   int                                    ival;
   iegenlib::TupleDecl*                   tdecl;
   iegenlib::Environment*                 env;
   iegenlib::Set*                         set;
   iegenlib::Relation*                    relation;
   iegenlib::Conjunction*                 conj;
   std::list<iegenlib::Conjunction*>*     conjlist;
   iegenlib::Exp*                         exp;
   std::list<iegenlib::Exp*>*             explist;
   std::list<std::string>*                symlist;
   std::list<std::string>*                existslist;

#line 170 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_HOME_KING_TOOLS_BRANCH_ISLUPDATE_SRC_PARSER_GEN_PARSER_HH_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 185 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:358  */

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
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   139

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  31
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  44
/* YYNRULES -- Number of rules.  */
#define YYNRULES  89
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  156

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   285

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
      25,    26,    27,    28,    29,    30
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   149,   149,   152,   155,   158,   161,   164,   168,   176,
     189,   206,   221,   238,   246,   252,   272,   280,   286,   309,
     315,   323,   344,   350,   358,   380,   388,   392,   394,   402,
     409,   415,   419,   422,   430,   434,   441,   447,   453,   458,
     461,   471,   475,   478,   485,   491,   497,   504,   510,   516,
     519,   522,   525,   528,   531,   539,   548,   556,   565,   573,
     576,   579,   582,   585,   604,   623,   641,   661,   664,   667,
     670,   673,   676,   679,   700,   707,   713,   719,   726,   732,
     738,   744,   747,   750,   753,   768,   782,   796,   806,   812
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "LBRACE", "RBRACE", "LBRACKET",
  "RBRACKET", "LPAREN", "RPAREN", "COMMA", "COLON", "LT", "LTE", "GT",
  "GTE", "SEMI", "OR", "UNION", "INVERSE", "EXISTS", "EQ", "ARROW", "ID",
  "INT", "INVALID_ID", "PLUS", "DASH", "STAR", "UMINUS", "AND", "WAND",
  "$accept", "Start", "environment", "omega_set", "isl_set",
  "omega_relation", "isl_relation", "conjunct_list", "single_conjunct",
  "rel_conjunct_list", "rel_single_conjunct", "semi_conjunct_list",
  "semi_conjunct", "semi_rel_conjunct_list", "semi_rel_conjunct",
  "in_symbolic", "symbol_list", "tuple_decl_with_brackets", "tuple_decl",
  "tuple_elem", "variable_id", "optional_constraints", "constraint_list",
  "constraint", "constraint_eq", "constraint_lt", "constraint_lte",
  "constraint_gt", "constraint_gte", "constraint_range",
  "constraint_range_lte_lt", "constraint_range_lt_lte",
  "constraint_range_lte_lte", "constraint_range_lt_lt", "expression",
  "expression_tuple", "expression_int", "expression_unop",
  "expression_binop", "expression_int_mult", "expression_simple",
  "expression_id", "expression_func", "expression_list", YY_NULLPTR
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
     285
};
# endif

#define YYPACT_NINF -57

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-57)))

#define YYTABLE_NINF -84

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
       9,   -57,    -9,    10,    19,   -57,   -57,   -57,   -57,   -57,
      24,    12,    23,    33,   -57,    38,    30,   -57,    55,   -57,
      -9,    77,    83,    26,    89,    90,    -1,   103,   105,   -57,
     -57,    93,   -57,   -57,   -57,    69,   -57,   -57,    38,    38,
      35,    92,   106,    38,   -57,    38,   -57,    87,   -57,   -57,
      -7,   108,    98,   104,   111,   101,    96,    35,   112,    -4,
      35,   113,     7,   -57,   -57,   -57,   -57,   -57,   -57,   -57,
     -57,   -57,   -57,   -57,    51,    94,   -57,   -57,   -57,   -57,
      95,   -57,   -57,   -57,   104,   -57,   104,    97,   116,   -57,
     -57,    38,   -57,   -57,    38,    92,    13,    75,    18,   -57,
     -57,   -57,    35,   -57,    35,    35,    35,    35,    35,    35,
      35,    35,    35,   102,   120,   106,    38,   118,   -57,   -57,
     104,   -57,    35,   -57,    91,   -57,   -57,   -57,   -57,    48,
      56,    13,    13,    13,   -57,   -57,   -57,   -57,   104,   -57,
     -57,    13,   -57,   122,    35,    35,    35,    35,   120,   107,
      13,    13,    13,    13,   123,   -57
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     7,     0,     0,     0,     2,     3,     5,     4,     6,
       0,    28,     0,     0,     1,    31,     9,    13,    11,    16,
       0,     0,     0,     0,     0,     0,    42,     0,     0,    29,
      26,     0,    30,    38,    37,     0,    34,    36,    31,    31,
      41,     0,    21,    31,    14,    31,    17,     0,    33,    32,
       0,     0,    20,    42,     0,    23,     0,     0,    84,    74,
       0,     0,     0,    43,    49,    52,    53,    50,    51,    46,
      59,    60,    61,    62,     0,    72,    67,    69,    70,    71,
      68,    81,    82,    25,    42,    15,    42,     0,     0,    35,
      10,    31,    21,    12,    31,     0,    89,     0,     0,    83,
      78,    75,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    24,     0,    31,     0,    19,    22,
      42,    73,     0,    79,     0,    44,    47,    45,    48,    55,
      56,    57,    58,    54,    76,    77,    80,    18,    42,     8,
      24,    88,    87,    85,     0,     0,     0,     0,     0,     0,
      66,    64,    63,    65,     0,    86
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -57,   -57,   -57,   -57,   -57,   -57,   -57,   -57,   109,   -57,
     100,    40,   117,    39,   119,   -57,   115,   -15,   -57,    88,
     -16,   -51,   -57,   -19,   -57,   -57,   -57,   -57,   -57,   -10,
     -57,   -57,   -57,   -57,   -56,   -54,   -57,   -57,   -57,   -57,
     -53,   -57,   -57,    37
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     4,     5,     6,     7,     8,     9,    16,    17,    18,
      19,    51,    52,    54,    55,    10,    12,    53,    35,    36,
      61,    42,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    97
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      26,    96,    92,    57,   101,    99,   100,    37,   103,    40,
       1,   -39,   -27,    11,     2,    33,    34,    13,    58,    14,
      41,    20,   -39,    98,    56,    57,    84,    15,    86,    21,
      87,     3,    32,   114,    37,   115,   104,   105,   111,   112,
      58,    22,    57,    23,    99,   123,    96,    27,    33,    34,
     129,   130,   131,   132,   133,   134,   135,    58,    59,   144,
     145,    60,   106,   107,   108,   109,   141,   146,   147,   140,
      48,   110,    28,   111,   112,    49,   111,   112,    50,    56,
     120,   111,   112,   121,   122,   125,   127,   148,   150,   151,
     152,   153,   142,    83,   126,   128,   -31,    23,    30,   143,
     122,   138,   -31,    31,    38,    39,    43,   -31,    45,    88,
      85,    47,    90,    91,    40,    93,    94,    95,   116,   -38,
     102,   -83,   113,   117,   137,   136,   139,   149,    46,   155,
     154,   118,    24,   119,    25,    29,    44,     0,    89,   124
};

static const yytype_int16 yycheck[] =
{
      15,    57,    53,     7,    60,    59,    59,    23,     1,    10,
       1,     4,     3,    22,     5,    22,    23,     7,    22,     0,
      21,     9,    15,    27,    39,     7,    41,     3,    43,     6,
      45,    22,     6,    84,    50,    86,    29,    30,    25,    26,
      22,     8,     7,     5,    98,    98,   102,    17,    22,    23,
     106,   107,   108,   109,   110,   111,   112,    22,    23,    11,
      12,    26,    11,    12,    13,    14,   122,    11,    12,   120,
       1,    20,    17,    25,    26,     6,    25,    26,     9,    94,
      95,    25,    26,     8,     9,   104,   105,   138,   144,   145,
     146,   147,     1,     1,   104,   105,     4,     5,    21,     8,
       9,   116,    10,    20,    15,    15,     3,    15,     3,    22,
       4,    18,     4,    15,    10,     4,    15,    21,    21,     7,
       7,    27,    27,     7,     4,    23,     8,     5,    28,     6,
      23,    91,    15,    94,    15,    20,    27,    -1,    50,   102
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     5,    22,    32,    33,    34,    35,    36,    37,
      46,    22,    47,     7,     0,     3,    38,    39,    40,    41,
       9,     6,     8,     5,    43,    45,    48,    17,    17,    47,
      21,    20,     6,    22,    23,    49,    50,    51,    15,    15,
      10,    21,    52,     3,    39,     3,    41,    18,     1,     6,
       9,    42,    43,    48,    44,    45,    48,     7,    22,    23,
      26,    51,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,     1,    48,     4,    48,    48,    22,    50,
       4,    15,    52,     4,    15,    21,    65,    74,    27,    66,
      71,    65,     7,     1,    29,    30,    11,    12,    13,    14,
      20,    25,    26,    27,    52,    52,    21,     7,    42,    44,
      48,     8,     9,    71,    74,    54,    60,    54,    60,    65,
      65,    65,    65,    65,    65,    65,    23,     4,    48,     8,
      52,    65,     1,     8,    11,    12,    11,    12,    52,     5,
      65,    65,    65,    65,    23,     6
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    31,    32,    32,    32,    32,    32,    32,    33,    34,
      35,    36,    37,    38,    38,    39,    40,    40,    41,    42,
      42,    43,    44,    44,    45,    45,    46,    46,    47,    47,
      48,    48,    48,    48,    49,    49,    50,    50,    51,    52,
      52,    52,    52,    53,    53,    53,    53,    53,    53,    54,
      54,    54,    54,    54,    55,    56,    57,    58,    59,    60,
      60,    60,    60,    61,    62,    63,    64,    65,    65,    65,
      65,    65,    65,    66,    67,    68,    69,    69,    70,    70,
      70,    71,    71,    71,    72,    73,    73,    73,    74,    74
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     1,     1,     8,     2,
       6,     2,     6,     1,     3,     4,     1,     3,     6,     3,
       1,     2,     3,     1,     4,     3,     4,     0,     1,     3,
       2,     0,     3,     3,     1,     3,     1,     1,     1,     2,
       3,     1,     0,     1,     3,     3,     1,     3,     3,     1,
       1,     1,     1,     1,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     5,     5,     5,     5,     1,     1,     1,
       1,     1,     1,     3,     1,     2,     3,     3,     2,     3,
       3,     1,     1,     1,     1,     4,     7,     4,     3,     1
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
          case 22: /* ID  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).sval));}
#line 1121 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 23: /* INT  */
#line 138 "parser/parser.y" /* yacc.c:1257  */
      {}
#line 1127 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 24: /* INVALID_ID  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).sval));}
#line 1133 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 33: /* environment  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).env));}
#line 1139 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 34: /* omega_set  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).set));}
#line 1145 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 35: /* isl_set  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).set));}
#line 1151 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 36: /* omega_relation  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).relation));}
#line 1157 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 37: /* isl_relation  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).relation));}
#line 1163 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 38: /* conjunct_list  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).conjlist));}
#line 1169 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 39: /* single_conjunct  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).conj));}
#line 1175 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 40: /* rel_conjunct_list  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).conjlist));}
#line 1181 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 41: /* rel_single_conjunct  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).conj));}
#line 1187 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 42: /* semi_conjunct_list  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).conjlist));}
#line 1193 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 43: /* semi_conjunct  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).conj));}
#line 1199 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 44: /* semi_rel_conjunct_list  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).conjlist));}
#line 1205 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 45: /* semi_rel_conjunct  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).conj));}
#line 1211 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 46: /* in_symbolic  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).symlist));}
#line 1217 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 47: /* symbol_list  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).symlist));}
#line 1223 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 48: /* tuple_decl_with_brackets  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).tdecl));}
#line 1229 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 49: /* tuple_decl  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).tdecl));}
#line 1235 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 50: /* tuple_elem  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).tdecl));}
#line 1241 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 51: /* variable_id  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).sval));}
#line 1247 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 52: /* optional_constraints  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).explist));}
#line 1253 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 53: /* constraint_list  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).explist));}
#line 1259 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 54: /* constraint  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).exp));}
#line 1265 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 55: /* constraint_eq  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).exp));}
#line 1271 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 56: /* constraint_lt  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).exp));}
#line 1277 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 57: /* constraint_lte  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).exp));}
#line 1283 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 58: /* constraint_gt  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).exp));}
#line 1289 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 59: /* constraint_gte  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).exp));}
#line 1295 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 60: /* constraint_range  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).explist));}
#line 1301 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 61: /* constraint_range_lte_lt  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).explist));}
#line 1307 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 62: /* constraint_range_lt_lte  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).explist));}
#line 1313 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 63: /* constraint_range_lte_lte  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).explist));}
#line 1319 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 64: /* constraint_range_lt_lt  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).explist));}
#line 1325 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 65: /* expression  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).exp));}
#line 1331 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 66: /* expression_tuple  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).exp));}
#line 1337 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 67: /* expression_int  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).exp));}
#line 1343 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 68: /* expression_unop  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).exp));}
#line 1349 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 69: /* expression_binop  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).exp));}
#line 1355 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 70: /* expression_int_mult  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).exp));}
#line 1361 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 71: /* expression_simple  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).exp));}
#line 1367 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 72: /* expression_id  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).exp));}
#line 1373 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 73: /* expression_func  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).exp));}
#line 1379 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
        break;

    case 74: /* expression_list  */
#line 139 "parser/parser.y" /* yacc.c:1257  */
      {delete (((*yyvaluep).explist));}
#line 1385 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1257  */
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
        case 2:
#line 150 "parser/parser.y" /* yacc.c:1646  */
    {parser::set_parse_env_result((yyvsp[0].env));}
#line 1649 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 3:
#line 153 "parser/parser.y" /* yacc.c:1646  */
    {parser::set_parse_set_result((yyvsp[0].set));}
#line 1655 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 4:
#line 156 "parser/parser.y" /* yacc.c:1646  */
    {parser::set_parse_relation_result((yyvsp[0].relation)); }
#line 1661 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 5:
#line 159 "parser/parser.y" /* yacc.c:1646  */
    {parser::set_parse_set_result((yyvsp[0].set));}
#line 1667 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 6:
#line 162 "parser/parser.y" /* yacc.c:1646  */
    {parser::set_parse_relation_result((yyvsp[0].relation));}
#line 1673 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 7:
#line 165 "parser/parser.y" /* yacc.c:1646  */
    {yyclearin;}
#line 1679 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 8:
#line 169 "parser/parser.y" /* yacc.c:1646  */
    { Environment* env = new Environment();
           env->setInverse(*(yyvsp[-7].sval),*(yyvsp[-2].sval));
           delete (yyvsp[-7].sval);
           delete (yyvsp[-2].sval);
           (yyval.env) = env;
         }
#line 1690 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 9:
#line 177 "parser/parser.y" /* yacc.c:1646  */
    {
            std::list<Conjunction*>::iterator it=(yyvsp[0].conjlist)->begin();
            std::list<Conjunction*>::iterator end=(yyvsp[0].conjlist)->end();
            Set* set = new Set((*it)->arity());
            for (; it!=end; ++it) {
               set->addConjunction(*it);
            }
            (yyval.set) = set;
            delete (yyvsp[0].conjlist);
            delete (yyvsp[-1].symlist);
         }
#line 1706 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 10:
#line 190 "parser/parser.y" /* yacc.c:1646  */
    {
            Set* set = new Set((yyvsp[-3].conj)->arity());
            set->addConjunction((yyvsp[-3].conj));

            std::list<Conjunction*>::iterator it=(yyvsp[-1].conjlist)->begin();
            std::list<Conjunction*>::iterator end=(yyvsp[-1].conjlist)->end();
            for (; it!=end; ++it) {
               set->addConjunction(*it);
            }
            (yyval.set) = set;
            delete (yyvsp[-1].conjlist);
            delete (yyvsp[-5].symlist);
         }
#line 1724 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 11:
#line 207 "parser/parser.y" /* yacc.c:1646  */
    {
            std::list<Conjunction*>::iterator it=(yyvsp[0].conjlist)->begin();
            std::list<Conjunction*>::iterator end=(yyvsp[0].conjlist)->end();
            int inarity = (*it)->inarity();
            int outarity = (*it)->arity()-inarity;
            Relation* relation = new Relation(inarity,outarity);
            for (; it!=end; ++it) {
               relation->addConjunction(*it);
            }
            (yyval.relation) = relation;
            delete (yyvsp[0].conjlist);
            delete (yyvsp[-1].symlist);
         }
#line 1742 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 12:
#line 222 "parser/parser.y" /* yacc.c:1646  */
    {
            int inarity = (yyvsp[-3].conj)->inarity();
            int outarity = (yyvsp[-3].conj)->arity()-inarity;
            Relation* relation = new Relation(inarity,outarity);
            relation->addConjunction((yyvsp[-3].conj));

            std::list<Conjunction*>::iterator it=(yyvsp[-1].conjlist)->begin();
            std::list<Conjunction*>::iterator end=(yyvsp[-1].conjlist)->end();
            for (; it!=end; ++it) {
               relation->addConjunction(*it);
            }
            (yyval.relation) = relation;
            delete (yyvsp[-1].conjlist);
            delete (yyvsp[-5].symlist);
         }
#line 1762 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 13:
#line 239 "parser/parser.y" /* yacc.c:1646  */
    {
            std::list<Conjunction*>* conlist=
               new std::list<Conjunction*>();
            conlist->push_back((yyvsp[0].conj));
            (yyval.conjlist) = conlist;
         }
#line 1773 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 14:
#line 247 "parser/parser.y" /* yacc.c:1646  */
    {
            (yyvsp[-2].conjlist)->push_back((yyvsp[0].conj));
            (yyval.conjlist)=(yyvsp[-2].conjlist);
         }
#line 1782 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 15:
#line 253 "parser/parser.y" /* yacc.c:1646  */
    {
            Conjunction* conj = new Conjunction((yyvsp[-2].tdecl)->size());
            conj->setTupleDecl( *((yyvsp[-2].tdecl)) );
            delete (yyvsp[-2].tdecl);
            
            std::list<Exp*>::iterator it2 = (yyvsp[-1].explist)->begin();
            std::list<Exp*>::iterator end2 = (yyvsp[-1].explist)->end();
            for (; it2 != end2; ++it2) {
               if ((*it2)->isEquality())
                  { conj->addEquality(*it2); }
               else if ((*it2)->isInequality())
                  { conj->addInequality(*it2); }
            }
            
            conj->substituteTupleDecl(); 
            (yyval.conj) = conj;
            delete (yyvsp[-1].explist);
         }
#line 1805 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 16:
#line 273 "parser/parser.y" /* yacc.c:1646  */
    {
            std::list<Conjunction*>* conlist=
               new std::list<Conjunction*>();
            conlist->push_back((yyvsp[0].conj));
            (yyval.conjlist) = conlist;
         }
#line 1816 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 17:
#line 281 "parser/parser.y" /* yacc.c:1646  */
    {
            (yyvsp[-2].conjlist)->push_back((yyvsp[0].conj));
            (yyval.conjlist)=(yyvsp[-2].conjlist);
         }
#line 1825 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 18:
#line 287 "parser/parser.y" /* yacc.c:1646  */
    {
            int arity = (yyvsp[-4].tdecl)->size() + (yyvsp[-2].tdecl)->size(), inarity = (yyvsp[-4].tdecl)->size();
            Conjunction* conj = new Conjunction(arity, inarity);
            conj->setTupleDecl( *((yyvsp[-4].tdecl)), *((yyvsp[-2].tdecl)) );
            delete (yyvsp[-4].tdecl);
            delete (yyvsp[-2].tdecl);
            
            std::list<Exp*>::iterator it2 = (yyvsp[-1].explist)->begin();
            std::list<Exp*>::iterator end2 = (yyvsp[-1].explist)->end();
            for (; it2 != end2; ++it2) {
               if ((*it2)->isEquality())
                  { conj->addEquality(*it2); }
               else if ((*it2)->isInequality())
                  { conj->addInequality(*it2); }
            }
            
            conj->substituteTupleDecl();
            (yyval.conj) = conj;

            delete (yyvsp[-1].explist);
         }
#line 1851 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 19:
#line 310 "parser/parser.y" /* yacc.c:1646  */
    { 
            (yyvsp[0].conjlist)->push_back((yyvsp[-2].conj)); 
            (yyval.conjlist) = (yyvsp[0].conjlist); 
         }
#line 1860 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 20:
#line 316 "parser/parser.y" /* yacc.c:1646  */
    {
            std::list<Conjunction*>* conlist = 
               new std::list<Conjunction*>();
            conlist->push_back((yyvsp[0].conj));
            (yyval.conjlist) = conlist;
         }
#line 1871 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 21:
#line 324 "parser/parser.y" /* yacc.c:1646  */
    {
            Conjunction* conj = new Conjunction((yyvsp[-1].tdecl)->size());
            conj->setTupleDecl(*((yyvsp[-1].tdecl)));
            delete (yyvsp[-1].tdecl);

            std::list<Exp*>::iterator it2 = (yyvsp[0].explist)->begin();
            std::list<Exp*>::iterator end2 = (yyvsp[0].explist)->end();
            for (; it2 != end2; ++it2) {
               if ((*it2)->isEquality())
                  { conj->addEquality(*it2); }
               else if ((*it2)->isInequality())
                  { conj->addInequality(*it2); }
            }
            
            conj->substituteTupleDecl(); 
            (yyval.conj) = conj;
            delete (yyvsp[0].explist);

         }
#line 1895 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 22:
#line 345 "parser/parser.y" /* yacc.c:1646  */
    { 
            (yyvsp[0].conjlist)->push_back((yyvsp[-2].conj)); 
            (yyval.conjlist) = (yyvsp[0].conjlist); 
         }
#line 1904 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 23:
#line 351 "parser/parser.y" /* yacc.c:1646  */
    {
            std::list<Conjunction*>* conlist = 
               new std::list<Conjunction*>();
            conlist->push_back((yyvsp[0].conj));
            (yyval.conjlist) = conlist;
         }
#line 1915 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 24:
#line 359 "parser/parser.y" /* yacc.c:1646  */
    {
            int arity = (yyvsp[-3].tdecl)->size() + (yyvsp[-1].tdecl)->size(), inarity = (yyvsp[-1].tdecl)->size();
            Conjunction* conj = new Conjunction(arity, inarity);
            conj->setTupleDecl(*((yyvsp[-3].tdecl)),*((yyvsp[-1].tdecl)));
            delete (yyvsp[-3].tdecl);
            delete (yyvsp[-1].tdecl);

            std::list<Exp*>::iterator it2 = (yyvsp[0].explist)->begin();
            std::list<Exp*>::iterator end2 = (yyvsp[0].explist)->end();
            for (; it2 != end2; ++it2) {
               if ((*it2)->isEquality())
                  { conj->addEquality(*it2); }
               else if ((*it2)->isInequality())
                  { conj->addInequality(*it2); }
            }
            
            conj->substituteTupleDecl();
            (yyval.conj) = conj;
            delete (yyvsp[0].explist);

         }
#line 1941 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 25:
#line 381 "parser/parser.y" /* yacc.c:1646  */
    {
            delete (yyvsp[-2].tdecl);
            (yyval.conj) =  new Conjunction(0,0);
            yyclearin;
      }
#line 1951 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 26:
#line 389 "parser/parser.y" /* yacc.c:1646  */
    {(yyval.symlist) = (yyvsp[-2].symlist);}
#line 1957 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 27:
#line 392 "parser/parser.y" /* yacc.c:1646  */
    {(yyval.symlist) = new std::list<std::string>();}
#line 1963 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 28:
#line 395 "parser/parser.y" /* yacc.c:1646  */
    {
            std::list<std::string>* symbols = new std::list<std::string>();
            symbols->push_back(*(yyvsp[0].sval));
            (yyval.symlist) = symbols;
            delete (yyvsp[0].sval);
         }
#line 1974 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 29:
#line 403 "parser/parser.y" /* yacc.c:1646  */
    {
            (yyvsp[0].symlist)->push_back(*(yyvsp[-2].sval));
            (yyval.symlist) = (yyvsp[0].symlist);
            delete (yyvsp[-2].sval);
         }
#line 1984 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 30:
#line 410 "parser/parser.y" /* yacc.c:1646  */
    { 
            (yyval.tdecl) = new TupleDecl();
         }
#line 1992 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 31:
#line 415 "parser/parser.y" /* yacc.c:1646  */
    {
            (yyval.tdecl) = new TupleDecl();
         }
#line 2000 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 32:
#line 420 "parser/parser.y" /* yacc.c:1646  */
    { (yyval.tdecl)= (yyvsp[-1].tdecl); }
#line 2006 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 33:
#line 423 "parser/parser.y" /* yacc.c:1646  */
    {
            delete (yyvsp[-1].tdecl);
            (yyval.tdecl) = new TupleDecl(0);
            yyclearin;
         }
#line 2016 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 34:
#line 431 "parser/parser.y" /* yacc.c:1646  */
    {
            (yyval.tdecl) = (yyvsp[0].tdecl);
         }
#line 2024 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 35:
#line 435 "parser/parser.y" /* yacc.c:1646  */
    {
            (yyval.tdecl) = (yyvsp[-2].tdecl);
            (yyval.tdecl)->concat(*(yyvsp[0].tdecl));
            delete (yyvsp[0].tdecl);
         }
#line 2034 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 36:
#line 442 "parser/parser.y" /* yacc.c:1646  */
    { (yyval.tdecl) = new TupleDecl(1);
           (yyval.tdecl)->setTupleElem(0, *(yyvsp[0].sval));
           delete (yyvsp[0].sval);
         }
#line 2043 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 37:
#line 448 "parser/parser.y" /* yacc.c:1646  */
    { (yyval.tdecl) = new TupleDecl(1);
           (yyval.tdecl)->setTupleElem(0, (yyvsp[0].ival));
         }
#line 2051 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 38:
#line 454 "parser/parser.y" /* yacc.c:1646  */
    {(yyval.sval) = (yyvsp[0].sval);}
#line 2057 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 39:
#line 459 "parser/parser.y" /* yacc.c:1646  */
    {(yyval.explist) = (yyvsp[0].explist);}
#line 2063 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 40:
#line 462 "parser/parser.y" /* yacc.c:1646  */
    {
            for (std::list<Exp*>::iterator it=(yyvsp[-1].explist)->begin(); it!=(yyvsp[-1].explist)->end(); ++it)
            { delete *it; }

            delete (yyvsp[-1].explist);
            (yyval.explist) = new std::list<Exp*>();
            yyclearin;
         }
#line 2076 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 41:
#line 472 "parser/parser.y" /* yacc.c:1646  */
    {(yyval.explist) = new std::list<Exp*>();}
#line 2082 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 42:
#line 475 "parser/parser.y" /* yacc.c:1646  */
    {(yyval.explist) = new std::list<Exp*>();}
#line 2088 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 43:
#line 479 "parser/parser.y" /* yacc.c:1646  */
    {
            std::list<Exp*>* clist = new std::list<Exp*>();
            clist->push_back((yyvsp[0].exp));
            (yyval.explist) = clist;
         }
#line 2098 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 44:
#line 486 "parser/parser.y" /* yacc.c:1646  */
    {
            (yyvsp[-2].explist)->push_back((yyvsp[0].exp));
            (yyval.explist) = (yyvsp[-2].explist);
         }
#line 2107 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 45:
#line 492 "parser/parser.y" /* yacc.c:1646  */
    {
            (yyvsp[-2].explist)->push_back((yyvsp[0].exp));
            (yyval.explist) = (yyvsp[-2].explist);
         }
#line 2116 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 46:
#line 498 "parser/parser.y" /* yacc.c:1646  */
    {
            std::list<Exp*>* clist = new std::list<Exp*>();
            clist->splice(clist->end(), *((yyvsp[0].explist)) );
            (yyval.explist) = clist;
         }
#line 2126 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 47:
#line 505 "parser/parser.y" /* yacc.c:1646  */
    {
            (yyvsp[-2].explist)->splice ((yyvsp[-2].explist)->end(), *((yyvsp[0].explist)) );
            (yyval.explist) = (yyvsp[-2].explist);
         }
#line 2135 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 48:
#line 511 "parser/parser.y" /* yacc.c:1646  */
    {
            (yyvsp[-2].explist)->splice ((yyvsp[-2].explist)->end(), *((yyvsp[0].explist)) );
            (yyval.explist) = (yyvsp[-2].explist);
         }
#line 2144 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 49:
#line 517 "parser/parser.y" /* yacc.c:1646  */
    {(yyval.exp) = (yyvsp[0].exp);}
#line 2150 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 50:
#line 520 "parser/parser.y" /* yacc.c:1646  */
    {(yyval.exp) = (yyvsp[0].exp);}
#line 2156 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 51:
#line 523 "parser/parser.y" /* yacc.c:1646  */
    {(yyval.exp) = (yyvsp[0].exp);}
#line 2162 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 52:
#line 526 "parser/parser.y" /* yacc.c:1646  */
    {(yyval.exp) = (yyvsp[0].exp);}
#line 2168 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 53:
#line 529 "parser/parser.y" /* yacc.c:1646  */
    {(yyval.exp) = (yyvsp[0].exp);}
#line 2174 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 54:
#line 532 "parser/parser.y" /* yacc.c:1646  */
    {
            (yyvsp[-2].exp)->setEquality();
            (yyvsp[0].exp)->multiplyBy(-1);
            (yyvsp[-2].exp)->addExp((yyvsp[0].exp));
            (yyval.exp) = (yyvsp[-2].exp);
         }
#line 2185 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 55:
#line 540 "parser/parser.y" /* yacc.c:1646  */
    {
            (yyvsp[0].exp)->setInequality();
            (yyvsp[-2].exp)->addTerm(new Term(1));
            (yyvsp[-2].exp)->multiplyBy(-1);
            (yyvsp[0].exp)->addExp((yyvsp[-2].exp));
            (yyval.exp) = (yyvsp[0].exp);
         }
#line 2197 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 56:
#line 549 "parser/parser.y" /* yacc.c:1646  */
    {
            (yyvsp[0].exp)->setInequality();
            (yyvsp[-2].exp)->multiplyBy(-1);
            (yyvsp[0].exp)->addExp((yyvsp[-2].exp));
            (yyval.exp) = (yyvsp[0].exp);
         }
#line 2208 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 57:
#line 557 "parser/parser.y" /* yacc.c:1646  */
    {
            (yyvsp[-2].exp)->setInequality();
            (yyvsp[0].exp)->addTerm(new Term(1));
            (yyvsp[0].exp)->multiplyBy(-1);
            (yyvsp[-2].exp)->addExp((yyvsp[0].exp));
            (yyval.exp) = (yyvsp[-2].exp);
         }
#line 2220 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 58:
#line 566 "parser/parser.y" /* yacc.c:1646  */
    {
            (yyvsp[-2].exp)->setInequality();
            (yyvsp[0].exp)->multiplyBy(-1);
            (yyvsp[-2].exp)->addExp((yyvsp[0].exp));
            (yyval.exp) = (yyvsp[-2].exp);
         }
#line 2231 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 59:
#line 574 "parser/parser.y" /* yacc.c:1646  */
    {(yyval.explist) = (yyvsp[0].explist);}
#line 2237 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 60:
#line 577 "parser/parser.y" /* yacc.c:1646  */
    {(yyval.explist) = (yyvsp[0].explist);}
#line 2243 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 61:
#line 580 "parser/parser.y" /* yacc.c:1646  */
    {(yyval.explist) = (yyvsp[0].explist);}
#line 2249 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 62:
#line 583 "parser/parser.y" /* yacc.c:1646  */
    {(yyval.explist) = (yyvsp[0].explist);}
#line 2255 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 63:
#line 586 "parser/parser.y" /* yacc.c:1646  */
    {
            Exp* tExp = (yyvsp[-2].exp)->clone();

            (yyvsp[-2].exp)->setInequality();
            (yyvsp[-4].exp)->multiplyBy(-1);
            (yyvsp[-2].exp)->addExp((yyvsp[-4].exp));

            (yyvsp[0].exp)->setInequality();
            tExp->addTerm(new Term(1));
            tExp->multiplyBy(-1);
            (yyvsp[0].exp)->addExp(tExp);

            std::list<Exp*>* clist = new std::list<Exp*>();
            clist->push_back((yyvsp[-2].exp));
            clist->push_back((yyvsp[0].exp));
            (yyval.explist) = clist;
         }
#line 2277 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 64:
#line 605 "parser/parser.y" /* yacc.c:1646  */
    {
            Exp* tExp = (yyvsp[-2].exp)->clone();

            (yyvsp[-2].exp)->setInequality();
            (yyvsp[-4].exp)->addTerm(new Term(1));
            (yyvsp[-4].exp)->multiplyBy(-1);
            (yyvsp[-2].exp)->addExp((yyvsp[-4].exp));

            (yyvsp[0].exp)->setInequality();
            tExp->multiplyBy(-1);
            (yyvsp[0].exp)->addExp(tExp);

            std::list<Exp*>* clist = new std::list<Exp*>();
            clist->push_back((yyvsp[-2].exp));
            clist->push_back((yyvsp[0].exp));
            (yyval.explist) = clist;
         }
#line 2299 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 65:
#line 624 "parser/parser.y" /* yacc.c:1646  */
    {
            Exp* tExp = (yyvsp[-2].exp)->clone();

            (yyvsp[-2].exp)->setInequality();
            (yyvsp[-4].exp)->multiplyBy(-1);
            (yyvsp[-2].exp)->addExp((yyvsp[-4].exp));

            (yyvsp[0].exp)->setInequality();
            tExp->multiplyBy(-1);
            (yyvsp[0].exp)->addExp(tExp);

            std::list<Exp*>* clist = new std::list<Exp*>();
            clist->push_back((yyvsp[-2].exp));
            clist->push_back((yyvsp[0].exp));
            (yyval.explist) = clist;
         }
#line 2320 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 66:
#line 642 "parser/parser.y" /* yacc.c:1646  */
    {
            Exp* tExp = (yyvsp[-2].exp)->clone();

            (yyvsp[-2].exp)->setInequality();
            (yyvsp[-4].exp)->addTerm(new Term(1));
            (yyvsp[-4].exp)->multiplyBy(-1);
            (yyvsp[-2].exp)->addExp((yyvsp[-4].exp));

            (yyvsp[0].exp)->setInequality();
            tExp->addTerm(new Term(1));
            tExp->multiplyBy(-1);
            (yyvsp[0].exp)->addExp(tExp);

            std::list<Exp*>* clist = new std::list<Exp*>();
            clist->push_back((yyvsp[-2].exp));
            clist->push_back((yyvsp[0].exp));
            (yyval.explist) = clist;
         }
#line 2343 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 67:
#line 662 "parser/parser.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2349 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 68:
#line 665 "parser/parser.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2355 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 69:
#line 668 "parser/parser.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2361 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 70:
#line 671 "parser/parser.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2367 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 71:
#line 674 "parser/parser.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2373 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 72:
#line 677 "parser/parser.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2379 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 73:
#line 680 "parser/parser.y" /* yacc.c:1646  */
    {
            // If there is only one expression then just pass
            // that up, otherwise create a TupleExpTerm and put that
            // in an Exp.
            if ((yyvsp[-1].explist)->size() == 1) {
                (yyval.exp) = (yyvsp[-1].explist)->front();
            } else {
                TupleExpTerm* tuple = new TupleExpTerm((yyvsp[-1].explist)->size());
                unsigned int count = 0;
                for (std::list<Exp*>::iterator i=(yyvsp[-1].explist)->begin(); i != (yyvsp[-1].explist)->end();
                        ++i) {
                    tuple->setExpElem(count++,*i);
                }
                Exp* exp = new Exp();
                exp -> addTerm(tuple);
                (yyval.exp) = exp;
            }
            delete (yyvsp[-1].explist);
         }
#line 2403 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 74:
#line 701 "parser/parser.y" /* yacc.c:1646  */
    { 
            Exp* exp = new Exp();
            exp->addTerm(new Term((yyvsp[0].ival)));
            (yyval.exp) = exp; 
         }
#line 2413 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 75:
#line 708 "parser/parser.y" /* yacc.c:1646  */
    {
            (yyvsp[0].exp)->multiplyBy(-1);
            (yyval.exp) = (yyvsp[0].exp);
         }
#line 2422 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 76:
#line 714 "parser/parser.y" /* yacc.c:1646  */
    {
            (yyvsp[-2].exp)->addExp((yyvsp[0].exp));
            (yyval.exp) = (yyvsp[-2].exp);
         }
#line 2431 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 77:
#line 720 "parser/parser.y" /* yacc.c:1646  */
    {
            (yyvsp[0].exp)->multiplyBy(-1);
            (yyvsp[-2].exp)->addExp((yyvsp[0].exp));
            (yyval.exp) = (yyvsp[-2].exp);
         }
#line 2441 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 78:
#line 727 "parser/parser.y" /* yacc.c:1646  */
    {
            (yyvsp[0].exp)->multiplyBy((yyvsp[-1].ival));
            (yyval.exp) = (yyvsp[0].exp);
         }
#line 2450 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 79:
#line 733 "parser/parser.y" /* yacc.c:1646  */
    {
            (yyvsp[0].exp)->multiplyBy((yyvsp[-2].ival));
            (yyval.exp) = (yyvsp[0].exp);
         }
#line 2459 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 80:
#line 739 "parser/parser.y" /* yacc.c:1646  */
    {
            (yyvsp[-2].exp)->multiplyBy((yyvsp[0].ival));
            (yyval.exp) = (yyvsp[-2].exp);
         }
#line 2468 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 81:
#line 745 "parser/parser.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2474 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 82:
#line 748 "parser/parser.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2480 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 83:
#line 751 "parser/parser.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2486 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 84:
#line 754 "parser/parser.y" /* yacc.c:1646  */
    {
            Exp* exp = new Exp();
            exp->addTerm(new VarTerm(*(yyvsp[0].sval)));
            (yyval.exp) = exp;
            delete (yyvsp[0].sval);
         }
#line 2497 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 85:
#line 769 "parser/parser.y" /* yacc.c:1646  */
    {
            UFCallTerm* uf_call = new UFCallTerm(*(yyvsp[-3].sval), (yyvsp[-1].explist)->size());
            unsigned int count = 0;
            for (std::list<Exp*>::iterator i=(yyvsp[-1].explist)->begin(); i != (yyvsp[-1].explist)->end(); ++i) {
                uf_call->setParamExp(count++,*i);
            }
            Exp* exp = new Exp();
            exp -> addTerm(uf_call);
            (yyval.exp) = exp;
            delete (yyvsp[-3].sval);
            delete (yyvsp[-1].explist);
         }
#line 2514 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 86:
#line 783 "parser/parser.y" /* yacc.c:1646  */
    {
            UFCallTerm* uf_call = new UFCallTerm(*(yyvsp[-6].sval), (yyvsp[-4].explist)->size(), (yyvsp[-1].ival));
            unsigned int count = 0;
            for (std::list<Exp*>::iterator i=(yyvsp[-4].explist)->begin(); i != (yyvsp[-4].explist)->end(); ++i) {
                uf_call->setParamExp(count++,*i);
            }
            Exp* exp = new Exp();
            exp -> addTerm(uf_call);
            (yyval.exp) = exp;
            delete (yyvsp[-4].explist);
            delete (yyvsp[-6].sval);
         }
#line 2531 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 87:
#line 797 "parser/parser.y" /* yacc.c:1646  */
    {
            for (std::list<Exp*>::iterator it=(yyvsp[-1].explist)->begin(); it!=(yyvsp[-1].explist)->end(); ++it)
            { delete *it; }
            (yyval.exp) = new Exp();
            delete (yyvsp[-3].sval);
            delete (yyvsp[-1].explist);
            yyclearin;
         }
#line 2544 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 88:
#line 807 "parser/parser.y" /* yacc.c:1646  */
    {
            (yyvsp[-2].explist)->push_back((yyvsp[0].exp));
            (yyval.explist) = (yyvsp[-2].explist);
         }
#line 2553 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;

  case 89:
#line 813 "parser/parser.y" /* yacc.c:1646  */
    {
            std::list<Exp*>* exp_list = new std::list<Exp*>();
            exp_list->push_back((yyvsp[0].exp));
            (yyval.explist) = exp_list;
         }
#line 2563 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
    break;


#line 2567 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.cc" /* yacc.c:1646  */
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
#line 820 "parser/parser.y" /* yacc.c:1906  */



/*! yyerror is called when an error occurs in parsing. It prints localion of the
error as well as why the error occured to standard error. When an error occurs
we set the parse_error;
@param const char pointer
*/
#include <iostream>
#include <typeinfo>
#include <sstream>
void yyerror (const char *s)
{
   std::stringstream EM;
   EM << s << " at '" << yytext << "', line " << yylineno << std::endl;
   //sets the parse error flag to true
   parser::set_parse_error(EM.str());
   parser::clearAll();
   yyclearin;
}

