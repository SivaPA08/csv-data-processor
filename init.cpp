#include "src/doth/builddata.h"
#include "src/doth/splitbyslash.h"
#include <bits/stdc++.h>
using namespace std;
int main() {

    BuildData buildData;
    ifstream head("file.csv");
    ifstream file("file.csv");
    vector<string> columnNames = buildData.getColumnNames(head);
    vector<vector<double>> data = buildData.getData(file);
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
    while (true) {
        cout << "csv-data-processor>>>";
        string s;
        cin >> s;
    }
}
