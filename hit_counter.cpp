#include <iostream>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <string>

using namespace std;
using namespace std::chrono;

class InstrumentedMap {
    struct Bucket {
        long long timestamp;  // epoch seconds
        int putCount;
        int getCount;
        Bucket() : timestamp(0), putCount(0), getCount(0) {}
    };

    unordered_map<string, string> data;
    vector<Bucket> buckets;  // circular buffer of 300 seconds
    static constexpr int WINDOW_SEC = 300; // 5 minutes

    long long nowSeconds() const {
        return duration_cast<seconds>(steady_clock::now().time_since_epoch()).count();
    }

    Bucket& getBucket(long long ts) {
        int idx = ts % WINDOW_SEC;
        Bucket &b = buckets[idx];
        // If this bucket is old (not current second), reset it
        if (b.timestamp != ts) {
            b.timestamp = ts;
            b.putCount = 0;
            b.getCount = 0;
        }
        return b;
    }

    double computeLoad(bool forPut) const {
        long long ts = duration_cast<seconds>(steady_clock::now().time_since_epoch()).count();
        int total = 0;
        for (const auto &b : buckets) {
            if (ts - b.timestamp < WINDOW_SEC) {
                total += (forPut ? b.putCount : b.getCount);
            }
        }
        return static_cast<double>(total) / WINDOW_SEC; // average per second
    }

public:
    InstrumentedMap() : buckets(WINDOW_SEC) {}

    void put(const string &key, const string &value) {
        data[key] = value;
        long long ts = nowSeconds();
        getBucket(ts).putCount++;
    }

    string get(const string &key) {
        long long ts = nowSeconds();
        getBucket(ts).getCount++;
        auto it = data.find(key);
        return (it != data.end()) ? it->second : "";
    }

    double measure_put_load() const {
        return computeLoad(true);
    }

    double measure_get_load() const {
        return computeLoad(false);
    }
};

int main() {
    InstrumentedMap m;
    m.put("x", "1");
    m.put("y", "2");
    m.get("x");
    m.get("y");
    m.get("z");

    cout << "Put load (avg/sec over 5 min): " << m.measure_put_load() << "\n";
    cout << "Get load (avg/sec over 5 min): " << m.measure_get_load() << "\n";

    // Simulate later calls
    std::this_thread::sleep_for(std::chrono::seconds(1));
    m.put("a", "100");
    m.get("a");

    cout << "Put load (avg/sec over 5 min): " << m.measure_put_load() << "\n";
    cout << "Get load (avg/sec over 5 min): " << m.measure_get_load() << "\n";
}
