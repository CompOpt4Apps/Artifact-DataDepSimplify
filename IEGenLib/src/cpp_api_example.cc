/*!
 * \file cpp_api_example.cc
 *
 * \brief This file is a small example of using the IEGenLib C++ API.  
 *
 * To compile, execute the following command from the root of the project:
 *     cd ..
 *     make install VERBOSE=1   // NOTE the compiler being used, <C++COMP>
 *     cd src
 *     <C++COMP> -o cpp_api_example -I../iegen/include/iegenlib -L../iegen/lib -liegenlib -L../lib/installed/lib  -lisl -lgmp cpp_api_example.cc
 *
 * \date Date Started: 3/11/11
 *
 * \authors Alan LaMielle, Michelle Strout, Cathie Olschanowsky
 *
 * Copyright (c) 2011-2013, Colorado State University <br>
 * Copyright (c) 2015, University of Arizona <br>
 * All rights reserved. <br>
 * See COPYING for details. <br>
 */

#include <iostream>
#include <iegenlib.h>

using iegenlib::Set;

int main(int ac, char **av)
{
//  iegenlib::Set* set = new iegenlib::Set("{[a]: a=10}");
  Set* set = new Set("{[a]:0<a}");

  std::cout << set->prettyPrintString() << std::endl;

  return 0;
}
