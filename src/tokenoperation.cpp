#include "doth/tokenoperation.h"
#include <bits/stdc++.h>

using namespace std;

string TokenOperation::getInsideParanthesis(string s) {
    int count = 0;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '(')
            count++;
        if (s[i] == ')')
            count--;
        if (count == 0)
            return s.substr(0, i + 1);
    }
    return "";
}

vector<string> TokenOperation::seperateByCommas(string s) {
    vector<string> result;
    string temp;
    for (char c : s) {
        if (c == ',') {
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
