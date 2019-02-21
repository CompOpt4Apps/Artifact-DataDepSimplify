#ifndef OMEGALIB_HH
#define OMEGALIB_HH

#include <iostream>
#include <fstream>
#include <locale>
#include <map>
#include <sstream>
#include <string>

#include <util/Lists.hh>
#include <util/Strings.hh>

#include <basic/Dynamic_Array.h>
#include <basic/Iterator.h>
#include <omega/parser/AST.hh>
#include <omega/hull.h>
#include <omega/closure.h>
#include <omega/reach.h>
#include <codegen.h>
#include <omega/parser/parser.tab.hh>

using namespace omega;

#define ASN_OP ":="
#define PROMPT ">>>"

#define IN(cont,item) (find((cont).begin(),(cont).end(),(item))!=(cont).end())
#define LEN(cont) ((unsigned)(cont).size())

int omega_run(std::istream *is, std::ostream *os);

namespace {
struct UninterpFunc {
    string name;
    vector<string> args;
    unsigned arity;
    unsigned order;
    string oldName;
    vector<string> oldArgs;
    int oldArity;

    explicit UninterpFunc(const string& name = "", unsigned arity = 1, unsigned order = 0) {
        this->name = name;
        this->arity = arity;
        this->order = order;
        this->oldName = "";
        this->oldArity = 0;
    }

    UninterpFunc(const UninterpFunc& other) {
        this->name = other.name;
        this->args = other.args;
        this->arity = other.arity;
        this->order = other.order;
        this->oldName = other.oldName;
        this->oldArgs = other.oldArgs;
        this->oldArity = other.oldArity;
    }

    unsigned num_args() const {
        return this->args.size();
    }

    void add_arg(const string& arg) {
        this->args.push_back(arg);
    }

    string to_string() const {
        ostringstream oss;
        oss << this->name << "(" << this->arity << ") -> (";
        int count = 0;
        int last = this->args.size() - 1;
        for (const string& arg : this->args) {
            oss << arg;
            if (count < last) {
                oss << ",";
            }
            count += 1;
        }
        oss << ")";
        return oss.str();
    }
};

//struct OmegaLib : public PolyLib {
struct OmegaLib {
private:
    vector<string> _kwords = {"exists", "union", "intersection", "complement", "compose", "inverse",
                              "domain", "range", "hull", "codegen", "farkas", "forall", "given", "and",
                              "or", "not", "within", "subsetof", "supersetof", "symbolic"};

    map<string, UninterpFunc> _ufuncs;

