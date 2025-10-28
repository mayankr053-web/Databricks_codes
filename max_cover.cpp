#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

struct Segment {
    int start, end; // end is exclusive
};

using Cover = vector<Segment>;

string reconstruct(const string& ref, const Cover& cover) {
    string result;
    for (auto& seg : cover)
        result += ref.substr(seg.start, seg.end - seg.start);
    return result;
}

// Delete character at logical index
Cover deleteChar(const string& ref, const Cover& cover, int index) {
    Cover result;
    int logical = 0;

    for (auto& seg : cover) {
        int seg_len = seg.end - seg.start;

        if (index < logical + seg_len) {
            // Deletion happens inside this segment
            int offset = index - logical;
            if (seg_len == 1) {
                // Skip entirely (deleted)
            } else if (offset == 0) {
                result.push_back({seg.start + 1, seg.end});
            } else if (offset == seg_len - 1) {
                result.push_back({seg.start, seg.end - 1});
            } else {
                // Split into two parts
                result.push_back({seg.start, seg.start + offset});
                result.push_back({seg.start + offset + 1, seg.end});
            }
        } else {
            result.push_back(seg);
        }
        logical += seg_len;
    }

    // --- Make it maximal ---
    Cover merged;
    for (auto& seg : result) {
        if (merged.empty() || merged.back().end != seg.start) {
            merged.push_back(seg);
        } else {
            merged.back().end = seg.end;
        }
    }

    return merged;
}

int main() {
    string ref = "abcdefg";
    Cover cover = {{0, 3}, {3, 5}}; // "abc" + "de" = "abcde"

    cout << "Original: " << reconstruct(ref, cover) << "\n";

    Cover newCover = deleteChar(ref, cover, 2); // remove 'c'

    cout << "After delete: " << reconstruct(ref, newCover) << "\n";

    cout << "New cover: ";
    for (auto& s : newCover)
        cout << "(" << s.start << "," << s.end << ") ";
    cout << "\n";
}
