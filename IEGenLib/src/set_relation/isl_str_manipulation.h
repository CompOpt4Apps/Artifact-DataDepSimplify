/*!
 * \file isl_str_manipulation.h
 *
 * \brief Interface of isl string manipulator functions
 *
 * \date Started: 2016-03-11
 *
 * \authors Michelle Strout, Mahdi Soltan Mohammadi
 *
 * Copyright (c) 2016, University of Arizona <br>
 * All rights reserved. 
 * See ../../COPYING for details. <br>
 */
 
#ifndef ISL_STR_MANIPULATION_H_
#define ISL_STR_MANIPULATION_H_

#include<iostream>
#include<string>
#include <queue>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

#include <isl/set.h>   // ISL Sets
#include <isl/map.h>   // ISL Relations
#include <isl/union_set.h>   // ISL Union_Sets
#include <isl/union_map.h>   // ISL Union_Relations

namespace iegenlib{

struct srParts;
//****** ISL tuple correction functions

/*! trim function trims spacing from left and right of a string
*/
std::string trim(std::string s);

/*! This function extracts tuple variables from a Tuple Declaration string.
**  Tuple Declaration can be either of Relation or Set form
**  Input:  "[i1,col_tv3_] -> [i3,i4]" or
**          "[i1,col_tv3_,i3,i4]"  
**  Output: (pointer to)
**          {"i1", "col_tv3_", "i3", "i4"}
*/
std::queue<std::string> tupVarsExtract(std::string tupDecl, int inArity, int outArity);

/*! This function constructs equality constraints between Tuple Varialbes
**  that are replaced eachother by ISL. To do this, it takes in two
**  Tuple declaration, extracts their Tuple Variables, then creates
**  equalities for those that are replaced:
**      origTupDecl: [i1, i2] -> [1, i4]
**      islTupDecl : [col_tv1_, i2] -> [1, i2]
**      output:      i1 = col_tv1_ and i4 = i2
**  Note: noFirstAnd determines whether we should put "and" at the beginning of
**  the output string, which depends on original constraints being empty or not. 
*/
std::string missingEqs(std::string origTupDecl, std::string islTupDecl,
                              int inArity, int outArity=0);

//! An Structure to store different parts of a Set or Relation string
typedef struct srParts{

    srParts(){
        symVars = "";
        sC = '{';
        sepC = ':';
        eC = '}';
    }

    std::string symVars;     // Symbolic constants 
    std::string sC;          // Starting character = '{'
    std::string tupDecl;     // Tuplel declaration 
    std::string sepC;        // Separating character = ':'
    std::string constraints; // Constraints
    std::string eC;          // Ending character = '}'
}srParts;

/*! This function takes in a Set or Relation string and returns different 
**  parts of it in a srParts structure.
*/
srParts getPartsFromStr(std::string str);

/*! The main function that restores changes that ISL library applies to
**  Tuple Declaration because of the equality constraints. ISL library replaces
**  tuple variables with their equal expression if one exists in the constraints:
**    input to isl    (origStr): "[n] -> { [i] : i = n and i <= 10 }"
**    output from isl (islStr) : "[n] -> { [n] : n <= 10 }"
**  This function replaces Tuple Declaration from ISL string with original one,
**  and creates missing equalities and puts them back into constraints.
**  Ex:
**  Inputs:  origStr & islStr
    Output:
             correctedStr = "[n] -> { [i] : n <= 10 and i = n }"
*/
std::string revertISLTupDeclToOrig(std::string origStr, std::string islStr,
                              int inArity, int outArity);

/*! This function takes in a Set or Relation string and removes tuple variable
**  located in poTv position from Tuple Declaration of the string.
**     str         : [n] -> { [i1,i2,i3] : ... }
**     poTv        : 1
**     correctedStr: [n] -> { [i1,i3] : ... }
*/
std::string projectOutStrCorrection(std::string str, int poTv,
                                    int inArity, int outArity);
//****** ISL tuple correction End

//! This function takes a Set string and returns equivalent isl_set*
isl_set* islStringToSet( std::string relstr , isl_ctx *ctx, bool doCoalesce = true );

/*! This function takes an isl_set* and returns equivalent Set string
** The function takes ownership of input argument 'iset'
*/
std::string islSetToString ( isl_set* iset , isl_ctx *ctx );

//! This function takes a Relation string and returns pointer to equ. isl_map
isl_map* islStringToMap( std::string relstr , isl_ctx *ctx, bool doCoalesce = true );

/*! This function takes an isl_map* and returns pointer to equ. Relation string
** The function takes ownership of input argument 'imap'
*/
std::string islMapToString ( isl_map* imap , isl_ctx *ctx );

//! This function takes a Set string and returns equivalent isl_union_set*
isl_union_set* islStringToUnionSet( std::string relstr , isl_ctx *ctx, bool doCoalesce = true );

/*! This function takes an isl_union_set* and returns equivalent Set string
** The function takes ownership of input argument 'iset'
*/
std::string islUnionSetToString ( isl_union_set* iset , isl_ctx *ctx);

//! This function takes a Relation string and returns pointer to equ. isl_union_map
isl_union_map* islStringToUnionMap( std::string relstr , isl_ctx *ctx, bool doCoalesce = true );

/*! This function takes an isl_union_map* and returns pointer to equ. Relation string
** The function takes ownership of input argument 'imap'
*/
std::string islUnionMapToString ( isl_union_map* imap , isl_ctx *ctx );

/*! This function buils and returns a full string from parts
*/
std::string getFullStrFromParts (srParts  parts);


/*! This function turns an IEGenLib Set represented in string form
**  into an IEGenLib Relation string
*/
std::string setStr2RelationStr(std::string set, int inArity, int outArity);

/*! This function turns an IEGenLib Relation represented in string form
**  into an IEGenLib Set string
*/
std::string relationStr2SetStr(std::string relation, int inArity, int outArity);


/*! This function adds constraints from a Set/Relation string to another one
**  without checking whether their tuple declaration match or not. 
**  Note that Intersect functionality of IEGenLib that can be used for similar 
**  purposes checks for matching tuple declaration.
*/
std::string strAddConstraints(std::string dest, std::string src);



}// iegenlib namespace
#endif
