#if ! defined _pres_decl_h
#define _pres_decl_h 1

#include <omega/pres_var.h>
#include <omega/pres_form.h>
#include <basic/Section.h>

namespace omega {

/**
 * @brief Abstract base class that can contain variable declarations
 */
class F_Declaration : public Formula {
public:
  //! add a new variable to this declaration node
  virtual Variable_ID declare(Const_String s)=0;
  virtual Variable_ID declare()=0;
  virtual Variable_ID declare(Variable_ID)=0;
  virtual Section<Variable_ID> declare_tuple(int size);

  void finalize();

  inline Variable_ID_Tuple &locals() {return myLocals;}

protected:
  F_Declaration(Formula *, Rel_Body *);
  F_Declaration(Formula *, Rel_Body *, Variable_ID_Tuple &);
  ~F_Declaration();

  Variable_ID do_declare(Const_String s, Var_Kind var_kind);

  void prefix_print(FILE *output_file, int debug = 1);
  void print(FILE *output_file);

  void setup_names();
  void setup_anonymous_wildcard_names();

  Variable_ID_Tuple myLocals;
  friend class F_Forall; // rearrange needs to access myLocals
  friend class F_Or;     // push_exists

private:
  virtual bool can_add_child();

  int priority();

  friend void align(Rel_Body *originalr, Rel_Body *newr, F_Exists *fe,
                    Formula *f, const Mapping &mapping, bool &newrIsSet,
                    List<int> &seen_exists, 
                    Variable_ID_Tuple &seen_exists_ids);
};

} // namespace

#endif
