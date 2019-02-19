#ifndef POLYEXT_LISTS_HP
#define POLYEXT_LISTS_HP

#include <algorithm>
#include <map>
#include <vector>
using namespace std;

class Lists {
public:
    template <typename K, typename V>
    static inline vector<K> keys(const map<K,V>& inmap) {
        vector<K> keys;
        keys.reserve(inmap.size());
        for(auto const& pair: inmap) {
            keys.push_back(pair.first);
        }
        std::sort(keys.begin(), keys.end());
        return keys;
    }

    template <typename K, typename V>
    static inline vector<K> values(const map<K,V>& inmap) {
        vector<V> vals;
        vals.reserve(inmap.size());
        for(auto const& pair: inmap) {
            vals.push_back(pair.second);
        }
        sort(vals.begin(), vals.end());
        return vals;
    }

    template <typename T>
    static inline int index(const vector<T>& in, T item, int start = 0) {
        ptrdiff_t pos = distance(in.begin() + start, find(in.begin() + start, in.end(), item));
        if (pos >= in.size()) {
            return -1;
        } else {
            return (int) pos;
        }
    }

    template <typename T>
    static inline vector<T> slice(const vector<T>& in, int start, int stop = -1) {
        vector<T> out;
//        if (stop < 0) {
//            stop = in.size() - 1;
//        }
        if (stop >= 0) {
            out.reserve(stop - start);
            for (int i = start; i <= stop; i++) {
                out.push_back(in[i]);
            }
        }
        return out;
    }
};

#endif // POLYEXT_LISTS_HP
