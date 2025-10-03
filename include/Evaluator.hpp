#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP

#include <cstdio>
#include <string>

class Evaluator {
  public:
    bool evaluate(const std::string &varName, int varValue,
                  const std::string &condition) {
        std::string cond = condition;
        // Convert C++ style &&/|| to Python style
        size_t pos = 0;
        while ((pos = cond.find("&&", pos)) != std::string::npos) {
            cond.replace(pos, 2, "and");
            pos += 3;
        }
        pos = 0;
        while ((pos = cond.find("||", pos)) != std::string::npos) {
            cond.replace(pos, 2, "or");
            pos += 2;
        }

        std::string jsonStr = "{ \"var_name\": \"" + varName +
                              "\", \"var_value\": " + std::to_string(varValue) +
                              ", \"condition\": \"" + cond + "\" }";

        std::string cmd = "python3 evaluator.py '" + jsonStr + "'";
        FILE *pipe = popen(cmd.c_str(), "r");
        if (!pipe)
            return false;

        char buffer[128];
        std::string result = "";
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
            result += buffer;
        pclose(pipe);

        if (!result.empty() && result.back() == '\n')
            result.pop_back();
        return result == "1";
    }
};

#endif
