#ifndef IEGENLIB_HH
#define IEGENLIB_HH

 /*!
 *\mainpage IEGen C++ Rewrite
 *\authors Michelle Strout
 *\authors Joe Strout
 *\authors Alan LaMielle
 *\authors Catherine Olschanowsky
 *\authors Barbara Kreaseck
 *\authors Mark Heim
 *\authors Ian Craig
 *\authors Nicholas Jeanette
 *\date Date Started: 5/17/10
 *
 *
 *\section Intro
 *  IEGenLib is a library with data structures and routines that
 *  can represent, parse, and visit integer tuple sets and relations with affine
 *  constraints and uninterpreted function symbol equality constraints. See
 *  src/parser/parser_test.cc for examples of strings that can be parsed.
 *
 *  The run_tests.cc driver will execute all tests identified with the TEST
 *  macro. In each of the src/ subdirectories, the *_test.cc files define
 *  regression tests.
 *
 *  The iegenlib_calc binary will execute tests that you input yourself and will
 *  return the interpreted version of the input along with a dotty file to the
 *  file set_relation.dot in the directory where the program is executed.
 *
 *

 *\section COPYING
 *\verbinclude COPYING

 *\section README
 *\verbinclude README

 */

/*
 * Copyright (c) 2009, 2010, 2011, 2012, 2013 Colorado State University <br>
 * All rights reserved. <br>
 * See COPYING for details. <br>
 $Revision:: 805                $: last committed revision\n
 $Date:: 2013-09-09 03:27:10 -0#$: date of last committed revision\n
 $Author:: mstrout              $: author of last committed revision\n
*/

#include <iostream>
#include <locale>
#include <map>
#include <sstream>
#include <iegenlib.h>
#include <util/Strings.hpp>

//using iegenlib::Set;
//using iegenlib::Relation;
using std::cerr;
using std::string;

//#define THROW_EXCEPTIONS 1

#define ASN_OP ":="
#define RLT_OP "->"

namespace {
struct IEGenLib {
private:
    map<string, iegenlib::Set*> _sets;
    map<string, iegenlib::Relation*> _relations;
    map<string, string> _setStrings;
    map<string, string> _relStrings;

    /*! lowercase
 *
 *  Converts the given string to lowercase.
 *
 * @param str -- string to convert
 * @return lowercase version of str
 */
    std::string lowercase(const std::string& str) {
        std::string lower;
        for (int ch : str) {
            lower += (char) tolower(ch);
        }
        return lower;
    }

/*! contains
 *
 *  This is a small helper that reports whether one string contains a
 *  substring of interest.  It is preferred over directly using the
 *  s.find() != string::npos idiom because (1) it is shorter, and (2)
 *  it more clearly expresses the intent to the human reader.  Also,
 *  this method can now act in a case-insensitive manner, which find
 *  doesn't naturally do.
 *
 * @param s -- string to search
 * @param substring -- substring to search for
 * @param caseSensitive -- if true, do a case-sensitive search (defaults to false)
 * @return true if s contains substring, false otherwise
 */
    bool contains(std::string s, std::string substring, bool caseSensitive=false) {
        if (not caseSensitive) {
            s = lowercase(s);
            substring = lowercase(substring);
        }
        return s.find(substring) != std::string::npos;
    }

/*! isRelation
 *
 *  Determine whether the given input is a relation, by searching
 *  for the arrow token ("->"), which appears only in a relation
 *  (and never in a set).
 *
 * @param input -- string to examine
 * @return true iff input appears to be a relation
 */
    bool isRelation(const std::string& input) {
        return contains(input, RLT_OP);
    }

/*! preprocess
 *
 * Preprocess set or relation string to remove nested uninterpreted function references if possible.
 *
 * @param input -- string to examine
 * @return Preprocessed output string
 */
    string preprocess(const string& input) {
        string output = input;
        if (output.find("))") != string::npos) {
            size_t stpos = output.rfind(':');
            string sub = output.substr(stpos + 1);
            sub = Strings::trim(Strings::rtrim(sub, '}'));
            vector<string> conds = Strings::split(sub, "&&");

            // 1) 1st pass: identify assignments...
            map<string, string> assigns;
            map<string, size_t> locs;
            string lhs, rhs;
            size_t loc;

            for (const string& cond : conds) {
                // Check for UF and equality statement
                if (cond.find('(') != string::npos && cond.find(" = ") != string::npos) {
                    loc = output.find(cond);
                    vector<string> elems = Strings::split(cond, " = ");
                    lhs = elems[0];
                    rhs = elems[1];

                    // Which one is the function, right or left?
                    if (lhs.find('(') != string::npos) {
                        string swap = lhs;
                        lhs = rhs;
                        rhs = swap;
                    }
                    assigns[lhs] = rhs;
                    locs[lhs] = loc;
                }
            }

            // 2) 2nd pass: replace assignments within conditions
            for (auto itr = assigns.begin(); itr != assigns.end(); ++itr) {
                lhs = itr->first;
                rhs = itr->second;
                loc = locs[lhs];

                size_t pos = output.find(rhs, stpos + 1);
                if (pos <= loc) {
                    pos = output.find(rhs, loc + 1);
                }

                while((pos = output.find(rhs, pos)) != string::npos) {
                    output.replace(pos, rhs.length(), lhs);
                    pos += lhs.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
                }
            }
        }

        return output;
    }

public:
    ~IEGenLib() {
        for (auto& keyVal : _sets) {
            delete keyVal.second;
        }
        for (auto& keyVal : _relations) {
            delete keyVal.second;
        }
    }

