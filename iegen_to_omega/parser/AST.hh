#ifndef _AST_H
#define _AST_H

#include <assert.h>
#include <omega.h>
#include <map>
#include <set>

typedef enum {eq, lt, gt, geq, leq, neq} Rel_Op;

//namespace ast {
    class tupleDescriptor;

    class Variable_Ref {
    public:
        int anonymous;
        omega::Const_String name;
        omega::Const_String stripped_name;
        omega::Variable_ID vid;

        omega::Variable_ID id(omega::Rel_Body *R) {
            if (vid) return vid;
            if (arity == 0)
                vid = R->get_local(g);
            else
                vid = R->get_local(g, of);
            return vid;
        }

        omega::Variable_ID id(omega::Relation &R) {
            if (vid) return vid;
            if (arity == 0)
                vid = R.get_local(g);
            else
                vid = R.get_local(g, of);
            return vid;
        }

        omega::Free_Var_Decl *g;
        int arity;
        int pos;
        omega::Argument_Tuple of;

        Variable_Ref(char *s, int _arity, omega::Argument_Tuple _of);

        Variable_Ref(char *s);

        Variable_Ref();

        ~Variable_Ref();
    };

    extern std::map<omega::Const_String, Variable_Ref *> functionOfInput;
    extern std::map<omega::Const_String, Variable_Ref *> functionOfOutput;

    class Declaration_Site {
    public:
        Declaration_Site();

        Declaration_Site(std::set<char *> *v);

        virtual Variable_Ref *extend(char *s);

        virtual Variable_Ref *extend(char *s, omega::Argument_Tuple, int);

        virtual ~Declaration_Site();

        Variable_Ref *extend();

        void print() {
            for (std::set<Variable_Ref *>::iterator i = declarations.begin();;) {
                printf("%s", static_cast<const char *>((*i)->name));
                i++;
                if (i != declarations.end())
                    printf(",");
                else
                    break;
            }
        }

        Declaration_Site *previous;
        std::set<Variable_Ref *> declarations;
    };

    class Global_Declaration_Site : public Declaration_Site {
    public:
        virtual Variable_Ref *extend(char *s);

        virtual Variable_Ref *extend() {
            return Declaration_Site::extend();
        }

        virtual Variable_Ref *extend(char *s, omega::Argument_Tuple in_of, int in_arity) {
            return Declaration_Site::extend(s, in_of, in_arity);
        }

        void extend_both_tuples(char *s, int arity);

        virtual ~Global_Declaration_Site();
    };

    extern Declaration_Site *current_Declaration_Site;

    inline void popScope() {
        assert(current_Declaration_Site);
        current_Declaration_Site = current_Declaration_Site->previous;
    }

    Variable_Ref *lookupScalar(char *s);

    Declaration_Site *defined(char *);

    namespace ast {
        class Exp {
        public:
            Exp(omega::coef_t c);

            Exp(Variable_Ref *v);

            std::map<Variable_Ref *, omega::coef_t> coefs;
            omega::coef_t constantTerm;
        protected:
            void addTerm(omega::coef_t coef, omega::Variable_ID v);
        };
    }

    typedef struct {
        ast::Exp *e;
        int step;
    } strideConstraint;

ast::Exp *multiply(omega::coef_t c, ast::Exp *x);

ast::Exp *multiply(ast::Exp *x, ast::Exp *y);

ast::Exp *negate(ast::Exp *x);

ast::Exp *add(ast::Exp *x, ast::Exp *y);

