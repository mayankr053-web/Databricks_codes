#include <bits/stdc++.h>
using namespace std;

vector<string> splitMessage(string message, int limit) {
    int n = message.size();

    auto digits = [](int x) {
        return to_string(x).size();
    };

    // Try increasing total parts b
    for (int b = 1; b <= n; ++b) {
        int used = 0;
        bool ok = true;
        for (int a = 1; a <= b; ++a) {
            int suffixLen = 3 + digits(a) + digits(b);
            if (suffixLen >= limit) {
                ok = false;
                break;
            }
            used += limit - suffixLen;
        }
        if (!ok) continue;
        if (used < n) continue; // not enough capacity

        // ✅ Feasible — construct result
        vector<string> res;
        int idx = 0;
        for (int a = 1; a <= b && idx < n; ++a) {
            string suffix = "<" + to_string(a) + "/" + to_string(b) + ">";
            int suffixLen = suffix.size();
            int take = min(limit - suffixLen, n - idx);
            string part = message.substr(idx, take) + suffix;
            res.push_back(part);
            idx += take;
        }
        return res;
    }

    return {}; // impossible
}

int main() {
    string msg = "this is really a very awesome message";
    int limit = 9;

    vector<string> res = splitMessage(msg, limit);
    for (auto &p : res) cout << "\"" << p << "\", ";
    cout << endl;
}
