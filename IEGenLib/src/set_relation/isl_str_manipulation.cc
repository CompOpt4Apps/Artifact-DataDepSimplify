/*!
 * \file isl_str_manipulation.h
 *
 * \Implementations of isl string manipulator functions
 *
 * \date Started: 2016-03-11
 *
 * \authors Michelle Strout, Mahdi Soltan Mohammadi
 *
 * Copyright (c) 2016, University of Arizona <br>
 * All rights reserved. 
 * See ../../COPYING for details. <br>
 */

#include "isl_str_manipulation.h"

namespace iegenlib{


//****** ISL tuple correction functions

/*! trim function trims spacing from left and right of a string
*/
std::string trim(std::string s) {
    // trim left
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
    // trim right
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}
/*! This function extracts tuple variables from a Tuple Declaration string.
**  Tuple Declaration can be either of Relation or Set form
**  Input:  "[i1,col_tv3_] -> [i3,i4]" or
**          "[i1,col_tv3_,i3,i4]"  
**  Output: (pointer to)
**          {"i1", "col_tv3_", "i3", "i4"}
*/
std::queue<std::string> tupVarsExtract(std::string tupDecl, int inArity, int outArity){

    int st=0, end=0;
    int nparts=1;
    int arity = inArity + outArity;
    std::queue<std::string> tupVars;

    if( outArity != 0 ){
      nparts = 2;
      arity = inArity;
    }
    for(int p=0 ; p < nparts ; p++){
        st = tupDecl.find_first_of('[', st);
        end = tupDecl.find_first_of(']', st);
        std::string str = tupDecl.substr(st+1,end-st-1);

        int tst = 0,tend = 0 ;
        for(int i = 0 ; i < arity ; i++ ){

            tend = str.find_first_of(',', tst);
            tupVars.push( trim( str.substr(tst,tend-tst) ) ); 
            tst = tend + 1;
        }

        arity = outArity;
        st = end + 1;
    }

    return tupVars;
}

/*! This function constructs equality constraints between Tuple Varialbes
**  that are replaced eachother by ISL. To do this, it takes in two
**  Tuple declaration, extracts their Tuple Variables, then creates
**  equalities for those that are replaced:
**      origTupDecl: [i1, i2] -> [1, i4]
**      islTupDecl : [col_tv1_, i2] -> [1, i2]
**      output:      i1 = col_tv1_ and i4 = i2
*/
std::string missingEqs(std::string origTupDecl, std::string islTupDecl,
                              int inArity, int outArity){
    std::string eqs("");
    int arity = inArity + outArity;
    bool firstEq = true;
 
    std::queue<std::string> origTupVars = tupVarsExtract(origTupDecl, inArity, outArity);
    std::queue<std::string> islTupVars = tupVarsExtract(islTupDecl, inArity, outArity);

    for(int i = 0 ; i < arity ; i++){
        std::string orig = origTupVars.front();
        origTupVars.pop();
        std::string isl = islTupVars.front();
        islTupVars.pop();
        if( orig != isl ){
            if(firstEq){
                eqs +=  " " + orig + " = " + isl;
                firstEq = false;
            } else {
                eqs +=  " and " + orig + " = " + isl;
            }
        }
    }

    return eqs;
}
/*! This function takes in a Set or Relation string and returns different 
**  parts of it in a srParts structure.
**  Ex:   set = "[n,m] -> { [i,j] : i < n and j > m }"
**
**  OUTPUT: parts    WHICH IS:
**           parts.symVars     = "[n,m] -> "          // Symbolic constants 
**           parts.sC          = '{'                  // Starting character
**           parts.tupDecl     = " [i,j] "            // Tuplel declaration 
**           parts.sepC        = ':'                  // Separating character
**           parts.constraints = " i < n and j > m "  // Constraints
**           parts.eC          = '}'                  // Ending character
*/
srParts getPartsFromStr(std::string str){
    srParts parts;
    int l1,l2,l3;

    l1 = str.find_first_of('{', 0);
    l2 = str.find_first_of(':', l1);
    if( l2 < 0 ){
        parts.symVars =     str.substr( 0 , l1-0 );
        l3 = str.find_first_of('}', l1);
        parts.tupDecl =     str.substr( l1+1 , l3-(l1+1)-1 );
        parts.constraints = "";
        return parts;
    }
    l3 = str.find_first_of('}', l2);

    parts.symVars =     str.substr( 0 , l1-0 );
    parts.tupDecl =     str.substr( l1+1 , l2-(l1+1) );
    parts.constraints = str.substr( l2+1 , l3-(l2+1) );

    return parts;
}
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
                              int inArity, int outArity){
    std::string correctedStr, eqsStr;
    
    srParts origParts = getPartsFromStr(origStr);
    srParts islParts = getPartsFromStr(islStr);

    eqsStr = missingEqs( origParts.tupDecl , islParts.tupDecl ,
                         inArity, outArity );
    
    if( islParts.constraints.length() == 0){
        correctedStr = islParts.symVars + islParts.sC +
                       origParts.tupDecl + islParts.sepC + eqsStr + islParts.eC;
    } else if( eqsStr.length() == 0){
        correctedStr = islParts.symVars + islParts.sC + origParts.tupDecl + 
                       islParts.sepC + islParts.constraints + islParts.eC;
    } else {
        correctedStr = islParts.symVars + islParts.sC + origParts.tupDecl + 
                       islParts.sepC + islParts.constraints + 
                       " and " + eqsStr + islParts.eC;
    }

    return correctedStr;
}
/*! This function takes in a Set or Relation string and removes tuple variable
**  located in poTv position from Tuple Declaration of the string.
**     str         : [n] -> { [i1,i2,i3] : ... }
**     poTv        : 1
**     correctedStr: [n] -> { [i1,i3] : ... }
*/
std::string projectOutStrCorrection(std::string str, int poTv,
                                    int inArity, int outArity){
    std::string correctedStr, newTupDecl;
    int arity = inArity + outArity;
    
    srParts parts = getPartsFromStr(str);
    
    std::queue<std::string> origTupVars = tupVarsExtract(parts.tupDecl,
                                              inArity, outArity);
    newTupDecl = "[";
    for (int i = 0 ; i < arity ; i++){
        std::string orig = origTupVars.front();
        origTupVars.pop();
        if( i == inArity && outArity != 0){
            newTupDecl += "] -> [";
        }
        if( i != poTv ){
            newTupDecl += orig;
            if( i != arity-1 && i != inArity-1 && 
                !( ((i == arity-2)&&(poTv == arity-1)) ||
                   ((i == inArity-2)&&(poTv == inArity-1)) )
              ){
                newTupDecl += ", ";
            }
        }
    }
    newTupDecl += "] ";
  
    if (parts.constraints.length() == 0 ){
        correctedStr = parts.symVars + parts.sC + newTupDecl + parts.eC; 
    } else{
        correctedStr = parts.symVars + parts.sC + newTupDecl +
                       parts.sepC + parts.constraints + parts.eC;
    }

    return correctedStr;
}
//****** ISL tuple correction End

