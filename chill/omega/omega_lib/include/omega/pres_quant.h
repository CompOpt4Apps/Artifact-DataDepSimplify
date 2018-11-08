#if ! defined _pres_quant_h
#define _pres_quant_h 1

#include <omega/pres_decl.h>

namespace omega {

//
// Presburger formula nodes for quantifiers
//

/**
 * @brief Existential qualifier
 *
 * This associates with one or more variables. These variables
 * are existentially qualified in the formula represented by
 * its single child node.
 */
class F_Exists : public F_Declaration {
public:
    inline Node_Type node_type() {return Op_Exists;}
    Variable_ID declare(Const_String s);
    Variable_ID declare();
    Variable_ID declare(Variable_ID v);
    virtual     void push_exists(Variable_ID_Tuple &S);

protected:
    friend class Formula;

    F_Exists(Formula *, Rel_Body *);
    F_Exists(Formula *, Rel_Body *, Variable_ID_Tuple &);

private:
    Formula *copy(Formula *parent, Rel_Body *reln);

    virtual Conjunct *find_available_conjunct();
    void print(FILE *output_file);
    void prefix_print(FILE *output_file, int debug = 1);
    void beautify();
    void rearrange();
    DNF* DNFize();
};

/**
 * @brief Universal qualifier
 *
 * This associates with one or more variables. These variables
 * are universally qualified in the formula represented
 * by its single child node.
 */
class F_Forall : public F_Declaration {
public:
    inline Node_Type node_type() {return Op_Forall;}
    Variable_ID declare(Const_String s);
    Variable_ID declare();
    Variable_ID declare(Variable_ID v);

protected:
    friend class Formula;

    F_Forall(Formula *, Rel_Body *);

private:
    Formula *copy(Formula *parent, Rel_Body *reln);

    virtual Conjunct *find_available_conjunct();
    void print(FILE *output_file);
    void prefix_print(FILE *output_file, int debug = 1);
    void beautify();
    void rearrange();
    DNF* DNFize();
};

} // namespace

#endif
