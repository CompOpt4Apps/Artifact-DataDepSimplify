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
#line 112 "parser.yy" /* yacc.c:1909  */

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

#line 169 "parser.tab.hh" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_HH_INCLUDED  */
