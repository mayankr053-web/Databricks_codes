#include <bits/stdc++.h>
using namespace std;

class CustomerSystem {
private:
    struct Customer {
        int id;
        int revenue;
    };

    unordered_map<int, Customer> customers;  // id → customer
    multiset<pair<int, int>> revenueSet;     // (revenue, id)
    int nextId = 1;

public:
    // Insert a new customer with given revenue
    int insert(int revenue) {
        int id = nextId++;
        customers[id] = {id, revenue};
        revenueSet.insert({revenue, id});
        return id;
    }

    // Insert a new customer with referral
    int insert(int revenue, int referralId) {
        int id = insert(revenue); // create new customer first

        // Update referrer's revenue if exists
        if (customers.count(referralId)) {
            auto& ref = customers[referralId];
            // remove old entry from set
            revenueSet.erase(revenueSet.find({ref.revenue, referralId}));
            ref.revenue += revenue;
            // re-insert updated
            revenueSet.insert({ref.revenue, referralId});
        }
        return id;
    }

    // Return top k customers (by revenue) under a given threshold
    vector<int> get(int k, int threshold) {
        vector<int> result;
        // find first element >= threshold
        auto it = revenueSet.lower_bound({threshold, INT_MIN});
        // iterate backwards to collect top k below threshold
        while (it != revenueSet.begin() && result.size() < k) {
            --it;
            result.push_back(it->second);
        }
        return result;
    }

    // For debugging
    void printAll() {
        cout << "Customers: \n";
        for (auto& [id, c] : customers)
            cout << "  ID=" << id << " revenue=" << c.revenue << "\n";
    }
};
