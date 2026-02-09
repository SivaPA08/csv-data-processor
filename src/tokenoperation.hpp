
#pragma once

#include <string>
#include <vector>

class TokenOperation {
  public:
    std::string getInsideParanthesis(const std::string &s) {
        int start = -1;
        int count = 0;

        for (size_t i = 0; i < s.length(); i++) {
            if (s[i] == '(') {
                if (count == 0)
                    start = i;
                count++;
            } else if (s[i] == ')') {
                count--;
                if (count == 0 && start != -1) {
                    return s.substr(start + 1, i - start - 1);
                }
            }
        }
        return "";
    }

    std::vector<std::string> seperateByCommas(const std::string &s) {
        std::vector<std::string> result;
        std::string temp;

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
};
