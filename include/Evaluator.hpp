#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP

#include <string>

extern "C" bool tokenEval(int num, const char *expr);
extern "C" double calcExpr(double num, const char *expr);

class Evaluator {
  public:
    bool evaluate(const std::string &varName, int varValue,
                  const std::string &condition) {
        (void)varName;
        std::string cond = condition;
        size_t pos = 0;
        while ((pos = cond.find("&&", pos)) != std::string::npos) {
            cond.replace(pos, 2, "&&");
            pos += 2;
        }
        pos = 0;
        while ((pos = cond.find("||", pos)) != std::string::npos) {
            cond.replace(pos, 2, "||");
            pos += 2;
        }
        return tokenEval(varValue, cond.c_str());
    }
    int calculate(const std::string &varName, double varValue,
                  const std::string &condition) {
        return calcExpr(varValue, condition.c_str());
    }
};

#endif
