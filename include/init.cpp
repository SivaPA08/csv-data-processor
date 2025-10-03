#include "Evaluator.hpp"
#include <bits/stdc++.h>

using namespace std;
int main() {
    Evaluator eval;
    string s;
    getline(cin, s);
    int arr[] = {-1, 2, 4, 5, 10, 3, 6, 8};
    for (int i = 0; i < 8; i++) {
        if (eval.evaluate("x", arr[i], s)) {
            cout << arr[i] << " True\n ";
        } else {
            cout << arr[i] << " False\n ";
        }
    }
}
