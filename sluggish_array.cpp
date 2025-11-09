#include <bits/stdc++.h>
using namespace std;

class SluggishArray {
    vector<int> arr;
    function<int(int)> composed; // the composed transformation

public:
    // Constructor
    SluggishArray(const vector<int>& a)
        : arr(a), composed([](int x) { return x; }) {} // start with identity

    // Apply a new map transformation lazily
    SluggishArray& map(function<int(int)> f) {
        // Compose existing transformation with the new one
        auto prev = composed;
        composed = [prev, f](int x) { return f(prev(x)); };
        return *this;
    }

    // Get index of a value after applying all transformations lazily
    int getIndexOf(int value) {
        for (int i = 0; i < (int)arr.size(); i++) {
            int transformed = composed(arr[i]);
            if (transformed == value)
                return i;  // stop early — first match
        }
        return -1; // not found
    }
};
