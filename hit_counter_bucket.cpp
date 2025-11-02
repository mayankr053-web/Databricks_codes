#include <bits/stdc++.h>
using namespace std;

class SubsecondBucketCounter {
private:
    const long long WINDOW_MS = 5LL * 60 * 1000;   // 5 minutes
    const int BUCKET_SIZE_MS = 100;                // 100 ms bucket
    const int NUM_BUCKETS = WINDOW_MS / BUCKET_SIZE_MS; // 3000 buckets

    vector<int> counts;           // number of hits in each bucket
    vector<long long> times;      // start time of each bucket (ms)

    long long nowMs() const {
        using namespace chrono;
        return duration_cast<milliseconds>(
            system_clock::now().time_since_epoch()
        ).count();
    }

public:
    SubsecondBucketCounter() : counts(NUM_BUCKETS, 0), times(NUM_BUCKETS, 0) {}

    void hit() {
        long long t = nowMs();
        long long bucketTime = (t / BUCKET_SIZE_MS) * BUCKET_SIZE_MS;
        int idx = (t / BUCKET_SIZE_MS) % NUM_BUCKETS;

        if (times[idx] != bucketTime) {
            // This bucket belongs to an old window, reset it.
            times[idx] = bucketTime;
            counts[idx] = 1;
        } else {
            counts[idx]++;
        }
    }

    long long getHits() {
        long long t = nowMs();
        long long total = 0;
        for (int i = 0; i < NUM_BUCKETS; ++i) {
            if (t - times[i] < WINDOW_MS) {
                total += counts[i];
            }
        }
        return total;
    }

    double getLoadPerSecond() {
        return (double)getHits() / (WINDOW_MS / 1000.0);
    }
};
