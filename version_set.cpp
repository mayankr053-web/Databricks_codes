#include <iostream>
#include <unordered_map>
#include <mutex>
#include <limits>

template<typename T>
class VersionedSet {
    struct Node {
        T value;
        uint64_t insert_ver;
        uint64_t delete_ver;
        Node* next;
        Node(const T& v, uint64_t ins)
            : value(v), insert_ver(ins),
              delete_ver(std::numeric_limits<uint64_t>::max()),
              next(nullptr) {}
    };

    Node* head;    // dummy sentinel
    Node* tail;
    std::unordered_map<T, Node*> map;
    uint64_t version = 0;  // global version counter
    std::mutex mtx;

public:
    VersionedSet() {
        head = new Node(T{}, 0);  // sentinel node
        tail = head;
    }

    ~VersionedSet() {
        Node* cur = head;
        while (cur) {
            Node* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
    }

    // ---------------- Core Operations ----------------

    bool add(const T& val) {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = map.find(val);
        if (it != map.end()) {
            Node* node = it->second;
            if (node->delete_ver == std::numeric_limits<uint64_t>::max())
                return false; // already active
        }
        uint64_t ver = ++version;
        Node* node = new Node(val, ver);
        tail->next = node;
        tail = node;
        map[val] = node;
        return true;
    }

    bool remove(const T& val) {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = map.find(val);
        if (it == map.end())
            return false;
        Node* node = it->second;
        if (node->delete_ver != std::numeric_limits<uint64_t>::max())
            return false; // already deleted
        node->delete_ver = ++version;
        map.erase(it);
        return true;
    }

    bool contains(const T& val) {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = map.find(val);
        return (it != map.end());
    }

    // ---------------- Iteration ----------------

    void iterateSnapshot() {
        std::lock_guard<std::mutex> lock(mtx);
        uint64_t snap = version;
        std::cout << "[Snapshot version " << snap << "]: ";
        for (Node* cur = head->next; cur; cur = cur->next) {
            if (cur->insert_ver <= snap && cur->delete_ver > snap)
                std::cout << cur->value << " ";
        }
        std::cout << "\n";
    }

    // ---------------- Debug Helpers ----------------

    void printLive() {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Live set: ";
        for (auto& [key, _] : map)
            std::cout << key << " ";
        std::cout << "\n";
    }
};

int main() {
    VersionedSet<int> vs;
    vs.add(1);
    vs.add(2);
    vs.add(3);

    vs.iterateSnapshot();   // Snapshot 3: 1 2 3

    vs.remove(3);
    vs.add(4);
    vs.iterateSnapshot();   // Snapshot 5: 1 2 4

    vs.add(5);
    vs.remove(2);
    vs.iterateSnapshot();   // Snapshot 7: 1 4 5

    vs.printLive();         // Live set: 1 4 5
}
