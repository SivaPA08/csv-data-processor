#include "include/Evaluator.hpp"
#include "include/TablePrinter.hpp"
#include "src/doth/builddata.h"
#include "src/doth/splitbypipe.h"
#include "src/doth/splitbyspace.h"
#include "src/doth/tokenoperation.h"
#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

int main() {
    // Set number of threads for optimal performance
    omp_set_num_threads(omp_get_max_threads());

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

        double totalTimer = 0.0;

// Process commands in parallel using tasks
#pragma omp parallel
        {
#pragma omp single
            {
                for (size_t cmd_idx = 0; cmd_idx < command.size(); cmd_idx++) {
#pragma omp task firstprivate(cmd_idx)
                    {
                        string str = command[cmd_idx];
                        double startTimer = omp_get_wtime();

                        // for column addition with range
                        if (str[0] == 'a') {
#pragma omp atomic write
                            executeFinalOutput = true;

                            string insideParanthesis =
                                tokenOperation.getInsideParanthesis(str);
                            vector<string> token =
                                tokenOperation.seperateByCommas(
                                    insideParanthesis);

                            int index = -1;
                            for (int i = 0; i < columnNames.size(); i++) {
                                if (columnNames[i] == token[0]) {
                                    index = i;
                                    break;
                                }
                            }

                            if (index == -1) {
#pragma omp critical
                                cout << "Column not found" << endl;
                            } else {
                                int start = (token[1] == "*") ? rowLenght - 1
                                                              : stoi(token[1]);
                                int end = (token[2] == "*") ? rowLenght - 1
                                                            : stoi(token[2]);

                                double share = 0.0;
// Use reduction for parallel summation
#pragma omp parallel for reduction(+ : share) schedule(static)
                                for (int row = start; row <= end; row++) {
                                    share += data[row][index];
                                }

#pragma omp critical
                                a.push_back(to_string(share));
                            }
                        }
                        // Update operation with range
                        else if (str[0] == 'u') {
#pragma omp atomic write
                            executeFinalOutput = true;

                            string insideParanthesis =
                                tokenOperation.getInsideParanthesis(str);
                            vector<string> token =
                                tokenOperation.seperateByCommas(
                                    insideParanthesis);

                            int index = -1;
                            for (int i = 0; i < columnNames.size(); i++) {
                                if (columnNames[i] == token[0]) {
                                    index = i;
                                    break;
                                }
                            }

                            if (index == -1) {
#pragma omp critical
                                cout << "Column not found" << endl;
                            } else {
                                int start = (token[1] == "*") ? rowLenght - 1
                                                              : stoi(token[1]);
                                int end = (token[2] == "*") ? rowLenght - 1
                                                            : stoi(token[2]);
                                double share = stod(token[3]);

// Parallel update
#pragma omp parallel for schedule(static)
                                for (int row = start; row <= end; row++) {
                                    data[row][index] = share;
                                }

#pragma omp critical
                                a.push_back("Null");
                            }
                        }
                        // Conditional addition
                        else if (str[0] == 'c' && str[1] == 'a') {
#pragma omp atomic write
                            executeFinalOutput = true;

                            string insideParanthesis =
                                tokenOperation.getInsideParanthesis(str);
                            vector<string> token =
                                tokenOperation.seperateByCommas(
                                    insideParanthesis);

                            int index = -1;
                            for (int i = 0; i < columnNames.size(); i++) {
                                if (columnNames[i] == token[0]) {
                                    index = i;
                                    break;
                                }
                            }

                            if (index == -1) {
#pragma omp critical
                                cout << "Column not found" << endl;
                            } else {
                                string condition = token[1];
                                double share = 0.0;

// Parallel reduction with conditional sum
#pragma omp parallel for reduction(+ : share) schedule(dynamic, 100)
                                for (int row = 0; row < rowLenght; row++) {
                                    if (eval.evaluate("x", data[row][index],
                                                      condition)) {
                                        share += data[row][index];
                                    }
                                }

#pragma omp critical
                                a.push_back(to_string(share));
                            }
                        }
                        // Conditional update
                        else if (str[0] == 'c' && str[1] == 'u') {
#pragma omp atomic write
                            executeFinalOutput = true;

                            string insideParanthesis =
                                tokenOperation.getInsideParanthesis(str);
                            vector<string> token =
                                tokenOperation.seperateByCommas(
                                    insideParanthesis);

                            int index = -1;
                            for (int i = 0; i < columnNames.size(); i++) {
                                if (columnNames[i] == token[0]) {
                                    index = i;
                                    break;
                                }
                            }

                            if (index == -1) {
#pragma omp critical
                                cout << "Column not found" << endl;
                            } else {
                                double newValue = stod(token[1]);
                                string condition = token[2];

// Parallel conditional update
#pragma omp parallel for schedule(dynamic, 100)
                                for (int row = 0; row < rowLenght; row++) {
                                    if (eval.evaluate("x", data[row][index],
                                                      condition)) {
                                        data[row][index] = newValue;
                                    }
                                }

#pragma omp critical
                                a.push_back("Null");
                            }
                        }
                        // Select/display operation
                        else if (str[0] == 's') {
                            string insideParanthesis =
                                tokenOperation.getInsideParanthesis(str);
                            vector<string> token =
                                tokenOperation.seperateByCommas(
                                    insideParanthesis);
                            vector<string> inputedColNames =
                                splitbyspace.split(token[0]);
                            vector<int> indexes(inputedColNames.size(), -1);

                            // Find column indexes
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
#pragma omp critical
                                cout << "Column not found" << endl;
                            } else {
                                // Build table with parallel data extraction
                                vector<vector<string>> table;

                                // Header row
                                vector<string> header;
                                for (int idx : indexes) {
                                    header.push_back(columnNames[idx]);
                                }
                                table.push_back(header);

                                // Pre-allocate space for rows
                                table.resize(rowLenght + 1);

// Parallel data row construction
#pragma omp parallel for schedule(static)
                                for (int i = 0; i < rowLenght; i++) {
                                    vector<string> row;
                                    for (int idx : indexes) {
                                        row.push_back(to_string(data[i][idx]));
                                    }
                                    table[i + 1] = row;
                                }

#pragma omp critical
                                TablePrinter::print(table, 12);
                            }
                        }

                        double endTimer = omp_get_wtime();
#pragma omp atomic
                        totalTimer += (endTimer - startTimer);
                    }
                }
#pragma omp taskwait
            }
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
