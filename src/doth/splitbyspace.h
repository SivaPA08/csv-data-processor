#pragma once
#include <bits/stdc++.h>
using namespace std;
class SplitBySpace {
  public:
    vector<string> split(const string &str) {
        vector<string> result;
        string temp = "";
        for (char c : str) {
            if (c == ' ') {
                result.push_back(temp);
                temp = "";
            } else {
                temp += c;
            }
        }
        result.push_back(temp);
        return result;
    }
};
