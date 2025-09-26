#include "doth/splitbypipe.h"
using namespace std;

vector<string> SplitByPipe::split(const string &str) {
    vector<string> result;
    result.reserve(10); // or estimate based on typical input
    string temp;
    temp.reserve(50); // reserve for typical token size

    for (char c : str) {
        if (c == '|') {
            if (!temp.empty()) {
                result.push_back(temp);
                temp.clear();
            }
        } else {
            temp += c;
        }
    }
    if (!temp.empty())
        result.push_back(temp);
    return result;
}
