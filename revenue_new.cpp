#include <bits/stdc++.h>
using namespace std;

struct Node {
    int revenue;
    int id;
    vector<int> children;
    int parent = -1;
    unordered_map<int, int> revenue_depth; // depth -> cumulative revenue
};

struct Compare {
    bool operator()(const Node& a, const Node& b) const {
        if (a.revenue == b.revenue)
            return a.id < b.id;
        return a.revenue < b.revenue;
    }
};

class RevenueSet {
    set<Node, Compare> revenueSet;
    unordered_map<int, Node> idMap;
    int nextId = 0;

public:
    int insert(int rev) {
        Node node;
        node.id = nextId++;
        node.revenue = rev;
        node.revenue_depth[0] = rev;
        idMap[node.id] = node;
        revenueSet.insert(node);
        return node.id;
    }

    int insert(int rev, int refererId) {
        int id = insert(rev);
        if (!idMap.count(refererId)) return id;

        Node& ref = idMap[refererId];
        revenueSet.erase(ref);
        ref.revenue += rev;
        ref.children.push_back(id);
        idMap[id].parent = refererId;
        revenueSet.insert(ref);

        // update depth-based aggregation upwards
        int parent = ref.parent;
        int level = 1;
        while (parent != -1) {
            idMap[parent].revenue_depth[level] += rev;
            parent = idMap[parent].parent;
            level++;
        }
        return id;
    }

    vector<int> get_k_lowest_revenue(int k, int minRevenue) {
        vector<int> ans;
        Node dummy{minRevenue, -1};
        auto it = revenueSet.lower_bound(dummy);
        while (it != revenueSet.end() && k--) {
            ans.push_back(it->id);
            ++it;
        }
        return ans;
    }

    int get_nested_referral_space_optimise(int customerId, int depth) {
        int ans = 0;
        queue<pair<int,int>> q;
        q.push({customerId, 0});
        while (!q.empty()) {
            auto [cur, d] = q.front(); q.pop();
            if (d > depth) continue;
            ans += idMap[cur].revenue;
            for (int child : idMap[cur].children)
                q.push({child, d + 1});
        }
        return ans;
    }

    int get_nested_referral_time_optimise(int customerId, int depth) {
        return idMap[customerId].revenue_depth[depth];
    }
};

int main() {
    RevenueSet rs;
    int a = rs.insert(10);
    int b = rs.insert(20);
    int c = rs.insert(30, a); // adds 30 to referer a

    cout << "get_nested_referral_space_optimise(0, 0) = "
         << rs.get_nested_referral_space_optimise(a, 0) << endl;
    cout << "get_nested_referral_time_optimise(0, 1) = "
         << rs.get_nested_referral_time_optimise(a, 1) << endl;

    auto res = rs.get_k_lowest_revenue(2, 15);
    cout << "Lowest customers: ";
    for (int id : res) cout << id << " ";
    cout << endl;
}