    /*! add
     *  Add a relation or set.
     */
    string add(const string& constStr) {
        string newStr = preprocess(constStr);
        if (isRelation(newStr)) {
            return newRelation(newStr);
        } else {
            return newSet(newStr);
        }
    }

    /*! get
     *  Get a relation or set.
     */
    string get(const string& name) {
        std::ostringstream oss;
        auto itr = _relations.find(name);
        if (itr != _relations.end()) {
            iegenlib::Relation *relation = itr->second;
            oss << relation->prettyPrintString();
        } else {
            auto its = _sets.find(name);
            if (its != _sets.end()) {
                iegenlib::Set *set = its->second;
                oss << set->prettyPrintString();
            } else {
                auto ite = _relStrings.find(name);
                if (ite != _relStrings.end()) {
                    oss << ite->second;
                } else {
                    ite = _setStrings.find(name);
                    if (ite != _setStrings.end()) {
                        oss << ite->second;
                    } else {
                        oss << "ERROR: Set or relation '" << name << "' not defined in 'get'!";
                    }
                }
            }
        }
        return oss.str();
    }

    /*! newSet
    *  Create a new set.
    */
    string newSet(const string& setStr) {
        string name;
        string setDef = setStr;
        size_t pos = setDef.find(ASN_OP);
        if (pos != string::npos) {
            name = Strings::rtrim(setStr.substr(0, pos));
            setDef = Strings::ltrim(setDef.substr(pos + 2));
        }

        //Create a set from the given set string
        iegenlib::Set* set = new iegenlib::Set(setDef);
        std::ostringstream oss;
        oss << set->prettyPrintString();
        string out = oss.str();

        if (name.empty()) {
            delete set;                                 // Delete the allocated set
        } else {
            _sets[name] = set;                   // Save set in the map
            out = name + " " + ASN_OP + " " + out;
        }

        return out;
    }

    /*! newRelation
    *  Create a new relation.
    */
    string newRelation(const string& relStr) {
        string name;
        string relDef = relStr;
        size_t pos = relDef.find(ASN_OP);
        if (pos != string::npos) {
            name = Strings::rtrim(relDef.substr(0, pos));
            relDef = Strings::ltrim(relDef.substr(pos + 2));
        }

        // Create a relation from the given relation string
        iegenlib::Relation* relation = nullptr;
#ifdef THROW_EXCEPTIONS
        try {
            relation = new iegenlib::Relation(relDef);
        } catch (iegenlib::parse_exception& ex) {
            cerr << "ERROR: " << ex.what() << "\n";
        }
#else
        relation = new iegenlib::Relation(relDef);
#endif

        string out;
        if (relation != nullptr) {
            std::ostringstream oss;
            oss << relation->prettyPrintString();
            out = oss.str();
            if (name.empty()) {
                delete relation;                        // Delete the allocated relation
            } else {
                _relations[name] = relation;            // Save relation in the map
                out = name + " " + ASN_OP + " " + out;
            }
        } else {
            _relStrings[name] = relDef;
            out = relStr;
        }

        return out;
    }

