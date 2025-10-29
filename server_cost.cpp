#include <bits/stdc++.h>
using namespace std;

int minCostToCoverAllServers(const vector<int>& cost) {
    int n = cost.size();
    vector<int> dp(n + 1, INT_MAX);
    dp[0] = 0;  // No servers covered initially

    for (int i = 0; i < n; ++i) {
        if (dp[i] == INT_MAX) continue; // Skip unreachable state

        int coverTill = min(n, i + i + 1);  // cover [i ... i+i]
        dp[coverTill] = min(dp[coverTill], dp[i] + cost[i]);
    }

    return dp[n];
}

int main() {
    vector<int> cost = {5, 10, 4, 11, 3};
    cout << "Minimum cost to cover all servers = "
         << minCostToCoverAllServers(cost) << endl;
    return 0;
}
