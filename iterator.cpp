#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>

class CustomSet {
private:
    int currVersion = 0;

    // Tracks presence of elements for each version
    // element -> vector of (version, isPresent)
    std::unordered_map<int, std::vector<std::pair<int, bool>>> history;

    // Fast access for current version
    std::unordered_set<int> currentSet;

public:
    // Add element
    bool add(int x) {
        if (currentSet.count(x)) return false;

        currentSet.insert(x);
        history[x].push_back({++currVersion, true});
        return true;
    }

    // Remove element
    bool remove(int x) {
        if (!currentSet.count(x)) return false;

        currentSet.erase(x);
        history[x].push_back({++currVersion, false});
        return true;
    }

    // Check containment (current state)
    bool contains(int x) const {
        return currentSet.count(x);
    }

    // Take a snapshot: returns a version number
    int takeSnapshot() {
        return currVersion;
    }

    // Iterator snapshot type
    class Iterator {
    private:
        std::vector<int> elements;
        size_t idx = 0;

    public:
        Iterator(std::vector<int> elems) : elements(std::move(elems)) {}
        bool hasNext() const { return idx < elements.size(); }
        int next() { return elements[idx++]; }
    };

    // Create iterator for snapshot version
    Iterator iterator(int version = -1) const {
        if (version == -1)
            version = currVersion; // default: current version

        std::vector<int> snapshotElements;
        for (auto& [elem, logs] : history) {
            // binary search for latest <= version
            int l = 0, r = (int)logs.size() - 1;
            bool present = false;
            while (l <= r) {
                int m = (l + r) / 2;
                if (logs[m].first <= version) {
                    present = logs[m].second;
                    l = m + 1;
                } else {
                    r = m - 1;
                }
            }
            if (present)
                snapshotElements.push_back(elem);
        }

        return Iterator(snapshotElements);
    }

    // Print all elements for snapshot
    void printAll(int version = -1) const {
        auto it = iterator(version);
        while (it.hasNext()) {
            std::cout << it.next() << " ";
        }
        std::cout << std::endl;
    }
};
