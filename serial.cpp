#include "include/Evaluator.hpp"
#include "include/SaveCSV.hpp"
#include "include/TablePrinter.hpp"
#include "src/doth/builddata.h"
#include "src/doth/splitbypipe.h"
#include "src/doth/splitbyspace.h"
#include "src/doth/tokenoperation.h"
#include <bits/stdc++.h>
#include <omp.h> // only used for omp_get_wtime()
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

    for (int i = 0; i < (int)columnNames.size(); i++) {
        cout << columnNames[i] << " ";
    }
    cout << endl;

    cout << "Welcome to csv-data-processor" << endl;
    int columnLenght = (int)columnNames.size();
    int rowLenght = (int)data.size();

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
        if (command[0] == "commit") {
            CSVWriter writer("file.csv");
            writer.save(columnNames, data);
            break;
        }

        double totalTimer = 0.0;

        for (size_t cmd_idx = 0; cmd_idx < command.size(); cmd_idx++) {
            string str = command[cmd_idx];
            double startTimer = omp_get_wtime();

            if (!str.empty() && str[0] == 'a') {
                executeFinalOutput = true;

                string insideParanthesis =
                    tokenOperation.getInsideParanthesis(str);
                vector<string> token =
                    tokenOperation.seperateByCommas(insideParanthesis);

                int index = -1;
                for (int i = 0; i < (int)columnNames.size(); i++) {
                    if (columnNames[i] == token[0]) {
                        index = i;
                        break;
                    }
                }

                if (index == -1) {
                    cout << "Column not found" << endl;
                } else {
                    int start =
                        (token[1] == "*") ? rowLenght - 1 : stoi(token[1]);
                    int end =
                        (token[2] == "*") ? rowLenght - 1 : stoi(token[2]);

                    double share = 0.0;
                    for (int row = start; row <= end; row++) {
                        share += data[row][index];
                    }

                    a.push_back(to_string(share));
                }
            } else if (!str.empty() && str[0] == 'u') {
                executeFinalOutput = true;

                string insideParanthesis =
                    tokenOperation.getInsideParanthesis(str);
                vector<string> token =
                    tokenOperation.seperateByCommas(insideParanthesis);

                int index = -1;
                for (int i = 0; i < (int)columnNames.size(); i++) {
                    if (columnNames[i] == token[0]) {
                        index = i;
                        break;
                    }
                }

                if (index == -1) {
                    cout << "Column not found" << endl;
                } else {
                    int start =
                        (token[1] == "*") ? rowLenght - 1 : stoi(token[1]);
                    int end =
                        (token[2] == "*") ? rowLenght - 1 : stoi(token[2]);
                    double share = stod(token[3]);

                    for (int row = start; row <= end; row++) {
                        data[row][index] = share;
                    }

                    a.push_back("Null");
                }
            } else if (str.size() >= 2 && str[0] == 'c' && str[1] == 'a') {
                executeFinalOutput = true;

                string insideParanthesis =
                    tokenOperation.getInsideParanthesis(str);
                vector<string> token =
                    tokenOperation.seperateByCommas(insideParanthesis);

                int index = -1;
                for (int i = 0; i < (int)columnNames.size(); i++) {
                    if (columnNames[i] == token[0]) {
                        index = i;
                        break;
                    }
                }

                if (index == -1) {
                    cout << "Column not found" << endl;
                } else {
                    string condition = token[1];
                    double share = 0.0;

                    for (int row = 0; row < rowLenght; row++) {
                        if (eval.evaluate("x", data[row][index], condition)) {
                            share += data[row][index];
                        }
                    }

                    a.push_back(to_string(share));
                }
            } else if (str.size() >= 2 && str[0] == 'c' && str[1] == 'u') {
                executeFinalOutput = true;

                string insideParanthesis =
                    tokenOperation.getInsideParanthesis(str);
                vector<string> token =
                    tokenOperation.seperateByCommas(insideParanthesis);

                int index = -1;
                for (int i = 0; i < (int)columnNames.size(); i++) {
                    if (columnNames[i] == token[0]) {
                        index = i;
                        break;
                    }
                }

                if (index == -1) {
                    cout << "Column not found" << endl;
                } else {
                    double newValue = stod(token[1]);
                    string condition = token[2];

                    for (int row = 0; row < rowLenght; row++) {
                        if (eval.evaluate("x", data[row][index], condition)) {
                            data[row][index] = newValue;
                        }
                    }

                    a.push_back("Null");
                }
            } else if (!str.empty() && str[0] == 's') {
                string insideParanthesis =
                    tokenOperation.getInsideParanthesis(str);
                vector<string> token =
                    tokenOperation.seperateByCommas(insideParanthesis);
                vector<string> inputedColNames = splitbyspace.split(token[0]);
                vector<int> indexes(inputedColNames.size(), -1);

                for (int i = 0; i < (int)inputedColNames.size(); i++) {
                    for (int j = 0; j < (int)columnNames.size(); j++) {
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
                } else {
                    vector<vector<string>> table;

                    vector<string> header;
                    for (int idx : indexes) {
                        header.push_back(columnNames[idx]);
                    }
                    table.push_back(header);

                    table.resize(rowLenght + 1);

                    for (int i = 0; i < rowLenght; i++) {
                        vector<string> row;
                        for (int idx : indexes) {
                            row.push_back(to_string(data[i][idx]));
                        }
                        table[i + 1] = row;
                    }

                    TablePrinter::print(table, 12);
                }
            }

            double endTimer = omp_get_wtime();
            totalTimer += (endTimer - startTimer);
        }

        if (executeFinalOutput == false) {
            continue;
        }

        cout << "Execution Time:" << totalTimer << endl;
        vector<vector<string>> finalOutput;
        finalOutput.push_back(command);
        finalOutput.push_back(a);
        TablePrinter::print(finalOutput, 20);
        cout << endl;
    }

    return 0;
}
