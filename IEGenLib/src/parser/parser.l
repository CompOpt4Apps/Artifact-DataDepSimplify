/*! \file parser.l
 *
 * \brief Flex file describing the tokens in inspector templates.
 *
 * \date Started: 5/17/2010
 * # $Revision:: 622                $: last committed revision
 * # $Date:: 2013-01-18 13:11:32 -0#$: date of last committed revision
 * # $Author:: cathie               $: author of last committed revision
 *
 * \authors Michelle Strout, Alan LaMielle, Nicholas Jeanette
 *
 * Copyright (c) 2009, 2010, 2011, 2012, Colorado State University <br>
 * All rights reserved. <br>
 * See ../../COPYING for details. <br>
*/
%option nounput
%option noinput

%{
    #include <iostream>
    #include <parser/parser.h>    
    #include "gen_parser.hh"

    #undef YY_INPUT

    #define YY_INPUT(buf,result,max_size) \
    { \
        int c = iegenlib::parser::string_get_next_char(); \
        result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
    }

%}

/*produces a integer*/
DIGIT    [0-9]+
/*IDENT    [a-zA-Z][a-zA-Z0-9'_]**/
IDENT [a-zA-Z'_]?[a-zA-Z'$]+[a-zA-Z0-9'$_]*
/*catches invalid identifiers*/
INVALID_IDENT [_$]*[a-zA-Z'_]?[a-zA-Z'$]+[a-zA-Z0-9'$_]*
/*produces the word and*/
WORDAND  [aA][nN][dD]

/* Define all symbols that are used*/
/*NOTE: NOT symbol not included if one wishes add "!="     {return NEQ;} to the
list of symbols below*/
%%
"+"       {return PLUS;}
"-"       {return DASH;}
"*"       {return STAR;}
"{"       {return LBRACE;}
"}"       {return RBRACE;}
"["       {return LBRACKET;}
"]"       {return RBRACKET;}
"("       {return LPAREN;}
")"       {return RPAREN;}
","       {return COMMA;}
":"       {return COLON;}
"="       {return EQ;}
"<"       {return LT;}
">"       {return GT;}
"<="      {return LTE;}
">="      {return GTE;}
"->"      {return ARROW;}
"&&"      {return AND;}
";"       {return SEMI;}
"UNION"   {return UNION;}
"union"   {return UNION;}
"OR"      {return OR;}
"or"      {return OR;}
"exists"  {return EXISTS;}
"inverse" {return INVERSE;}
"INVERSE" {return INVERSE;}

{WORDAND} {return WAND;}

{IDENT} { yylval.sval = new std::string(yytext);
          return ID;}

{DIGIT} { yylval.ival = atoi(yytext);
          return INT;}

{INVALID_IDENT} { //yylval.sval = new std::string(yytext);
          return INVALID_ID;} 

[ \t\n\r]  /* skip whitespace */

<<EOF>> { return 0; };

%%

int yywrap() { return 1; }

