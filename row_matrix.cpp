#include <bits/stdc++.h>
using namespace std;

struct Node {
    int val, row, idx;
    bool operator>(const Node &other) const {
        return val > other.val;
    }
};

vector<int> minimizeDifference(vector<vector<int>> &A) {
    int n = A.size();
    for (auto &row : A) sort(row.begin(), row.end());

    priority_queue<Node, vector<Node>, greater<Node>> pq;
    int currentMax = INT_MIN;

    // Step 1: take first element from each row
    for (int i = 0; i < n; ++i) {
        pq.push({A[i][0], i, 0});
        currentMax = max(currentMax, A[i][0]);
    }

    int bestRange = INT_MAX;
    int bestMin = 0, bestMax = 0;

    // Step 2: move through possible combinations
    while (true) {
        Node cur = pq.top(); pq.pop();
        int currentMin = cur.val;

        // Update best range
        if (currentMax - currentMin < bestRange) {
            bestRange = currentMax - currentMin;
            bestMin = currentMin;
            bestMax = currentMax;
        }

        // Advance in this row
        if (cur.idx + 1 == (int)A[cur.row].size()) break;
        int nextVal = A[cur.row][cur.idx + 1];
        pq.push({nextVal, cur.row, cur.idx + 1});
        currentMax = max(currentMax, nextVal);
    }

    // Step 3: build one valid result within [bestMin, bestMax]
    vector<int> result;
    for (auto &row : A) {
        // pick the element in row that lies within range
        int pick = row[0];
        for (int v : row)
            if (v >= bestMin && v <= bestMax) { pick = v; break; }
        result.push_back(pick);
    }

    sort(result.begin(), result.end());
    return result;
}

int main() {
    vector<vector<int>> A = {
        {61, 90, 60},
        {59, 61},
        {58, 62, 92}
    };

    vector<int> res = minimizeDifference(A);
    cout << "Result = ";
    for (int x : res) cout << x << " ";
    cout << endl;
}
