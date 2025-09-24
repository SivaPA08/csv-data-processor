#include "src/doth/builddata.h"
#include "src/doth/splitbyslash.h"
#include <bits/stdc++.h>
using namespace std;
int main() {
    BuildData buildData;
    ifstream file("file.csv");
    vector<vector<double>> data = buildData.getData(file);
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
