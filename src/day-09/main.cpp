#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <queue>
#include <vector>

int64_t part1(const std::vector<std::string> &heightmap) {
  size_t n = heightmap.size();
  size_t m = heightmap[0].size();
  int risk = 0;

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < m; j++) {
      char c = heightmap[i][j];
      char nearby = '9';

      for (int dir : {1, -1}) {
        size_t row = i + dir;
        size_t col = j;
        if (row >= 0 && row < n && col >= 0 && col < m)
          nearby = std::min(nearby, heightmap[row][col]);

        row = i;
        col = j + dir;
        if (row >= 0 && row < n && col >= 0 && col < m)
          nearby = std::min(nearby, heightmap[row][col]);
      }

      risk += (c < nearby) * (c - '0' + 1);
    }
  }

  return risk;
}

int64_t part2(const std::vector<std::string> &heightmap) {
  size_t n = heightmap.size();
  size_t m = heightmap[0].size();
  std::vector<std::pair<int, int>> low_points;

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < m; j++) {
      char c = heightmap[i][j];
      char nearby = '9';

      for (int dir : {1, -1}) {
        size_t row = i + dir;
        size_t col = j;
        if (row >= 0 && row < n && col >= 0 && col < m)
          nearby = std::min(nearby, heightmap[row][col]);

        row = i;
        col = j + dir;
        if (row >= 0 && row < n && col >= 0 && col < m)
          nearby = std::min(nearby, heightmap[row][col]);
      }

      if (c < nearby)
        low_points.emplace_back(i, j);
    }
  }

  std::vector<int> sizes;
  // BFS for every low point
  std::vector<std::vector<bool>> visited(n, std::vector<bool>(m, false));
  for (auto p : low_points) {
    int64_t sz = 0;
    std::queue<std::pair<int, int>> q;
    visited[p.first][p.second] = true;
    q.push(p);

    while (!q.empty()) {
      int i = q.front().first;
      int j = q.front().second;
      q.pop();
      sz++;

      for (int dir : {1, -1}) {
        size_t row = i + dir;
        size_t col = j;
        if (row >= 0 && row < n && col >= 0 && col < m &&
            heightmap[row][col] != '9' && !visited[row][col])
          q.emplace(row, col), visited[row][col] = true;

        row = i;
        col = j + dir;
        if (row >= 0 && row < n && col >= 0 && col < m &&
            heightmap[row][col] != '9' && !visited[row][col])
          q.emplace(row, col), visited[row][col] = true;
      }
    }
    sizes.push_back(sz);
  }

  sort(sizes.begin(), sizes.end(), std::greater<int>());
  return sizes[0] * sizes[1] * sizes[2];
}

int main() {
  std::string in;
  std::vector<std::string> heightmap;
  while (std::cin >> in) {
    heightmap.push_back(in);
  }

  auto start = std::chrono::high_resolution_clock::now();
  int64_t part1_ans = part1(heightmap);
  auto end = std::chrono::high_resolution_clock::now();
  auto time1 =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();

  start = std::chrono::high_resolution_clock::now();
  int64_t part2_ans = part2(heightmap);
  end = std::chrono::high_resolution_clock::now();
  auto time2 =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();

  std::cout << "\nPart 1: " << part1_ans << "\nTime: " << time1 << "ms"
            << std::endl;
  std::cout << "\nPart 2: " << part2_ans << "\nTime: " << time2 << "ms"
            << std::endl;
}
