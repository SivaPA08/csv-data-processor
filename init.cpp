#include "src/doth/builddata.h"
#include "src/doth/splitbypipe.h"
#include "src/doth/tokenoperation.h"
#include <bits/stdc++.h>
using namespace std;

int main() {
    SplitByPipe splitbypipe;
    TokenOperation tokenOperation;

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
    while (true) {
        cout << "csv-data-processor>>>";
        vector<double> a;
        string s;
        getline(cin, s);
        if (s.empty())
            continue;
        vector<string> command = splitbypipe.split(s);

        if (command[0] == "exit") {
            break;
        }

        for (string str : command) {

            if (str[0] == 'a') { // column add with a range
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
                if (token[1] == "*") {
                    start = 0;

                } else {
                    start = stoi(token[1]);
                }
                double share = 0.0;
                for (int row = start; row < data.size(); row++) {
                    share += data[row][index];
                }
                a.push_back(share);
            }
        }
        for (double colvals : a) {
            cout << colvals << " | ";
        }
        cout << endl;
    }
}
