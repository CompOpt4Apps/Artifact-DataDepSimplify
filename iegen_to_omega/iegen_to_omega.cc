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
        PolyLib poly;
        string itype = "int";
        string result;

        unsigned i, j = 0;
        for (i = 1; i < argc; i += 2) {
            string expr = argv[i];
            string tuple = argv[i+1];
            string id = "Is" + to_string(++j);

            tuple = Strings::rtrim(Strings::ltrim(tuple, '('), ')');
            vector<string> elems = Strings::split(tuple, ',');
            string statement = "DAG[#" + elems[0] + "].push_back(#" + elems[1] + ")";

            result = poly.add(expr, id);
            result = poly.codegen(id, itype, "auto", (j < 2), {statement});
            cout << result << endl;
            itype = "";
        }
    }
}
