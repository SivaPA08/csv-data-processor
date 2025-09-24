#include "doth/builddata.h"
#include <sstream>
#include <string>
using namespace std;

vector<string> BuildData::getColumnNames(ifstream &file) {
    string line;
    vector<string> columns;

    if (!file.good())
        return columns;

    file.clear();
    file.seekg(0);

    if (getline(file, line)) {
        if (line.length() >= 3 && (unsigned char)line[0] == 0xEF &&
            (unsigned char)line[1] == 0xBB && (unsigned char)line[2] == 0xBF) {
            line = line.substr(3);
        }

        stringstream ss(line);
        string cell;

        while (getline(ss, cell, ',')) {
            size_t start = cell.find_first_not_of(" \t\r\n");
            if (start == string::npos) {
                columns.push_back(""); // Empty cell
                continue;
            }
            size_t end = cell.find_last_not_of(" \t\r\n");
            cell = cell.substr(start, end - start + 1);

            columns.push_back(cell);
        }
    }
    return columns;
}

vector<vector<double>> BuildData::getData(ifstream &file) {
    string line;
    vector<vector<double>> matrix;
    bool isHeader = true;

    while (getline(file, line)) {
        stringstream ss(line);
        string cell;

        if (isHeader) {
            // skip header
            isHeader = false;
            continue;
        }

        vector<double> row;
        while (getline(ss, cell, ',')) {
            row.push_back(stod(cell));
        }
        matrix.push_back(row);
    }

    return matrix;
}
