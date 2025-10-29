#include <bits/stdc++.h>
using namespace std;

class Solution {
    int dfs(int u, int parent, const vector<vector<int>> &adj,
            const vector<bool> &isTarget, int &edgeCount) {
        bool needed = isTarget[u];
        for (int v : adj[u]) {
            if (v == parent) continue;
            bool childNeeded = dfs(v, u, adj, isTarget, edgeCount);
            if (childNeeded) {
                edgeCount++;  // this edge (u-v) is part of essential subtree
                needed = true;
            }
        }
        return needed;
    }

public:
    int minTraversalCost(int n, int root, vector<vector<int>> &edges,
                         vector<int> &targets) {
        vector<vector<int>> adj(n);
        for (auto &e : edges) {
            int u = e[0], v = e[1];
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        vector<bool> isTarget(n, false);
        for (int t : targets) isTarget[t] = true;
        isTarget[root] = true; // root also included

        int edgeCount = 0;
        dfs(root, -1, adj, isTarget, edgeCount);
        return 2 * edgeCount; // each edge traversed twice
    }
};

int main() {
    int n = 7;
    int root = 0;
    vector<vector<int>> edges = {
        {0,1}, {0,2}, {1,3}, {1,4}, {2,5}, {2,6}
    };
    vector<int> targets = {3, 5};

    Solution sol;
    cout << "Minimum traversal cost = "
         << sol.minTraversalCost(n, root, edges, targets) << endl;
    return 0;
}
