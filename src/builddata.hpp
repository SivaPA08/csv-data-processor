#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class BuildData {
  public:
    std::vector<std::string> getColumnNames(std::ifstream &file) {
        std::string line;
        std::vector<std::string> columns;

        if (!file.good())
            return columns;

        file.clear();
        file.seekg(0);

        if (std::getline(file, line)) {

            if (line.length() >= 3 && (unsigned char)line[0] == 0xEF &&
                (unsigned char)line[1] == 0xBB &&
                (unsigned char)line[2] == 0xBF) {
                line = line.substr(3);
            }

            std::stringstream ss(line);
            std::string cell;

            while (std::getline(ss, cell, ',')) {

                size_t start = cell.find_first_not_of(" \t\r\n");
                if (start == std::string::npos) {
                    columns.push_back("");
                    continue;
                }

                size_t end = cell.find_last_not_of(" \t\r\n");
                cell = cell.substr(start, end - start + 1);

                columns.push_back(cell);
            }
        }

        return columns;
    }

    std::vector<std::vector<double>> getData(std::ifstream &file) {
        std::string line;
        std::vector<std::vector<double>> matrix;

        file.clear();
        file.seekg(0);

        bool isHeader = true;

        while (std::getline(file, line)) {

            if (isHeader) {
                isHeader = false;
                continue;
            }

            std::stringstream ss(line);
            std::string cell;
            std::vector<double> row;

            while (std::getline(ss, cell, ',')) {
                row.push_back(std::stod(cell));
            }

            matrix.push_back(row);
        }

        return matrix;
    }
};
