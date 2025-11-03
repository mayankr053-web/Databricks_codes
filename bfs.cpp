#include <bits/stdc++.h>
using namespace std;

struct Point {
  int r, c;
};

int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

int bfs(vector<vector<char>>& grid, char mode) {
    queue<pair<Point, int>> q;
    Point dest;
    vector<vector<bool>> vis(grid.size(), vector<bool>(grid[0].size(), false));

    for(int i = 0; i < grid.size(); i++) {
        for(int j = 0; j < grid[0].size(); j++) {
            if(grid[i][j] == 'S') {
                q.push({{i, j}, 0});
                vis[i][j] = true;
            }
            if(grid[i][j] == 'D') {
                dest = {i, j};
            }
        }
    }

    while(!q.empty()) {
        auto [p, dist] = q.front();
        q.pop();

        if(p.r == dest.r && p.c == dest.c)
            return dist;

        for(int k = 0; k < 4; k++) {
            int r = p.r + dx[k];
            int c = p.c + dy[k];
            if(r < 0 || c < 0 || r >= grid.size() || c >= grid[0].size()) continue;
            if(vis[r][c] || grid[r][c] == 'X') continue;
            if(grid[r][c] == mode || grid[r][c] == 'D') {
                vis[r][c] = true;
                q.push({{r, c}, dist + 1});
            }
        }
    }
    return -1; // unreachable
}

void calculate(vector<vector<char>>& grid, vector<int>& cost, vector<int>& t) {
    int ans = INT_MAX;
    int cost_min = INT_MAX;
    char best_mode = '-';

    for(char mode = '1'; mode <= '4'; mode++) {
        int steps = bfs(grid, mode);
        if(steps == -1) continue;
        int total_time = steps * t[mode - '0'];

        if(total_time < ans) {
            ans = total_time;
            cost_min = cost[mode - '0'];
            best_mode = mode;
        }
        else if(total_time == ans && cost[mode - '0'] < cost_min) {
            cost_min = cost[mode - '0'];
            best_mode = mode;
        }
    }

    if(ans == INT_MAX)
        cout << "No path exists using any mode.\n";
    else
        cout << "Fastest mode: " << best_mode
             << " | Total time: " << ans
             << " | Cost: " << cost_min << endl;
}

int main() {
    vector<vector<char>> grid = {
        {'1','2','3','S'},
        {'1','2','4','X'},
        {'2','3','2','1'},
        {'X','2','2','1'},
        {'1','D','2','X'}
    };

    // index 0 unused, since modes are '1'–'4'
    vector<int> t = {0, 1, 2, 3, 4};
    vector<int> cost = {0, 10, 15, 20, 25};

    calculate(grid, cost, t);
}
