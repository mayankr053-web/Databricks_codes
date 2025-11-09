#include <iostream>
#include <unordered_map>
#include <vector>
#include <climits>

class CustomSet {
private:
    struct Entry {
        bool present;
        size_t versionAdded;
        size_t versionRemoved;
        Entry(size_t added) : present(true), versionAdded(added), versionRemoved(SIZE_MAX) {}
    };

    std::unordered_map<int, Entry*> elements;
    size_t currentVersion = 0;

public:
    ~CustomSet() {
        for (auto& p : elements)
            delete p.second;
    }

    bool add(int value) {
        ++currentVersion;
        auto it = elements.find(value);
        if (it == elements.end()) {
            elements[value] = new Entry(currentVersion);
            return true;
        }
        Entry* e = it->second;
        if (!e->present) {
            e->present = true;
            e->versionAdded = currentVersion;
            e->versionRemoved = SIZE_MAX;
            return true;
        }
        return false;
    }

    bool remove(int value) {
        auto it = elements.find(value);
        if (it == elements.end() || !it->second->present)
            return false;
        ++currentVersion;
        it->second->present = false;
        it->second->versionRemoved = currentVersion;
        return true;
    }

    bool contains(int value) const {
        auto it = elements.find(value);
        return it != elements.end() && it->second->present;
    }

    class Iterator {
    private:
        std::vector<int> snapshot;
        size_t idx = 0;

    public:
        Iterator(const std::unordered_map<int, Entry*>& map, size_t version) {
            snapshot.reserve(map.size());
            for (auto& p : map) {
                Entry* e = p.second;
                if (e->versionAdded <= version && e->versionRemoved > version)
                    snapshot.push_back(p.first);
            }
        }

        bool hasNext() const {
            return idx < snapshot.size();
        }

        int next() {
            return snapshot[idx++];
        }
    };

    Iterator iterator() const {
        return Iterator(elements, currentVersion);
    }

    void printAll() const {
        Iterator it = iterator();
        while (it.hasNext())
            std::cout << it.next() << " ";
        std::cout << "\n";
    }
};

// ---------------- Example Usage ----------------
int main() {
    CustomSet cs;
    cs.add(10);
    cs.add(20);
    cs.add(30);

    std::cout << "Initial elements: ";
    cs.printAll();

    auto it = cs.iterator(); // snapshot iterator

    cs.add(40);
    cs.remove(10);

    std::cout << "Iterating snapshot: ";
    while (it.hasNext()) {
        std::cout << it.next() << " ";
    }
    std::cout << "\n";

    std::cout << "After modification: ";
    cs.printAll();

    return 0;
}
