#include <bits/stdc++.h>
using namespace std;

string groupSum(string s, int k) {
    while (s.size() > k) {
        string newStr = "";
        for (int i = 0; i < s.size(); i += 3) {
            int sum = 0;
            for (int j = i; j < i + 3 && j < s.size(); ++j) {
                sum += s[j] - '0';
            }
            newStr += to_string(sum);
        }
        s = newStr;
    }
    return s;
}

int main() {
    string s = "1111122222";
    int k = 3;
    cout << groupSum(s, k) << endl; // Output: 132
    return 0;
}
