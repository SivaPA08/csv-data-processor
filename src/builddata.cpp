#include "doth/builddata.h"
#include <bits/stdc++.h>
using namespace std;

vector<vector<double>> BuildData::getData(ifstream &file) {
    string line;
    vector<vector<double>> matrix;
    bool isHeader = true;

    while (getline(file, line)) {
        stringstream ss(line);
        string cell;

        if (isHeader) {
            isHeader = false;
            continue;
        }

        vector<double> row;
        while (getline(ss, cell, ',')) {
            row.push_back(stof(cell));
        }
        matrix.push_back(row);
    }

    return matrix;
}
