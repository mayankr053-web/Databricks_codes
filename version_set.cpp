#include <iostream>
#include <list>
#include <unordered_map>
#include <functional>

template<typename T>
class CustomSet {
private:
    struct Node {
        T value;
        size_t add_version;    // when node becomes visible to iterations
        size_t remove_version; // when node becomes invisible (0 if not removed)
        bool present;          // immediate presence for contains()
    };

    std::list<Node> nodes;
    std::unordered_map<T, typename std::list<Node>::iterator> lookup;

    size_t version = 1;           // current committed version
    size_t active_iterators = 0;  // how many snapshots are active

    // helper to gc nodes when safe
    void garbage_collect_if_safe() {
        if (active_iterators != 0) return; // only safe when no active iterators
        for (auto it = nodes.begin(); it != nodes.end(); ) {
            if (!it->present) {
                lookup.erase(it->value);
                it = nodes.erase(it);
            } else ++it;
        }
    }

public:
    CustomSet() = default;

    bool contains(const T &value) const {
        auto it = lookup.find(value);
        if (it == lookup.end()) return false;
        return it->second->present;
    }

    void add(const T &value) {
        auto it = lookup.find(value);
        if (it == lookup.end()) {
            // new node
            size_t av = (active_iterators > 0 ? version + 1 : version);
            nodes.push_back(Node{value, av, 0, true});
            lookup[value] = std::prev(nodes.end());
        } else {
            auto nit = it->second;
            if (nit->present) return; // already present
            // resurrect
            nit->present = true;
            nit->remove_version = 0;
            nit->add_version = (active_iterators > 0 ? version + 1 : version);
        }
    }

    void remove(const T &value) {
        auto it = lookup.find(value);
        if (it == lookup.end()) return;
        auto nit = it->second;
        if (!nit->present) return; // already removed

        nit->present = false;
        nit->remove_version = (active_iterators > 0 ? version + 1 : version);
    }

    // Begin an iteration session and return the snapshot version to iterate over.
    size_t beginIteration() {
        ++active_iterators;
        return version; // snapshot V = current committed version
    }

    // End iteration session.
    void endIteration() {
        if (active_iterators == 0) return;
        --active_iterators;
        // when the last active iterator finishes, we commit pending scheduled changes
        // by bumping version so version+1 scheduled changes become visible.
        if (active_iterators == 0) {
            ++version;
            garbage_collect_if_safe();
        }
    }

    // Iterate over snapshot_version V, applying Fn(value) for visible elements.
    template<typename Fn>
    void iterate_snapshot(size_t snapshot_version, Fn &&fn) const {
        for (const auto &node : nodes) {
            if (node.add_version <= snapshot_version && (node.remove_version == 0 || node.remove_version > snapshot_version)) {
                fn(node.value);
            }
        }
    }

    // RAII Snapshot object to manage begin/end automatically.
    class Snapshot {
        CustomSet<T> &parent;
        size_t snap_ver;
        bool active;
    public:
        Snapshot(CustomSet<T> &p) : parent(p), snap_ver(parent.beginIteration()), active(true) {}
        ~Snapshot() { if (active) parent.endIteration(); }
        Snapshot(const Snapshot&) = delete;
        Snapshot& operator=(const Snapshot&) = delete;
        Snapshot(Snapshot&& o) noexcept : parent(o.parent), snap_ver(o.snap_ver), active(o.active) { o.active = false; }
        size_t version() const { return snap_ver; }

        template<typename Fn>
        void for_each(Fn &&fn) const {
            parent.iterate_snapshot(snap_ver, std::forward<Fn>(fn));
        }
    };
};
