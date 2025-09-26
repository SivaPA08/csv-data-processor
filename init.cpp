#include "src/doth/builddata.h"
#include "src/doth/splitbyslash.h"
#include <bits/stdc++.h>
using namespace std;

int main() {
    SplitBySlash splitBySlash;

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
        string s;
        getline(cin, s);
        if (s.empty())
            continue;
        vector<string> command = splitBySlash.split(s);

        if (command[0] == "exit") {
            break;
        }
    }
}