//! This function takes a Set string and returns equivalent isl_set*
isl_set* islStringToSet( std::string relstr , isl_ctx *ctx, bool doCoalesce )
{
  // load Relation r into ISL map
  isl_set* iset = isl_set_read_from_str(ctx, relstr.c_str());

  if( doCoalesce ){
    iset = isl_set_coalesce(iset);
  }

  return iset;
}

/*! This function takes an isl_set* and returns equivalent Set string
** The function takes ownership of input argument 'iset'
*/
std::string islSetToString ( isl_set* iset , isl_ctx *ctx ) {
  // Get an isl printer and associate to an isl context
  isl_printer * ip = isl_printer_to_str(ctx);

  // get string back from ISL map
  isl_printer_set_output_format(ip, ISL_FORMAT_ISL);
  isl_printer_print_set(ip, iset);
  char *i_str = isl_printer_get_str(ip);
  std::string stringFromISL (i_str); 
  
  // clean-up
  isl_printer_flush(ip);
  isl_printer_free(ip);
  isl_set_free(iset);
  iset= NULL;
  free(i_str);

  return stringFromISL;
}

//! This function takes a Relation string and returns pointer to equ. isl_map
isl_map* islStringToMap( std::string relstr , isl_ctx *ctx, bool doCoalesce )
{
  // load Relation r into ISL map
  isl_map* imap = isl_map_read_from_str(ctx, relstr.c_str());

  if( doCoalesce ){
    imap = isl_map_coalesce(imap);
  }

  return imap;
}


/*! This function takes an isl_map* and returns pointer to equ. Relation string
** The function takes ownership of input argument 'imap'
*/
std::string islMapToString ( isl_map* imap , isl_ctx *ctx )
{
  // Get an isl printer and associate to an isl context
  isl_printer * ip = isl_printer_to_str(ctx);

  // get string back from ISL map
  isl_printer_set_output_format(ip , ISL_FORMAT_ISL);
  isl_printer_print_map(ip ,imap);
  char *i_str = isl_printer_get_str(ip);
  std::string stringFromISL (i_str); 
  
  // clean-up
  isl_printer_flush(ip);
  isl_printer_free(ip);
  isl_map_free(imap);
  imap= NULL;
  free(i_str);

  return stringFromISL;
}

//! This function takes a Set string and returns equivalent isl_union_set*
isl_union_set* islStringToUnionSet( std::string relstr , isl_ctx *ctx, bool doCoalesce )
{
  // load Relation r into ISL map
  isl_union_set* iset = isl_union_set_read_from_str(ctx, relstr.c_str());

  if( doCoalesce ){
    iset = isl_union_set_coalesce(iset);
  }

  return iset;
}

