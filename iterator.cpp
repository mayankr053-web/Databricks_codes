#include <bits/stdc++.h>
using namespace std;

class CustomSet {
private:
    unordered_set<int> data; // main data storage

public:
    // Add element to the set
    bool add(int i) {
        return data.insert(i).second; // returns true if insertion happened
    }

    // Remove element from the set
    bool remove(int i) {
        return data.erase(i) > 0; // returns true if element existed
    }

    // Check if element exists in the set
    bool contains(int i) const {
        return data.find(i) != data.end();
    }

    // Snapshot iterator class
    class Iterator {
    private:
        vector<int> snapshot;
        size_t index;

    public:
        Iterator(const unordered_set<int>& currentSet) {
            // Take snapshot of current elements
            snapshot.assign(currentSet.begin(), currentSet.end());
            index = 0;
        }

        bool hasNext() const {
            return index < snapshot.size();
        }

        int next() {
            return snapshot[index++];
        }
    };

    // Returns a snapshot iterator over current elements
    Iterator iterator() const {
        return Iterator(data);
    }

    // Print all elements using snapshot iterator
    void printAll() const {
        Iterator it = iterator();
        cout << "[";
        bool first = true;
        while (it.hasNext()) {
            if (!first) cout << ", ";
            cout << it.next();
            first = false;
        }
        cout << "]" << endl;
    }
};

int main() {
    CustomSet s;
    s.add(2);
    s.add(3);
    s.add(4);
    s.remove(3);

    auto it = s.iterator(); // snapshot: [2, 4]

    s.add(5);
    cout << boolalpha << "contains(5): " << s.contains(5) << endl; // true

    s.remove(5);
    s.add(6);
    s.remove(6);
    s.add(6);

    cout << boolalpha << "contains(5): " << s.contains(5) << endl; // false

    // Iteration should reflect snapshot [2, 4]
    cout << "iteration result -> [";
    bool first = true;
    while (it.hasNext()) {
        if (!first) cout << ", ";
        cout << it.next();
        first = false;
    }
    cout << "]" << endl;

    // Current state reflects live set
    cout << "current state -> ";
    s.printAll();

    return 0;
}
