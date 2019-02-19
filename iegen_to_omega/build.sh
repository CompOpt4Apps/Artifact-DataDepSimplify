#!/bin/bash
g++ iegen_to_omega.cc $OMEGAHOME/parser/parser.tab.cc \
    $OMEGAHOME/parser/lex.yy.cc $OMEGAHOME/parser/AST.cc \
    ${IEGENHOME}/build/src/libiegenlib.a ${IEGENHOME}/lib/installed/lib/libisl.a \
    ${OMEGAHOME}/basic/libomega_basic.a \
    ${OMEGAHOME}/omega_lib/libomega_lib.a ${OMEGAHOME}/code_gen/libcodegen.a \
    -I. -I${IEGENHOME}/src -I$CHILLHOME -I${OMEGAHOME}/basic/include \
    -I${OMEGAHOME}/omega_lib/include -I${OMEGAHOME}/code_gen/include \
    -o iegen_to_omega -std=c++11
