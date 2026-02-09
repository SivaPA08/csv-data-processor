
#pragma once

#include <string>
#include <vector>

class SplitBySpace {
  public:
    std::vector<std::string> split(const std::string &str) {
        std::vector<std::string> result;
        std::string temp;

        for (char c : str) {
            if (c == ' ') {
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
