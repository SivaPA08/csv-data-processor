#include "doth/tokenoperation.h"
#include <bits/stdc++.h>

using namespace std;

string TokenOperation::getInsideParanthesis(string s) {
    int start = -1;
    int count = 0;

    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '(') {
            if (count == 0)
                start = i; // Mark start of first opening
            count++;
        }
        if (s[i] == ')') {
            count--;
            if (count == 0 && start != -1) {
                return s.substr(start + 1, i - start - 1); // Content inside
            }
        }
    }
    return ""; // No balanced parentheses found
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
