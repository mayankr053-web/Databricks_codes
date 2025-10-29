#include <bits/stdc++.h>
using namespace std;

int minRangeCoverAllColumns(vector<vector<long long>> &A) {
    int N = A.size();
    int M = A[0].size();

    vector<pair<long long,int>> vals;  // (value, column)
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            vals.push_back({A[i][j], j});

    sort(vals.begin(), vals.end()); // sort by value

    vector<int> colCount(M, 0);
    int covered = 0;
    int left = 0;
    long long ans = LLONG_MAX;

    for (int right = 0; right < (int)vals.size(); ++right) {
        int colR = vals[right].second;
        if (colCount[colR] == 0) covered++;
        colCount[colR]++;

        // When all M columns are covered, try shrinking from left
        while (covered == M) {
            long long diff = vals[right].first - vals[left].first;
            ans = min(ans, diff);

            int colL = vals[left].second;
            colCount[colL]--;
            if (colCount[colL] == 0) covered--;
            left++;
        }
    }

    return (int)ans;
}

int main() {
    int N = 3, M = 3;
    vector<vector<long long>> A = {
        {1, 4, 5},
        {2, 9, 3},
        {3, 1, 1}
    };
    cout << "Minimum cost to go from 1st to last column = "
         << minRangeCoverAllColumns(A) << endl;
    return 0;
}
