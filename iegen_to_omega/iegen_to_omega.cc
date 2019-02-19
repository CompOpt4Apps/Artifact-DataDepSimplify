#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <poly/PolyLib.hh>
#include <util/Strings.hh>

using std::cout;
using std::string;
using std::vector;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "usage: iegen_to_omega <iegen_set_str> <statement|dag_tuple>\n";
    } else {
        string setstr = string(argv[1]);
        string statement;
        if (argc > 2) {
            statement = string(argv[2]);
        }

        if (statement[0] == '(') {
            string tuple = Strings::rtrim(Strings::ltrim(statement, '('), ')');
            vector<string> elems = Strings::split(tuple, ',');
            statement = "DAG[#" + elems[0] + "].push_back(#" + elems[1] + ")";
        }

        PolyLib poly;
        poly.add(setstr, "Iset");
        cout << poly.codegen("Iset", "int", "auto", true, {statement});
    }
}
