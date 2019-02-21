#ifndef POLYLIB_HH
#define POLYLIB_HH

#include <string>
#include <poly/iegenlib/IEGenLib.hh>
#include <poly/omega/OmegaLib.hh>

using std::string;

namespace {
    struct PolyLib {
    private:
        IEGenLib _iegen;
        OmegaLib _omega;

    public:
        string add(const string &constStr, const string &name = "") {
            string result;
            if (constStr.find(":=") == string::npos && !name.empty()) {
                result = _iegen.add(name + " := " + constStr);
            } else {
                result = _iegen.add(constStr);
            }
            return result;
        }

        string get(const string &name) {
            return _iegen.get(name);
        }

        string newSet(const string &setStr) {
            return _iegen.newSet(setStr);
        }

        string newRelation(const string &relStr) {
            return _iegen.newRelation(relStr);
        }

        string apply(const string& relName, const string& setName, const string& resName = "") {
            return _iegen.apply(relName, setName, resName);
        }

        string domain(const string &name) {
            return _iegen.domain(name);
        }

        string inverse(const string &relName) {
            return _iegen.inverse(relName);
        }

        string union_(const string &lhsName, const string &rhsName) {
            return _iegen.union_(lhsName, rhsName);
        }

        string compose(const string &lhsName, const string &rhsName) {
            return _iegen.compose(lhsName, rhsName);
        }

        string codegen(const string &setName) {
            string setStr = _iegen.get(setName);
            if (setStr.find("ERROR") == string::npos) {
                return _omega.codegen(setStr, setName);
            } else {
                return "ERROR: Set '" + setName + "' does not exist in 'codegen'.";
            }
        }

        string codegen(const string& setName, const string& iterType, const string& ompSched = "",
                       bool defineMacros = false, const vector<string>& statements = {}) {
            string code = codegen(setName);
            if (code.find("ERROR") == string::npos) {
                unsigned nIters = 0;
                string inIters;
                string outIters;
                string defines;

                //if (!iterType.empty()) {
                    string setStr = _iegen.get(setName);
                    size_t pos = setStr.find('[');
                    for (pos++; pos < setStr.length() && setStr[pos] != ']'; pos++) {
                        char setChr = setStr[pos];
                        if (setChr == ',') {
                            nIters++;
                        }
                        if (setChr != ' ') {
                            inIters += setChr;
                        }
                    }
                    if (nIters > 0) {
                        for (unsigned i = 0; i <= nIters; i++) {
                            outIters += "t" + to_string(i + 1);
                            if (i < nIters) {
                                outIters += ",";
                            }
                        }
                    }
                //}

                if (!ompSched.empty()) {
                    string pragma = "#pragma omp parallel for schedule(" + ompSched + ")";
                    if (!outIters.empty()) {
                        pragma += " private(" + outIters + ")";
                    }
                    code = pragma + "\n" + code;
                }

                if (!iterType.empty()) {
                    code = iterType + " " + outIters + ";\n" + code;
                }

                if (!statements.empty()) {
                    for (size_t i = 0; i < statements.size(); i++) {
                        defines += "#undef s" + to_string(i) + "\n";
                    }

                    vector<string> iterList = Strings::split("" + inIters, ",");
                    for (size_t i = 0; i < statements.size(); i++) {
                        string statement = statements[i];
                        for (unsigned n = 0; n <= nIters; n++) {
                            string pattern = "#" + to_string(n+1);
                            statement = Strings::replace(statement, pattern, iterList[n]);
                        }

                        for (const string& itr : iterList) {
                            statement = Strings::replace(statement, itr, "(" + itr + ")");
                        }
                        defines += "#define s" + to_string(i) + "(" + inIters + ") " + statement + "\n";
                    }
                    code = defines + "\n" + code;
                }

                if (defineMacros) {
                    defines = "";
                    map<string, string> macros = _omega.macros();
                    for (auto itr = macros.begin(); itr != macros.end(); ++itr) {
                        defines += "#define " + itr->first + " " + itr->second + "\n";
                    }
                    code = defines + "\n" + code;
                }
            }

            return code;
        }
    };
}

#endif  // POLYLIB_HPP
