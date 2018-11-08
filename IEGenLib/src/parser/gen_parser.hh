/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

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
#line 36 "parser/parser.y" /* yacc.c:1909  */

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

#line 100 "/home/king/tools/branch/islUpdate/src/parser/gen_parser.hh" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_HOME_KING_TOOLS_BRANCH_ISLUPDATE_SRC_PARSER_GEN_PARSER_HH_INCLUDED  */
