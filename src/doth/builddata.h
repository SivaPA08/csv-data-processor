#pragma once
#include <fstream>
#include <string>
#include <vector>

class BuildData {
  public:
    std::vector<std::string> getColumnNames(std::ifstream &file);
    std::vector<std::vector<double>> getData(std::ifstream &file);
};
