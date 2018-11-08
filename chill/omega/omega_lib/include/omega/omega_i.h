#if ! defined _omega_i_h
#define _omega_i_h 1

#include <omega/pres_var.h>

namespace omega {

/* #define Assert(c,t) if(!(c)) PresErrAssert(t) */
/* void PresErrAssert(const char *t); */

extern Rel_Body null_rel;

extern int     skip_finalization_check;
// extern int     skip_set_checks;

// Global input and output variable tuples.

extern Global_Input_Output_Tuple input_vars;
extern Global_Input_Output_Tuple output_vars;
extern Global_Input_Output_Tuple &set_vars;

} // namespace

#if ! defined DONT_INCLUDE_TEMPLATE_CODE
// with g++258, everything will need to make Tuple<Relation>, as a
// function taking it as an argument is a friend of lots of classes
#include <omega/Relation.h>
#endif

#endif