    /*! apply
     *  Apply a relation to a set.
     */
    string apply(const string& relName, const string& setName, const string& resName = "") {
        std::ostringstream oss;
        string relStr;
        iegenlib::Relation* relation = nullptr;
        iegenlib::Set* set = nullptr;

        auto itr = _relations.find(relName);
        if (itr != _relations.end()) {
            relation = itr->second;
        } else {
            auto ite = _relStrings.find(relName);
            if (ite != _relStrings.end()) {
                relStr = ite->second;
            } else {
                oss << "ERROR: Relation '" << relName << "' not defined in 'apply'!";
            }
        }

        auto its = _sets.find(setName);
        if (its != _sets.end()) {
            set = its->second;
        } else {
            oss << "ERROR: Set '" << setName << "' not defined in 'apply'!";
        }

        string newName = resName;
        if (newName.empty()) {
            newName = relName + "*" + setName;
        }

        if (set != nullptr) {
            if (relation != nullptr) {
                iegenlib::Set* newSet = relation->Apply(set);
                oss << newSet->prettyPrintString();
                _sets[newName] = newSet;
            } else {
                // "Ttile := {[i,k,j]  -> [ii,kk,i,k,j]: exists(ri,ck: 0 <= ri < 8 && i = ii*8+ri && 0 <= ck < 8 && k = kk*8+ck)}"
                // "Idense := { [i, j, k] : k - col(j) = 0 && i >= 0 && k >= 0 && col(j) >= 0 && index(i) >= 0 && j - index(i) >= 0 && NNZ - index(i + 1) >= 0 && NNZ - 1 >= 0 && N_C - 1 >= 0 && -i + N_R - 1 >= 0 && -j + index(i + 1) - 1 >= 0 && -k + N_C - 1 >= 0 && N_C - col(j) - 1 >= 0 }"
                size_t lpos = relStr.find(RLT_OP);
                size_t cpos = relStr.find(":", lpos + 1);
                string iters = Strings::trim(relStr.substr(lpos + 2, cpos - lpos - 2));
                string rconds = Strings::ltrim(relStr.substr(cpos + 1));
                rconds = Strings::rtrim(rconds, '}');

                string setStr = set->prettyPrintString();
                cpos = setStr.find(":");
                string sconds = Strings::ltrim(setStr.substr(cpos + 1));
                sconds = Strings::trim(Strings::rtrim(sconds, '}'));

                string newSetStr = "{" + iters + ": " + rconds + " && " + sconds + "}";
                oss << newSetStr;
                _setStrings[newName] = newSetStr;
            }
        }

        return oss.str();
    }

    /*! domain
     * Return the domain of a set or relation.
     */
    string domain(const string& name) {
        std::ostringstream oss;
        auto its = _sets.find(name);
        if (its != _sets.end()) {
            iegenlib::Set* set = its->second;
            oss << set->prettyPrintString();
        } else {
            auto itr = _relations.find(name);
            if (itr != _relations.end()) {
                iegenlib::Relation *rel = itr->second;
                oss << rel->prettyPrintString();
            } else {
                oss << "ERROR: Name '" << name << "' is not defined in 'domain'!";
            }
        }
        return oss.str();
    }

    /*! inverse
     * Perform inverse of a relation.
     */
    string inverse(const string& relName) {
        std::ostringstream oss;
        auto itr = _relations.find(relName);
        if (itr != _relations.end()) {
            iegenlib::Relation* relation = itr->second;
            if (relation) {
                iegenlib::Relation* inv = relation->Inverse();
                oss << inv->prettyPrintString();
                string resName = relName + "_inv";
                _relations[resName] =  inv;
                //delete relation;
            } else {
                oss << "ERROR: Inverse requires one relation in 'inverse'!";
            }
        } else {
            oss << "ERROR: Relation '" << relName << "' not defined in 'inverse'!";
        }
        return oss.str();
    }

    /*! union_
     * Perform union of two sets or relations.
     */
    string union_(const string& lhsName, const string& rhsName) {
        std::ostringstream oss;
        string resName = lhsName + "U" + rhsName;

        auto lsi = _sets.find(lhsName);
        auto rsi = _sets.find(rhsName);
        if (lsi != _sets.end() && rsi != _sets.end()) {
            iegenlib::Set* lhs = lsi->second;
            iegenlib::Set* rhs = rsi->second;
            iegenlib::Set* uset = lhs->Union(rhs);
            oss << uset->prettyPrintString();
            _sets[resName] = uset;
        } else {
            auto lri = _relations.find(lhsName);
            auto rri = _relations.find(rhsName);
            if (lri != _relations.end() && rri != _relations.end()) {
                iegenlib::Relation* lhs = lri->second;
                iegenlib::Relation* rhs = rri->second;
                iegenlib::Relation* urel = lhs->Union(rhs);
                oss << urel->prettyPrintString();
                _relations[resName] = urel;
            } else {
                oss << "ERROR: Set/relation union mismatch between '" << lhsName
                    << "' and '" << rhsName << "' in 'union'.";
            }
        }

        return oss.str();
    }

    /*! compose
     * Compose two relations.
     */
    string compose(const string& lhsName, const string& rhsName) {
        std::ostringstream oss;
        auto lri = _relations.find(lhsName);
        auto rri = _relations.find(rhsName);
        if (lri != _relations.end() && rri != _relations.end()) {
            string compName = lhsName + "o" + rhsName;
            iegenlib::Relation* lhs = lri->second;
            iegenlib::Relation* rhs = rri->second;
            iegenlib::Relation* comp = lhs->Compose(rhs);
            oss << comp->prettyPrintString();
            _relations[compName] = comp;
        } else {
            oss << "ERROR: Relation '" << lhsName << "' and/or '"
                << rhsName << "' not defined in 'compose'.";
        }
        return oss.str();
    }

    string codegen(const string& setName) {
        return "ERROR: Code generation is not supported in IEGenLib::codegenSet.";
    }
};
}

#endif    // IEGENLIB_HH