    bool isknown(const string& cond, const vector<string>& iters, const vector<string>& exists) {
        // TODO: Simplify this logic!!!
        bool known = false;
        vector<char> rel_ops = {'>', '<', '='};
        vector<char> arith_ops = {'+', '-', '*', '/'};
        for (const string& operand : Strings::split(cond, rel_ops)) {
            known = !IN(iters, operand) && !IN(exists, operand);
            if (!known) {
                break;
            }
        }

        if (known) {
            for (const string& kword : _kwords) {
                known = !Strings::in(cond, kword);
                if (!known) {
                    break;
                }
            }
        }

        if (known) {
            for (const string& operand : Strings::split(cond, arith_ops)) {
                known = !IN(iters, operand) && !IN(exists, operand);
                if (!known) {
                    break;
                }
            }
        }

        return known;
    }

public:
    //string codegen(const string& code) override {
    string codegen(const string& relation, const string& id = "I") {
        string relId, relStr;
        map<string, UninterpFunc> ufuncs;
        ostringstream oss;

        size_t pos = relation.find(ASN_OP);
        if (pos != string::npos) {
            relId = Strings::rtrim(relation.substr(0, pos - 1));
            relStr = Strings::ltrim(relation.substr(pos + 2));
        } else {
            relId = id;
            relStr = relation;
        }

        string result = Strings::removeWhitespace(relStr);
        pos = result.find(':');

        string iterStr = result.substr(2, pos - 3);
        vector<string> iters = Strings::split(iterStr, ',');

        string condStr = Strings::rtrim(result.substr(pos + 1), '}');
        vector<string> conds = Strings::split(condStr, "&&");

        vector<string> exists;
        vector<string> knowns;
        map<string, size_t> symcons;

        // Collect conditions and uninterpreted functions...
        for (const string& cond : conds) {
            pos = cond.find("exists(");
            if (pos != string::npos) {
                vector<string> evars = Strings::split(cond.substr(pos + 7, cond.find(':', pos + 7)), ',');
                for (const string& evar : evars) {
                    exists.push_back(evar);
                }
            }

            vector<string> items = Strings::words(cond);
            for (const string& item : items) {
                string lcitem = Strings::lower(item);
                if (!IN(iters, item) && !IN(_kwords, lcitem)) {
                    if (Strings::in(result, item + '(') && ufuncs.find(item) == ufuncs.end()) {
                        UninterpFunc ufunc(item, 1, ufuncs.size());
                        ufuncs[item] = ufunc;
                    }
                    if (symcons.find(item) == symcons.end() && ufuncs.find(item) == ufuncs.end() &&
                        find(exists.begin(), exists.end(), item) == exists.end()) {
                        symcons[item] = symcons.size();
                    }
                }
            }
            if (isknown(cond, iters, exists)) {
                knowns.push_back(cond);
            }
        }

        // Separate set string for codegen argument from those for given clause.
        string given;
        if (LEN(knowns) > 0) {
            for (const string& known : knowns) {
                conds.erase(find(conds.begin(), conds.end(), known));
            }
            result = Strings::replace(result, condStr, Strings::join(conds, "&&"));
            given = "{[" + Strings::join(iters, ",") + "]: " + Strings::join(knowns, "&&") + "}";
        }

        vector<string> symkeys = Lists::keys<string, size_t>(symcons);
        string symlist = Strings::join(symkeys, ",") + ",";

        // U-funcs are tricky, need to consider the args...
        // 1st Pass: Collect all data on u-funcs.
        map<string, UninterpFunc> newfuncs;
        for (auto ufpair : ufuncs) {
            string ufname = ufpair.first;
            UninterpFunc ufunc = ufpair.second;
            size_t fpos = result.find(ufname);
            while (fpos >= 0 && fpos != string::npos) {
                fpos += LEN(ufname);
                size_t lpos = result.find('(', fpos);
                size_t rpos = result.find(')', lpos + 1);
                string sub = result.substr(lpos + 1, rpos - lpos - 1);
                vector<string> args = Strings::split(sub, ',');
                ufunc.arity = LEN(args);
                for (unsigned i = 0; i < ufunc.arity; i++) {
                    string arg = args[i];
                    if (ufunc.num_args() <= i) {  // New arg!
                        ufunc.add_arg(arg);
                    } else if (arg != ufunc.args[i]) {
                        UninterpFunc newfunc = UninterpFunc(ufunc);
                        newfunc.oldName = ufunc.name;
                        if (Strings::isDigit(newfunc.name[newfunc.name.size() - 1])) {
                            newfunc.name += "_";
                        }
                        newfunc.name += to_string(i + 1);
                        newfunc.oldArgs.push_back(arg);
                        newfuncs[newfunc.name] = newfunc;
                    }
                }
                fpos = result.find(ufname, rpos + 1);
            }
            newfuncs[ufname] = ufunc;
        }

        ufuncs = newfuncs;
        newfuncs.clear();

        // 2nd Pass: To prevent prefix errors, need to ensure leading arg includes first iters...
        for (auto ufpair : ufuncs) {
            UninterpFunc ufunc = ufpair.second;
            if (ufunc.arity > 0) {
                ufunc.oldArity = ufunc.arity;
                vector<string> newargs;
                for (const string& arg : ufunc.args) {
                    if (IN(iters, arg)) {
                        newargs = Lists::slice<string>(iters, 0, Lists::index<string>(iters, arg) - 1);
                        break;
                    }
                }
                ufunc.arity += LEN(newargs);
                if (LEN(ufunc.oldArgs) < 1) {
                    ufunc.oldArgs = ufunc.args;
                }
                for (const string& arg : newargs) {
                    ufunc.args.insert(ufunc.args.end() - 1, arg);
                }
            }
            symlist += ufunc.name + '(' + to_string(ufunc.arity) + "),";
            newfuncs[ufpair.first] = ufunc;
        }

        ufuncs = newfuncs;

        //  3rd Pass: Replace the occurrences of the UFs in the original relation...
        for (auto ufpair : ufuncs) {
            string ufname = ufpair.first;
            UninterpFunc ufunc = ufpair.second;
            string oldcall;
            if (!ufunc.oldName.empty()) {
                oldcall = ufunc.oldName + '(' + Strings::join(ufunc.oldArgs, ",") + ')';
            } else if (ufunc.arity > ufunc.oldArity) {
                oldcall = ufname + '(' + Strings::join(ufunc.oldArgs, ",") + ')';
            }
            if (!oldcall.empty()) {
                string newcall = ufname + '(' + Strings::join(ufunc.args, ",") + ')';
                if (oldcall != newcall) {
                    result = Strings::replace(result, oldcall, newcall);
                    given = Strings::replace(given, oldcall, newcall);
                }
            }
        }

        oss << "symbolic " << Strings::rtrim(symlist, ',') << ";\n";
        oss << relId << " := " << result << ";\n";
        oss << "codegen(" << relId << ")";
        if (!given.empty()) {
            oss << " given " << given;
        }
        oss << ";\n";

        _ufuncs = ufuncs;
        cerr << oss.str();

        return parse(oss.str());
    }

    string parse(const string& code) {
        istringstream iss(code);
        ostringstream oss;
        omega_run(&iss, &oss);

        vector<string> lines = Strings::filter(Strings::split(oss.str(), '\n'), PROMPT, true);
        return Strings::join(lines, "\n");
    }

    map<string, string> macros() {
        map<string, string> macros;
        for (auto itr = _ufuncs.begin(); itr != _ufuncs.end(); ++itr) {
            UninterpFunc ufunc = itr->second;
            string ufName = ufunc.name;
            string arrName = ufName;
            if (!ufunc.oldName.empty())
                arrName = ufunc.oldName;
            string lhs = ufName + "(" + Strings::join(ufunc.args, ",") + ")";
            string rhs = arrName + "[(" + Strings::join(ufunc.oldArgs, "),(") + ")]";
            macros[lhs] = rhs;
        }
        return macros;
    }
};
}

#endif  // OMEGALIB_HH
