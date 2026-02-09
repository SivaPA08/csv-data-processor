
#pragma once

#include <string>
#include <vector>

class SplitByPipe {
  public:
    std::vector<std::string> split(const std::string &str) {
        std::vector<std::string> result;
        result.reserve(10);

        std::string temp;
        temp.reserve(50);

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
};
