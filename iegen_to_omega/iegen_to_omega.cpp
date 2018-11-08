#include <iostream>
#include <cstring>
#include <string>
#include <Python.h>

using namespace std;

class CppPy {
public:
    CppPy() {
        Py_Initialize();
        _pymod = PyImport_AddModule("__main__");
    }

    virtual ~CppPy() {
        Py_Finalize();
    }

    string run(const string& code) {
        string header = "import sys\n\
class Redirect:\n\
    def __init__(self):\n\
        self.value = ''\n\
    def write(self, txt):\n\
        self.value += txt\n\
redirect = Redirect()\n\
sys.stdout = redirect\n\
sys.stderr = redirect\n";

        PyRun_SimpleString(header.c_str());
        PyRun_SimpleString(code.c_str());
        PyObject *redirect = PyObject_GetAttrString(_pymod, "redirect");
        PyObject *value = PyObject_GetAttrString(redirect, "value");
        PyErr_Print();  // Print any errors...
        const char *str = PyString_AsString(value);

        return string(str);
    }

private:
    PyObject *_pymod;
};

string toOmega(const string& iegenstr) {
    CppPy cpy;
    string pycode = "from iegen_to_omega import to_omega\nprint(to_omega('" + iegenstr + "'))\n";
    return cpy.run(pycode);
}

string codegen(const string& omcode, const string& omcalc) {
    const int bufflen = 1024;
    char buffer[bufflen];
    string data;

    FILE *out = fopen(".out", "w");
    fprintf(out, "%s", omcode.c_str());
    fclose(out);

    string cmd = omcalc + " < .out";
    FILE *proc = popen(cmd.c_str(), "r");
    if (proc) {
        while (!feof(proc)) {
            if (fgets(buffer, bufflen, proc) && !strstr(buffer, ">>>")) {
                data.append(buffer);
            }
        }
        pclose(proc);
    }

    return data;
}

int main(int argc, char *argv[]) {
    cout << codegen(toOmega(string(argv[1])), string(argv[2])) << endl;
}