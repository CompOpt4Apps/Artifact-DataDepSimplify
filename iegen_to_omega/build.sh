#!/bin/bash
export ROSEHOME=../rose_build
export BOOSTHOME=../boost
export IEGENHOME=../IEGenLib/iegen
export OMEGAHOME=../chill/omega
g++ iegen_to_omega.cc ./poly/iegenlib/gen_parser.cc \
    ./poly/iegenlib/gen_scanner.cc ./poly/iegenlib/parser.cc \
    ../IEGenLib/src/set_relation/set_relation.cc \
    ../IEGenLib/src/set_relation/UFCallMap.cc \
    ../IEGenLib/src/set_relation/expression.cc \
    ../IEGenLib/src/set_relation/TupleDecl.cc \
    ../IEGenLib/src/set_relation/SubMap.cc \
    ../IEGenLib/src/set_relation/UninterpFunc.cc \
    ../IEGenLib/src/set_relation/environment.cc \
    ../IEGenLib/src/set_relation/isl_str_manipulation.cc \
    ../IEGenLib/src/util/jsonHelper.cc \
    ../IEGenLib/src/util/util.cc \
    ../IEGenLib/lib/installed/lib/libisl.a \
    ./poly/omega/parser.tab.cc \
    ./poly/omega/lex.yy.cc ./poly/omega/AST.cc \
    ../chill/build/omega/basic/libomega_basic.a \
    ../chill/build/omega/omega_lib/libomega_lib.a \
    ../chill/build/omega/code_gen/libcodegen.a \
    -I. -I$./poly/iegenlib -I${IEGENHOME}/include/iegenlib -I$CHILLHOME \
    -I${OMEGAHOME}/omega_calc/include -I${OMEGAHOME}/basic/include \
    -I${OMEGAHOME}/omega_lib/include -I${OMEGAHOME}/code_gen/include \
    -g -O3 -o iegen_to_omega -std=c++11

