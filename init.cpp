#include "include/Evaluator.hpp"
#include "include/TablePrinter.hpp"
#include "src/doth/builddata.h"
#include "src/doth/splitbypipe.h"
#include "src/doth/splitbyspace.h"
#include "src/doth/tokenoperation.h"
#include <bits/stdc++.h>
using namespace std;

int main() {
    SplitByPipe splitbypipe;
    TokenOperation tokenOperation;
    SplitBySpace splitbyspace;
    Evaluator eval;

    BuildData buildData;
    ifstream head("file.csv");
    ifstream file("file.csv");
    cout << "Loading data..." << endl;
    vector<string> columnNames = buildData.getColumnNames(head);
    vector<vector<double>> data = buildData.getData(file);

    // removing part after testing
    for (int i = 0; i < columnNames.size(); i++) {
        cout << columnNames[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data[i].size(); j++) {
            cout << data[i][j] << " ";
        }
        cout << endl;
    }
    // ending of the removing part
    cout << "Welcome to csv-data-processor" << endl;
    int columnLenght = columnNames.size();
    int rowLenght = data.size();
    while (true) {
        bool executeFinalOutput = false;
        cout << "csv-data-processor>>>";
        vector<string> a;
        string s;
        getline(cin, s);
        if (s.empty())
            continue;
        vector<string> command = splitbypipe.split(s);

        if (command[0] == "exit") {
            break;
        }

        for (string str : command) {

            // for column addition
            if (str[0] == 'a') { // column add with a range
                executeFinalOutput = true;
                string insideParanthesis =
                    tokenOperation.getInsideParanthesis(str);
                vector<string> token =
                    tokenOperation.seperateByCommas(insideParanthesis);
                bool ok = false;
                int index = -1;
                for (int i = 0; i < columnNames.size(); i++) {
                    if (columnNames[i] == token[0]) {
                        ok = true;
                        index = i;
                        break;
                    }
                }
                if (!ok) {
                    cout << "Column not found" << endl;
                    continue;
                }
                int start, end;
                if (token[1] == "*") {
                    start = rowLenght - 1;

                } else {
                    start = stoi(token[1]);
                }
                if (token[2] == "*") {
                    end = rowLenght - 1;
                } else {
                    end = stoi(token[2]);
                }
                double share = 0.0;
                for (int row = start; row <= end; row++) {

                    share += data[row][index];
                }
                a.push_back(to_string(share));
            }

            else if (str[0] == 'u') {
                executeFinalOutput = true;
                string insideParanthesis =
                    tokenOperation.getInsideParanthesis(str);
                vector<string> token =
                    tokenOperation.seperateByCommas(insideParanthesis);
                bool ok = false;
                int index = -1;
                for (int i = 0; i < columnNames.size(); i++) {
                    if (columnNames[i] == token[0]) {
                        ok = true;
                        index = i;
                        break;
                    }
                }
                if (!ok) {
                    cout << "Column not found" << endl;
                    continue;
                }
                int start;
                int end;
                if (token[1] == "*") {
                    start = rowLenght - 1;

                } else {
                    start = stoi(token[1]);
                }
                if (token[2] == "*") {
                    end = rowLenght - 1;
                } else {
                    end = stoi(token[2]);
                }
                double share = stod(token[3]);
                for (int row = start; row <= end; row++) {
                    data[row][index] = share;
                }
                a.push_back("Null");
            } else if (str[0] == 'c' && str[1] == 'a') {
                executeFinalOutput = true;
                string insideParanthesis =
                    tokenOperation.getInsideParanthesis(str);
                vector<string> token =
                    tokenOperation.seperateByCommas(insideParanthesis);
                bool ok = false;
                int index = -1;
                for (int i = 0; i < columnNames.size(); i++) {
                    if (columnNames[i] == token[0]) {
                        ok = true;
                        index = i;
                        break;
                    }
                }
                if (!ok) {
                    cout << "Column not found" << endl;
                    continue;
                }
                string condition = token[1];
                double share = 0.0;

                for (int row = 0; row < rowLenght; row++) {
                    if (eval.evaluate("x", data[row][index], condition)) {

                        share += data[row][index];
                    }
                }
                a.push_back(to_string(share));
            } else if (str[0] == 'c' && str[1] == 'u') {
                executeFinalOutput = true;
                string insideParanthesis =
                    tokenOperation.getInsideParanthesis(str);
                vector<string> token =
                    tokenOperation.seperateByCommas(insideParanthesis);
                bool ok = false;
                int index = -1;
                for (int i = 0; i < columnNames.size(); i++) {
                    if (columnNames[i] == token[0]) {
                        ok = true;
                        index = i;
                        break;
                    }
                }
                if (!ok) {
                    cout << "column not found u mf" << endl;
                    continue;
                }
                for (int row = 0; row < rowLenght; row++) {
                    if (eval.evaluate("x", data[row][index], token[2])) {
                        data[row][index] = stod(token[1]);
                    }
                }
                a.push_back("Null");
            } else if (str[0] == 's') {
                string insideParanthesis =
                    tokenOperation.getInsideParanthesis(str);
                vector<string> token =
                    tokenOperation.seperateByCommas(insideParanthesis);
                vector<string> inputedColNames = splitbyspace.split(token[0]);
                vector<int> indexes(inputedColNames.size(), -1);

                // find column indexes
                for (int i = 0; i < inputedColNames.size(); i++) {
                    for (int j = 0; j < columnNames.size(); j++) {
                        if (columnNames[j] == inputedColNames[i]) {
                            indexes[i] = j;
                            break;
                        }
                    }
                }

                bool ok = true;
                for (int i : indexes) {
                    if (i == -1) {
                        ok = false;
                        break;
                    }
                }
                if (!ok) {
                    cout << "Column not found" << endl;
                    continue;
                }

                // ---- Use TablePrinter ----
                std::vector<std::vector<std::string>> table;

                // header row
                std::vector<std::string> header;
                for (int idx : indexes) {
                    header.push_back(columnNames[idx]);
                }
                table.push_back(header);

                // data rows
                for (int i = 0; i < rowLenght; i++) {
                    std::vector<std::string> row;
                    for (int idx : indexes) {
                        row.push_back(std::to_string(data[i][idx]));
                    }
                    table.push_back(row);
                }

                TablePrinter::print(table, 12);
            }
        }
        if (executeFinalOutput == false) {
            continue;
        }
        vector<vector<string>> finalOutput;
        finalOutput.push_back(command);
        finalOutput.push_back(a);
        TablePrinter::print(finalOutput, 20);
        cout << endl;
    }
}
