#pragma once
#include <fstream>
#include <vector>

class BuildData {
  public:
    std::vector<std::vector<double>> getData(std::ifstream &file);
};
