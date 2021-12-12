#include <chrono>
#include <iostream>
#include <map>
#include <queue>
#include <unordered_set>
#include <vector>

int64_t part1(const std::vector<std::vector<int>> adj_list,
              const std::unordered_set<int> small_caves, const int n) {
  int num_paths = 0;
  std::vector<bool> visited(n, false);
  visited[0] = true;
  std::queue<std::pair<int, std::vector<bool>>> q;
  q.emplace(0, visited);

  // lot's of vector copying
  // fat and slow
  while (!q.empty()) {
    int u = q.front().first;
    auto visited = q.front().second;
    q.pop();

    // end
    if (u == 1) {
      num_paths++;
      continue;
    }

    for (int v : adj_list[u]) {
      auto vis_copy = visited;
      if (!vis_copy[v] || !small_caves.contains(v))
        vis_copy[v] = true, q.emplace(v, vis_copy);
    }
  }

  return num_paths;
}

int64_t part2(const std::vector<std::vector<int>> adj_list,
              const std::unordered_set<int> small_caves, const int n) {

  struct state {
    int u;
    std::vector<int> visited;
    bool twice;

    state(int u, std::vector<int> visited, bool twice)
        : u(u), visited(visited), twice(twice) {}
  };

  int num_paths = 0;
  std::vector<int> visited(n, 0);
  visited[0] = 2;
  std::queue<state> q;
  q.emplace(0, visited, false);

  // lot's of vector copying
  // fat and slow
  // even slower
  while (!q.empty()) {
    int u = q.front().u;
    auto visited = q.front().visited;
    bool twice = q.front().twice;
    q.pop();

    // end
    if (u == 1) {
      num_paths++;
      continue;
    }

    for (int v : adj_list[u]) {
      auto vis_copy = visited;
      auto twice_copy = twice;
      if (!small_caves.contains(v))
        vis_copy[v]++, q.emplace(v, vis_copy, twice_copy);
      else {
        int x = vis_copy[v];
        if (x >= 2 || (x == 1 && twice_copy))
          continue;
        if (x == 1)
          twice_copy = true;
        vis_copy[v]++;
        q.emplace(v, vis_copy, twice_copy);
      }
    }
  }

  return num_paths;
}

int main() {
  std::map<std::string, int> points;
  points["start"] = 0;
  points["end"] = 1;

  std::vector<std::pair<int, int>> edges;
  std::string in;
  int n = 2;
  std::unordered_set<int> small_caves;

  while (std::cin >> in) {
    size_t pos = in.find('-');
    std::string s1 = in.substr(0, pos);
    std::string s2 = in.substr(pos + 1);

    int v1, v2;
    if (!points.contains(s1))
      points[s1] = n++;
    if (!points.contains(s2))
      points[s2] = n++;
    v1 = points[s1], v2 = points[s2];

    if (s1 != "end" && std::islower(s1[0]))
      small_caves.insert(v1);
    if (s2 != "end" && std::islower(s2[0]))
      small_caves.insert(v2);

    edges.emplace_back(v1, v2);
  }

  std::vector<std::vector<int>> adj_list(n);
  for (auto e : edges) {
    adj_list[e.first].push_back(e.second);
    adj_list[e.second].push_back(e.first);
  }

  auto start = std::chrono::high_resolution_clock::now();
  int64_t part1_ans = part1(adj_list, small_caves, n);
  auto end = std::chrono::high_resolution_clock::now();
  auto time1 =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();

  start = std::chrono::high_resolution_clock::now();
  int64_t part2_ans = part2(adj_list, small_caves, n);
  end = std::chrono::high_resolution_clock::now();
  auto time2 =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();

  std::cout << "\nPart 1: " << part1_ans << "\nTime: " << time1 << "ms"
            << std::endl;
  std::cout << "\nPart 2: " << part2_ans << "\nTime: " << time2 << "ms"
            << std::endl;
}
