#include <array>
#include <chrono>
#include <iostream>
#include <queue>

#define N 10

int64_t part1(std::array<std::array<int, N>, N> octopuses) {
  int glowies = 0;

  for (int iter = 0; iter < 100; iter++) {

    std::array<std::array<bool, N>, N> glowboard = {};
    std::queue<std::pair<int, int>> to_glow;

    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        octopuses[i][j]++;
        if (octopuses[i][j] >= 10)
          glowboard[i][j] = true, to_glow.emplace(i, j);
      }
    }

    if (to_glow.empty())
      continue;

    while (!to_glow.empty()) {
      int row = to_glow.front().first;
      int col = to_glow.front().second;
      to_glow.pop();
      glowies++;

      for (int ver : {-1, 0, 1}) {
        for (int hor : {-1, 0, 1}) {
          int i = row + ver;
          int j = col + hor;

          if (i >= 0 && i < N && j >= 0 && j < N) {
            octopuses[i][j]++;
            if (octopuses[i][j] >= 10 && !glowboard[i][j])
              glowboard[i][j] = true, to_glow.emplace(i, j);
          }
        }
      }
    }

    // reset
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        if (octopuses[i][j] >= 10)
          octopuses[i][j] = 0;
      }
    }
  }

  return glowies;
}

int64_t part2(std::array<std::array<int, N>, N> octopuses) {

  for (int iter = 0; iter < 100000; iter++) {

    int glowies = 0;
    std::array<std::array<bool, N>, N> glowboard = {};
    std::queue<std::pair<int, int>> to_glow;

    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        octopuses[i][j]++;
        if (octopuses[i][j] >= 10)
          glowboard[i][j] = true, to_glow.emplace(i, j);
      }
    }

    if (to_glow.empty())
      continue;

    while (!to_glow.empty()) {
      int row = to_glow.front().first;
      int col = to_glow.front().second;
      to_glow.pop();
      glowies++;

      for (int ver : {-1, 0, 1}) {
        for (int hor : {-1, 0, 1}) {
          int i = row + ver;
          int j = col + hor;

          if (i >= 0 && i < N && j >= 0 && j < N) {
            octopuses[i][j]++;
            if (octopuses[i][j] >= 10 && !glowboard[i][j])
              glowboard[i][j] = true, to_glow.emplace(i, j);
          }
        }
      }
    }

    if (glowies == N * N)
      return iter + 1;

    // reset
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        if (octopuses[i][j] >= 10)
          octopuses[i][j] = 0;
      }
    }
  }
  return -1;
}

int main() {
  std::string in;
  std::array<std::array<int, N>, N> octopuses;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      char c;
      std::cin >> c;
      octopuses[i][j] = c - '0';
    }
  }

  auto start = std::chrono::high_resolution_clock::now();
  int64_t part1_ans = part1(octopuses);
  auto end = std::chrono::high_resolution_clock::now();
  auto time1 =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();

  start = std::chrono::high_resolution_clock::now();
  int64_t part2_ans = part2(octopuses);
  end = std::chrono::high_resolution_clock::now();
  auto time2 =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();

  std::cout << "\nPart 1: " << part1_ans << "\nTime: " << time1 << "ms"
            << std::endl;
  std::cout << "\nPart 2: " << part2_ans << "\nTime: " << time2 << "ms"
            << std::endl;
}
