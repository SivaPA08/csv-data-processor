#include "include/Evaluator.hpp"
#include "include/TablePrinter.hpp"
#include "src/doth/builddata.h"
#include "src/doth/splitbypipe.h"
#include "src/doth/splitbyspace.h"
#include "src/doth/tokenoperation.h"
#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

static inline int findColumnIndex(const vector<string> &columnNames,
                                  const string &name) {
    for (int i = 0; i < (int)columnNames.size(); ++i) {
        if (columnNames[i] == name)
            return i;
    }
    return -1;
}

int main() {
    // Tune this: only parallelize loops when there are many rows.
    const int PARALLEL_THRESHOLD = 10000;

    // Prevent nested parallel regions from causing oversubscription
    omp_set_nested(0);
    omp_set_max_active_levels(1);
    // Optional: you can set threads to a reasonable number; leaving default is
    // fine omp_set_num_threads(omp_get_max_threads());

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

    for (const auto &cn : columnNames)
        cout << cn << " ";
    cout << endl;

    cout << "Welcome to csv-data-processor" << endl;
    int columnLenght = (int)columnNames.size();
    int rowLenght = (int)data.size();

    while (true) {
        bool executeFinalOutput = false;
        cout << "csv-data-processor>>>";
        string s;
        getline(cin, s);
        if (s.empty())
            continue;

        vector<string> command = splitbypipe.split(s);
        if (command.empty())
            continue;
        if (command[0] == "exit")
            break;

        double totalTimer = 0.0;
        vector<string> a; // results corresponding to commands (kept sequential)

        // Process commands sequentially (avoid task overhead). Parallelize only
        // inner loops.
        for (size_t cmd_idx = 0; cmd_idx < command.size(); ++cmd_idx) {
            string str = command[cmd_idx];
            double startTimer = omp_get_wtime();

            // for column addition with range: a(col,start,end)
            if (!str.empty() && str[0] == 'a') {
                executeFinalOutput = true;

                string insideParanthesis =
                    tokenOperation.getInsideParanthesis(str);
                vector<string> token =
                    tokenOperation.seperateByCommas(insideParanthesis);
                if (token.size() < 3) {
                    cerr << "Invalid arguments for add" << endl;
                    a.push_back("Error");
                    continue;
                }

                int index = findColumnIndex(columnNames, token[0]);
                if (index == -1) {
                    cout << "Column not found" << endl;
                    a.push_back("Error");
                } else {
                    int start = (token[1] == "*") ? 0 : stoi(token[1]);
                    int end =
                        (token[2] == "*") ? rowLenght - 1 : stoi(token[2]);
                    start = max(0, min(start, rowLenght - 1));
                    end = max(0, min(end, rowLenght - 1));
                    double share = 0.0;

                    if (end - start + 1 > PARALLEL_THRESHOLD) {
#pragma omp parallel for reduction(+ : share) schedule(static)
                        for (int row = start; row <= end; ++row)
                            share += data[row][index];
                    } else {
                        for (int row = start; row <= end; ++row)
                            share += data[row][index];
                    }

                    a.push_back(to_string(share));
                }
            }
            // update operation with range: u(col,start,end,value)
            else if (!str.empty() && str[0] == 'u') {
                executeFinalOutput = true;

                string insideParanthesis =
                    tokenOperation.getInsideParanthesis(str);
                vector<string> token =
                    tokenOperation.seperateByCommas(insideParanthesis);
                if (token.size() < 4) {
                    cerr << "Invalid arguments for update" << endl;
                    a.push_back("Error");
                    continue;
                }

                int index = findColumnIndex(columnNames, token[0]);
                if (index == -1) {
                    cout << "Column not found" << endl;
                    a.push_back("Error");
                } else {
                    int start = (token[1] == "*") ? 0 : stoi(token[1]);
                    int end =
                        (token[2] == "*") ? rowLenght - 1 : stoi(token[2]);
                    start = max(0, min(start, rowLenght - 1));
                    end = max(0, min(end, rowLenght - 1));
                    double share = stod(token[3]);

                    if (end - start + 1 > PARALLEL_THRESHOLD) {
#pragma omp parallel for schedule(static)
                        for (int row = start; row <= end; ++row)
                            data[row][index] = share;
                    } else {
                        for (int row = start; row <= end; ++row)
                            data[row][index] = share;
                    }

                    a.push_back("Null");
                }
            }
            // conditional addition: ca(col, condition)
            else if (str.size() >= 2 && str[0] == 'c' && str[1] == 'a') {
                executeFinalOutput = true;

                string insideParanthesis =
                    tokenOperation.getInsideParanthesis(str);
                vector<string> token =
                    tokenOperation.seperateByCommas(insideParanthesis);
                if (token.size() < 2) {
                    cerr << "Invalid arguments for conditional add" << endl;
                    a.push_back("Error");
                    continue;
                }

                int index = findColumnIndex(columnNames, token[0]);
                if (index == -1) {
                    cout << "Column not found" << endl;
                    a.push_back("Error");
                } else {
                    string condition = token[1];
                    double share = 0.0;

                    if (rowLenght > PARALLEL_THRESHOLD) {
#pragma omp parallel for reduction(+ : share) schedule(dynamic, 256)
                        for (int row = 0; row < rowLenght; ++row) {
                            if (eval.evaluate("x", data[row][index], condition))
                                share += data[row][index];
                        }
                    } else {
                        for (int row = 0; row < rowLenght; ++row)
                            if (eval.evaluate("x", data[row][index], condition))
                                share += data[row][index];
                    }

                    a.push_back(to_string(share));
                }
            }
            // conditional update: cu(col, newValue, condition)
            else if (str.size() >= 2 && str[0] == 'c' && str[1] == 'u') {
                executeFinalOutput = true;

                string insideParanthesis =
                    tokenOperation.getInsideParanthesis(str);
                vector<string> token =
                    tokenOperation.seperateByCommas(insideParanthesis);
                if (token.size() < 3) {
                    cerr << "Invalid arguments for conditional update" << endl;
                    a.push_back("Error");
                    continue;
                }

                int index = findColumnIndex(columnNames, token[0]);
                if (index == -1) {
                    cout << "Column not found" << endl;
                    a.push_back("Error");
                } else {
                    double newValue = stod(token[1]);
                    string condition = token[2];

                    if (rowLenght > PARALLEL_THRESHOLD) {
#pragma omp parallel for schedule(dynamic, 256)
                        for (int row = 0; row < rowLenght; ++row)
                            if (eval.evaluate("x", data[row][index], condition))
                                data[row][index] = newValue;
                    } else {
                        for (int row = 0; row < rowLenght; ++row)
                            if (eval.evaluate("x", data[row][index], condition))
                                data[row][index] = newValue;
                    }

                    a.push_back("Null");
                }
            }
            // select/display: s(col1 col2 ...)
            else if (!str.empty() && str[0] == 's') {
                string insideParanthesis =
                    tokenOperation.getInsideParanthesis(str);
                vector<string> token =
                    tokenOperation.seperateByCommas(insideParanthesis);
                if (token.empty()) {
                    cout << "No columns specified" << endl;
                    a.push_back("Error");
                    continue;
                }

                vector<string> inputedColNames = splitbyspace.split(token[0]);
                vector<int> indexes(inputedColNames.size(), -1);
                bool ok = true;
                for (int i = 0; i < (int)inputedColNames.size(); ++i) {
                    indexes[i] =
                        findColumnIndex(columnNames, inputedColNames[i]);
                    if (indexes[i] == -1) {
                        ok = false;
                        break;
                    }
                }

                if (!ok) {
                    cout << "Column not found" << endl;
                    a.push_back("Error");
                } else {
                    // Build table: header + rows
                    vector<vector<string>> table;
                    table.reserve(rowLenght + 1);

                    vector<string> header;
                    for (int idx : indexes)
                        header.push_back(columnNames[idx]);
                    table.push_back(header);

                    // Pre-allocate empty rows
                    table.resize(rowLenght + 1);

                    // Fill rows (parallel for if large)
                    if (rowLenght > PARALLEL_THRESHOLD) {
#pragma omp parallel for schedule(static)
                        for (int i = 0; i < rowLenght; ++i) {
                            vector<string> row;
                            row.reserve(indexes.size());
                            for (int idx : indexes)
                                row.push_back(to_string(data[i][idx]));
                            table[i + 1] = std::move(row);
                        }
                    } else {
                        for (int i = 0; i < rowLenght; ++i) {
                            vector<string> row;
                            row.reserve(indexes.size());
                            for (int idx : indexes)
                                row.push_back(to_string(data[i][idx]));
                            table[i + 1] = std::move(row);
                        }
                    }

                    TablePrinter::print(table, 12);
                    a.push_back("Displayed");
                }
            } else {
                // Unknown command: just echo or handle
                cout << "Unknown command: " << str << endl;
                a.push_back("Unknown");
            }

            double endTimer = omp_get_wtime();
            totalTimer += (endTimer - startTimer);
        } // end for each command

        if (!executeFinalOutput)
            continue;

        cout << "Execution Time: " << totalTimer << " seconds" << endl;
        vector<vector<string>> finalOutput;
        finalOutput.push_back(command);
        finalOutput.push_back(a);
        TablePrinter::print(finalOutput, 20);
        cout << endl;
    } // while

    return 0;
}
