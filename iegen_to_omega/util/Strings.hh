#ifndef STRINGS_HH
#define STRINGS_HH

#include <algorithm>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

class Strings {
public:
    static inline bool in(const string& str, const string& sub) {
        return str.find(sub) != string::npos;
    }

    static vector<std::string> split(const string& str, char delim = ' ') {
        vector<string> tokens;
        string token;
        istringstream stream(str);
        while (getline(stream, token, delim)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    static vector<std::string> split(const string& str, const string& delim) {
        vector<string> tokens;
        char* cstr = (char*) str.c_str();
        char* cdelim = (char*) delim.c_str();
        char* token = strtok(cstr, cdelim);

        while (token != NULL) {
            tokens.push_back(string(token));
            token = strtok(NULL, cdelim);
        }
        return tokens;
    }

    static vector<std::string> split(const string& str, const vector<char>& delims) {
        string token;
	    vector<string> tokens;
        for (char ch : str) {
            if (find(delims.begin(), delims.end(), ch) == delims.end()) {
                token += ch;
            } else if (token.size() > 0) {
                tokens.push_back(token);
                token = "";
            }
        }
        if (token.size() > 0) {
            tokens.push_back(token);
        }
        return tokens;
    }

    static string join(const vector<string>& items, const string& delim = "\n") {
        ostringstream oss;
        if (!items.empty()) {
            size_t last = items.size() - 1;;
            for (size_t i = 0; i < last; i++) {
                oss << items[i] << delim;
            }
            oss << items[last];
        }
        return oss.str();
	}

    static vector<std::string> filter(const vector<string>& items, string pattern, bool negate = false) {
	    vector<string> matches;
	    if (!pattern.empty()) {
            for (string item : items) {
                bool match = in(item, pattern);
                if (negate) {
                    match = !match;
                }
                if (match) {
                    matches.push_back(item);
                }
            }
        }
	    return matches;
	}

    // trim from start
    static inline string ltrim(const string& str) {
        string copy = str;
        copy.erase(copy.begin(), find_if(copy.begin(), copy.end(), [](int ch) {
            return !isspace(ch);
        }));
        return copy;
    }

    // trim from start
    static inline string ltrim(const string& str, char tchar) {
	    size_t pos = 0;
        string trimmed;
        for (; pos < str.size() && str[pos] == tchar; pos += 1);
        if (pos < str.size()) {
            trimmed = str.substr(pos);
        } else {
            trimmed = str;
        }
        return trimmed;
    }

    // trim from end
    static inline string rtrim(const string &str) {
        string copy = str;
        copy.erase(find_if(copy.rbegin(), copy.rend(), [](int ch) {
            return !isspace(ch);
        }).base(), copy.end());
        return copy;
    }

    // trim from end
    static inline string rtrim(const string &str, char tchar) {
        size_t pos = str.size() - 1;
        string trimmed;
        for (; pos > 0 && str[pos] == tchar; pos -= 1);
        if (pos > 0) {
            trimmed = str.substr(0, pos + 1);
        } else {
            trimmed = str;
        }
        return trimmed;
    }

    // trim from both ends
    static inline string trim(const string &str) {
        string trimStr = ltrim(str);
        trimStr = rtrim(trimStr);
        return trimStr;
    }

    // trim from both ends
    static inline string trim(const string &str, char tchar) {
        string trimStr = ltrim(str, tchar);
        trimStr = rtrim(trimStr, tchar);
        return trimStr;
    }

    static inline string removeWhitespace(const string& in) {
	    string out;
	    for (char ch : in) {
	        if (!isspace(ch)) {
	            out += ch;
	        }
	    }
        return out;
	}

	static inline bool isDigit(const char ch) {
	    return (ch >= '0' && ch <= '9');
	}

    static inline bool isOperator(const char ch) {
	    return (ch >= '"' && ch <= '0') || (ch >= '<' && ch <= '@') || (ch == '^');
	}

	static inline vector<string> words(const string& in) {
        string word;
	    vector<string> words;
	    bool inword = false;
	    for (char ch : in) {
	        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == '_') {
                inword = true;
                word += ch;
            } else if (inword && (ch >= '0' && ch <= '9')) {
                word += ch;
	        } else if (!word.empty()) {
	            words.push_back(word);
	            word = "";
                inword = false;
	        }
	    }
        if (word.size() > 0) {
            words.push_back(word);
        }
        return words;
	}

    static inline string lower(const string& str) {
        string lower;
        for (int ch : str) {
            lower += (char) tolower(ch);
        }
        return lower;
    }

    static inline string upper(const string& str) {
        string upper;
        for (int ch : str) {
            upper += (char) toupper(ch);
        }
        return upper;
    }

    static inline string replace(const string& input, const string& from, const string& to) {
        string output = input;
	    if (!from.empty()) {
            size_t pos = 0;
            while ((pos = output.find(from, pos)) != string::npos) {
                output.replace(pos, from.length(), to);
                pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
            }
        }
        return output;
    }
};

#endif /* STRINGS_HH */
