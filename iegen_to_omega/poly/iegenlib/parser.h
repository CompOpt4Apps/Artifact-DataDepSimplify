/*!
 * \file parser.h
 *
 * \brief a namespace to allow the user to make use of the parser
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


#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <stdio.h>
#include <typeinfo>
#include <string>
#include <stack>

#include <set_relation/set_relation.h>
#include <util/util.h>

/*!
    The parser namespace hides the set and relation lexer and parser
    functions and associated global variables.
*/
namespace iegenlib { namespace parser{
   //! Used by lexer to obtain each character of input string being parsed.
   int string_get_next_char();

   /*!parser_env passes a string representation of an environment 
   to the parser and an Environment gets created.
   @param string env string that is to be parsed
   @return Null pointer if error occurs or an environment is returned
   @return Environment pointer when parsing is successful
   */
   Environment* parse_env(std::string env_string);

   /*! setter for the parse_env_result
   @param Environment parse_env_result */
   void set_parse_env_result(Environment* env);

   /*!parser_set passes a string representation of a set to the parser and a
   Set gets created.
   @param string set string that is to be parsed
   @return Null pointer if error occurs or a relation is returned
   @return Set pointer when parsing is successful
   */
   Set* parse_set(std::string set_string);

   /*! setter for the parse_set_result
   @param Set parse_set_result */
   void set_parse_set_result(Set* s);

   /*! passes a relation string into the parser and creates the Relation
   @param string relation string that is to be parsed
   @return Null pointer if error occurs or a set is returned
   @return Relation pointer when parsing is successful */
   Relation* parse_relation(std::string relation_string);

   /*! setter for the parse_relation_result
   @param Relation parse_relation_result */
   void set_parse_relation_result(Relation* s);

   /*! sets the parse_error flag to true */
   void set_parse_error(std::string err);

   /*! getter for the parse_error flag
   @return bool parse_error */
   bool get_parse_error();

   /*! clearAll frees (if needed) and resets the parse_relation_result and the
   parse_set_result so no data is left in those variables*/
   void clearAll();

}}//end namespace iegenlib::parser

#endif