ast::Exp *subtract(ast::Exp *x, ast::Exp *y);

    class AST;

    class AST_constraints;


    class Tuple_Part {
    public:
        Variable_Ref *from, *to;

        Tuple_Part(Variable_Ref *f, Variable_Ref *t) {
            from = f;
            to = t;
        }

        Tuple_Part(Variable_Ref *f) {
            from = f;
            to = 0;
        }

        Tuple_Part() {
            from = 0;
            to = 0;
        }
    };


    class AST {
    public:
        virtual void install(omega::Formula *F) = 0;

        virtual void print() = 0;

        virtual ~AST() {}
    };


    class AST_And : public AST {
    public:
        AST_And(AST *l, AST *r) {
            left = l;
            right = r;
        }

        ~AST_And() {
            delete left;
            delete right;
        }

        virtual void install(omega::Formula *F);

        virtual void print() {
            printf("(");
            left->print();
            printf(" && ");
            right->print();
            printf(")");
        }

        AST *left, *right;
    };


    class AST_Or : public AST {
    public:
        AST_Or(AST *l, AST *r) {
            left = l;
            right = r;
        }

        ~AST_Or() {
            delete left;
            delete right;
        }

        virtual void install(omega::Formula *F);

        virtual void print() {
            printf("(");
            left->print();
            printf(" || ");
            right->print();
            printf(")");
        }


        AST *left, *right;
    };


    class AST_Not : public AST {
    public:
        AST_Not(AST *c) { child = c; }

        ~AST_Not() { delete child; }

        virtual void install(omega::Formula *F);

        virtual void print() {
            printf("(!");
            child->print();
            printf(")");
        }

        AST *child;
    };


    class AST_declare : public AST {
    public:
        virtual void install(omega::Formula *F) = 0;

        virtual void print() {
            printf("(");
            declaredVariables->print();
            printf(" : ");
            child->print();
            printf(")");
        }

        Declaration_Site *declaredVariables;
        AST *child;
    };


    class AST_exists : public AST_declare {
    public:
        AST_exists(Declaration_Site *dV, AST *c) { declaredVariables = dV, child = c; }

        ~AST_exists() {
            delete child;
            delete declaredVariables;
        }

        virtual void install(omega::Formula *F);

        virtual void print() {
            printf("exists ");
            AST_declare::print();
        }
    };


    class AST_forall : public AST_declare {
    public:
        AST_forall(Declaration_Site *dV, AST *c) { declaredVariables = dV, child = c; }

        ~AST_forall() {
            delete child;
            delete declaredVariables;
        }

        virtual void install(omega::Formula *F);

        virtual void print() {
            printf("forall ");
            AST_declare::print();
        }
    };


    class AST_constraints : public AST {
    public:
        AST_constraints(std::set<ast::Exp *> *f, Rel_Op r, AST_constraints *o);

        AST_constraints(std::set<ast::Exp *> *f, Rel_Op r, std::set<ast::Exp *> *s);

        AST_constraints(std::set<ast::Exp *> *f);

        ~AST_constraints();

        virtual void install(omega::Formula *F);

        virtual void print();

        AST_constraints *others;
        std::set<ast::Exp *> *first;
        Rel_Op rel_op;
    };

    void install_stride(omega::F_And *F, strideConstraint *c);

    void install_eq(omega::F_And *F, ast::Exp *e1, ast::Exp *e2);

    void install_geq(omega::F_And *F, ast::Exp *e1, ast::Exp *e2);

    void install_gt(omega::F_And *F, ast::Exp *e1, ast::Exp *e2);

    void install_neq(omega::F_And *F, ast::Exp *e1, ast::Exp *e2);


    class tupleDescriptor {
    public:
        tupleDescriptor() { size = 0; }

        void extend();

        void extend(ast::Exp *e);

        void extend(ast::Exp *lb, ast::Exp *ub);

        void extend(ast::Exp *lb, ast::Exp *ub, omega::coef_t stride);

        void extend(char *s, ast::Exp *e);

        void extend(char *s);

        void extend(char *s, omega::Argument_Tuple, int);

        int size;
        std::vector<Variable_Ref *> vars;
        std::set<ast::Exp *> eq_constraints;
        std::set<ast::Exp *> geq_constraints;
        std::set<strideConstraint *> stride_constraints;

        ~tupleDescriptor() {
            for (std::set<ast::Exp *>::iterator i = eq_constraints.begin(); i != eq_constraints.end(); i++)
                delete *i;
            for (std::set<ast::Exp *>::iterator i = geq_constraints.begin(); i != geq_constraints.end(); i++)
                delete *i;
            for (std::set<strideConstraint *>::iterator i = stride_constraints.begin();
                 i != stride_constraints.end(); i++) {
                delete (*i)->e;
                delete *i;
            }
        }
    };

    extern Global_Declaration_Site *globalDecls;
    extern Declaration_Site *relationDecl;
    extern tupleDescriptor *currentTupleDescriptor;

    void resetGlobals();


// Used to parse a list of paired relations for code generation commands
// class RelTuplePair {
// public:
// RelTuplePair() : ispaces(0), mappings(0) {}
//   omega::Tuple<omega::Relation>  ispaces;
//   omega::Tuple<omega::Relation> mappings;
// };
//}

#endif 
