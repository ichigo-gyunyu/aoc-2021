#include <chrono>
#include <iomanip>
#include <iostream>
#include <queue>
#include <vector>

// bfs
int64_t part1(std::vector<std::vector<int>> cave) {
    int n = cave.size();
    int m = cave[0].size();

    const int MAX = std::numeric_limits<int>::max();
    struct node {
        int i;
        int j;
        int dist;
    };
    std::vector<std::vector<int>> mem(n, std::vector<int>(m, MAX));
    std::queue<node> q;
    q.push({0, 0, 0});
    mem[0][0] = 0;

    auto checkidx = [&](int i, int j) {
        return i >= 0 && j >= 0 && i < n && j < m;
    };

    auto updateq = [&](int row, int col, int d) {
        if (checkidx(row, col)) {
            int dist = d + cave[row][col];
            if (dist < mem[row][col]) {
                q.push({row, col, dist});
                mem[row][col] = dist;
            }
        }
    };

    while (!q.empty()) {
        int r, c, d;
        r = q.front().i;
        c = q.front().j;
        d = q.front().dist;
        q.pop();

        for (int dir : {1, -1}) {
            // left and right
            int row = r;
            int col = c + dir;
            updateq(row, col, d);

            // up and down
            row = r + dir;
            col = c;
            updateq(row, col, d);
        }
    }

    return mem[n - 1][m - 1];
}

int64_t part2(std::vector<std::vector<int>> cave) {
    int n = 5 * cave.size();
    int m = 5 * cave[0].size();

    const int MAX = std::numeric_limits<int>::max();
    struct node {
        int i;
        int j;
        int dist;
    };
    std::vector<std::vector<int>> mem(n, std::vector<int>(m, MAX));
    std::queue<node> q;
    q.push({0, 0, 0});
    mem[0][0] = 0;

    auto get_risk = [&](int i, int j) {
        int r = n / 5;
        int c = m / 5;
        int off = i / r + j / c;
        int risk = cave[i % r][j % c] + off;
        if (risk > 9)
            risk -= 9;
        return risk;
    };

    auto checkidx = [&](int i, int j) {
        return i >= 0 && j >= 0 && i < n && j < m;
    };

    auto updateq = [&](int row, int col, int d) {
        if (checkidx(row, col)) {
            int dist = d + get_risk(row, col);
            if (dist < mem[row][col]) {
                q.push({row, col, dist});
                mem[row][col] = dist;
            }
        }
    };

    while (!q.empty()) {
        int r, c, d;
        r = q.front().i;
        c = q.front().j;
        d = q.front().dist;
        q.pop();

        for (int dir : {1, -1}) {
            // left and right
            int row = r;
            int col = c + dir;
            updateq(row, col, d);

            // up and down
            row = r + dir;
            col = c;
            updateq(row, col, d);
        }
    }

    return mem[n - 1][m - 1];
}

int main() {
    std::string in;
    std::vector<std::vector<int>> cave;
    while (std::cin >> in) {
        std::vector<int> t;
        for (auto c : in)
            t.push_back(c - '0');
        cave.push_back(t);
    }

    auto start = std::chrono::high_resolution_clock::now();
    int64_t part1_ans = part1(cave);
    auto end = std::chrono::high_resolution_clock::now();
    auto time1 =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();

    start = std::chrono::high_resolution_clock::now();
    int64_t part2_ans = part2(cave);
    end = std::chrono::high_resolution_clock::now();
    auto time2 =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();

    std::cout << "\nPart 1: " << part1_ans << "\nTime: " << time1 << "ms"
              << std::endl;
    std::cout << "\nPart 2: " << part2_ans << "\nTime: " << time2 << "ms"
              << std::endl;
}