/*! This function takes an isl_union_set* and returns equivalent Set string
** The function takes ownership of input argument 'iset'
*/
std::string islUnionSetToString ( isl_union_set* iset , isl_ctx *ctx ) {
  // Get an isl printer and associate to an isl context
  isl_printer * ip = isl_printer_to_str(ctx);

  // get string back from ISL map
  isl_printer_set_output_format(ip, ISL_FORMAT_ISL);
  isl_printer_print_union_set(ip, iset);
  char *i_str = isl_printer_get_str(ip);
  std::string stringFromISL (i_str); 
  
  // clean-up
  isl_printer_flush(ip);
  isl_printer_free(ip);
  isl_union_set_free(iset);
  iset= NULL;
  free(i_str);

  return stringFromISL;
}

//! This function takes a Relation string and returns pointer to equ. isl_union_map
isl_union_map* islStringToUnionMap( std::string relstr , isl_ctx *ctx, bool doCoalesce )
{
  // load Relation r into ISL map
  isl_union_map* imap = isl_union_map_read_from_str(ctx, relstr.c_str());

  if( doCoalesce ){
    imap = isl_union_map_coalesce(imap);
    imap = isl_union_map_coalesce(imap);
  }

  return imap;
}

/*! This function takes an isl_union_map* and returns pointer to equ. Relation string
** The function takes ownership of input argument 'imap'
*/
std::string islUnionMapToString ( isl_union_map* imap , isl_ctx *ctx )
{
  // Get an isl printer and associate to an isl context
  isl_printer * ip = isl_printer_to_str(ctx);

  // get string back from ISL map
  isl_printer_set_output_format(ip , ISL_FORMAT_ISL);
  isl_printer_print_union_map(ip ,imap);
  char *i_str = isl_printer_get_str(ip);
  std::string stringFromISL (i_str); 
  
  // clean-up
  isl_printer_flush(ip);
  isl_printer_free(ip);
  isl_union_map_free(imap);
  imap= NULL;
  free(i_str);

  return stringFromISL;
}

/*! This function buils and returns a full string from parts
*/
std::string getFullStrFromParts(srParts  parts){
  std::string compStr;
  if( parts.symVars != "" ){
    compStr = parts.symVars + " -> ";
  }
  compStr = compStr + parts.sC + parts.tupDecl +
            parts.sepC + parts.constraints + parts.eC;

  return compStr;
}


/*! This function turns an IEGenLib Set represented in string form
**  into an IEGenLib Relation string. Basically, it just changes
**  the tuple declaration, from [...] to [...] -> [...], based on
**  inArity and outArity. set's arity equals to inArity+outArity
*/
std::string setStr2RelationStr(std::string set, int inArity, int outArity){

  int arity = inArity+outArity;
  srParts setParts;
  setParts = getPartsFromStr(set);

  std::queue<std::string> iters;
  iters = tupVarsExtract(setParts.tupDecl, arity, 0);
  std::string newTupDecl = "[";
  for(int i = 0 ; i < arity ; i++){
    std::string iter = iters.front();
    iters.pop();
    if( i == inArity ){
      newTupDecl = newTupDecl + "] -> [";
    } else if( i != 0 ){
      newTupDecl = newTupDecl + ",";      
    }
    newTupDecl = newTupDecl + iter;
  }
  newTupDecl = newTupDecl + "]";

  setParts.tupDecl = newTupDecl;

  return getFullStrFromParts(setParts);
}

/*! This function turns an IEGenLib Relation represented in string form
**  into an IEGenLib Set string. It reverses setStr2RelationStr effects.
*/
std::string relationStr2SetStr(std::string relation, int inArity, int outArity){

  int arity = inArity+outArity;
  srParts relationParts;
  relationParts = getPartsFromStr(relation);

  std::queue<std::string> iters;
  iters = tupVarsExtract(relationParts.tupDecl, inArity, outArity);
  std::string newTupDecl = "[";
  for(int i = 0 ; i < arity ; i++){
    std::string iter = iters.front();
    iters.pop();
    if( i != 0 ){
      newTupDecl = newTupDecl + ",";      
    }
    newTupDecl = newTupDecl + iter;
  }
  newTupDecl = newTupDecl + "]";

  relationParts.tupDecl = newTupDecl;

  return getFullStrFromParts(relationParts);
}


/*! This function adds constraints from a Set/Relation string to another one
**  without checking whether their tuple declaration match or not. 
**  Note that Intersect functionality of IEGenLib that can be used for similar 
**  purposes checks for matching tuple declaration.
*/
std::string strAddConstraints(std::string dest, std::string src){
  srParts destParts = getPartsFromStr(dest);
  srParts srcParts = getPartsFromStr(src);
  destParts.constraints = srcParts.constraints + " && " + destParts.constraints;
  return getFullStrFromParts(destParts);
}

}// iegenlib namespace
