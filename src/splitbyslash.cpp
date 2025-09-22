#include "doth/splitbyslash.h"
using namespace std;

vector<string> SplitBySlash::split(const string &str) {
    vector<string> result;
    string temp;
    for (char c : str) {
        if (c == '/') {
            if (!temp.empty())
                result.push_back(temp);
            temp.clear();
        } else {
            temp += c;
        }
    }
    if (!temp.empty())
        result.push_back(temp);
    return result;
}
