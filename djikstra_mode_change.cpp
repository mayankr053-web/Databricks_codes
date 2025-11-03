#include <bits/stdc++.h>
using namespace std;

struct Point {
    int r, c;
    int cost; // total cost so far
    int tm;   // total time so far
    char mode; // current mode ('1'..'4'), 'N' means none yet
};

int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

struct compare {
    bool operator()(const Point &a, const Point &b) const {
        if (a.tm == b.tm)
            return a.cost > b.cost;
        return a.tm > b.tm;
    }
};

void calculate(vector<vector<char>> &grid, vector<int> &cost, vector<int> &tm, int penalty) {
    int m = grid.size();
    int n = grid[0].size();
    int ans = INT_MAX;
    int best_cost = INT_MAX;
    char best_mode = 'N';

    vector<vector<int>> dist(m, vector<int>(n, INT_MAX));
    int dest_r = -1, dest_c = -1;

    priority_queue<Point, vector<Point>, compare> pq;

    // Initialize with S
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == 'S') {
                pq.push({i, j, 0, 0, 'N'});
                dist[i][j] = 0;
            }
            if (grid[i][j] == 'D') {
                dest_r = i;
                dest_c = j;
            }
        }
    }

    while (!pq.empty()) {
        auto front = pq.top();
        pq.pop();

        int r0 = front.r, c0 = front.c;

        // Already found better
        if (front.tm > dist[r0][c0]) continue;

        // Reached destination
        if (r0 == dest_r && c0 == dest_c) {
            if (front.tm < ans || (front.tm == ans && front.cost < best_cost)) {
                ans = front.tm;
                best_cost = front.cost;
                best_mode = front.mode;
            }
            continue;
        }

        // Explore neighbors
        for (int k = 0; k < 4; k++) {
            int r = r0 + dx[k];
            int c = c0 + dy[k];
            if (r < 0 || r >= m || c < 0 || c >= n) continue;
            if (grid[r][c] == 'X') continue;

            char nextCell = grid[r][c];

            // Skip invalid transitions
            if (nextCell == 'S') continue;

            int new_time = front.tm;
            int new_cost = front.cost;
            char new_mode = front.mode;

            if (nextCell == 'D') {
                // Moving into D (use current mode time)
                if (front.mode == 'N') continue; // Can't move without having started a mode
                new_time += tm[front.mode - '0'];
                // cost doesn’t change entering D
            } else {
                // Cell has a mode
                char cellMode = nextCell;
                int modeIdx = cellMode - '0';

                if (front.mode == 'N') {
                    // First mode adoption
                    new_mode = cellMode;
                    new_time += tm[modeIdx];
                    new_cost += cost[modeIdx];
                } else if (front.mode == cellMode) {
                    // Same mode
                    new_time += tm[modeIdx];
                    new_cost += cost[modeIdx];
                } else {
                    // Different mode → switch + move
                    new_mode = cellMode;
                    new_time += tm[modeIdx] + penalty;
                    new_cost += cost[modeIdx];
                }
            }

            if (new_time < dist[r][c]) {
                dist[r][c] = new_time;
                pq.push({r, c, new_cost, new_time, new_mode});
            }
        }
    }

    if (ans == INT_MAX)
        cout << "No path exists.\n";
    else
        cout << "Fastest time: " << ans
             << " | Lowest cost: " << best_cost
             << " | Final mode: " << best_mode << endl;
}

int main() {
    vector<vector<char>> grid = {
        {'1', '2', '3', 'S'},
        {'1', '2', '4', 'X'},
        {'2', '3', '2', '1'},
        {'X', '2', '2', '1'},
        {'1', 'D', '2', 'X'}
    };

    vector<int> time = {0, 1, 2, 3, 4}; // per-mode travel time
    vector<int> cost = {0, 10, 15, 20, 25}; // per-mode cost
    int penalty = 5;

    calculate(grid, cost, time, penalty);
}
