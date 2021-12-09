#include <array>
#include <chrono>
#include <iostream>
#include <sstream>
#include <vector>

#define DAYS1 80
#define DAYS2 256

// 1. Naive Simulation

int64_t part1(std::vector<int> &fishes) {
  for (int i = 0; i < DAYS1; i++) {
    int babies = 0;
    for (int &f : fishes) {
      f--;
      babies += (f == -1); // save a branch lol
      f = (f != -1) ? f : 6;
    }
    fishes.insert(fishes.end(), babies, 8);
  }

  return fishes.size();
}

// O(DAYS^2)
// can be O(DAYS) with some precomputation

int64_t part2(const std::vector<int> &fishes) {
  // dp[x]: if a lanternfish is created on day x
  // dp[x] stores the total number at the end of DAYS
  std::array<int64_t, DAYS2 + 1> dp = {};

  for (int i = DAYS2; i > 0; i--) {
    dp[i] = 1;
    int idx = i + 9;
    while (idx <= DAYS2) {
      dp[i] += dp[idx];
      idx += 7;
    }
  }

  // since the input can take in only 6 different values,
  // we can precompute the answer for them
  std::array<int64_t, 7> pc = {};
  for (int i = 1; i <= 6; i++) {
    pc[i] = 1;
    int idx = i + 1;
    while (idx <= DAYS2) {
      pc[i] += dp[idx];
      idx += 7;
    }
  }

  int64_t count = 0;
  for (auto f : fishes) {
    count += pc[f];
  }

  return count;
}

int main() {
  std::string input;
  std::cin >> input;
  std::stringstream ss(input);
  std::vector<int> lanternfishes;

  while (ss.good()) {
    std::string sub;
    std::getline(ss, sub, ',');
    lanternfishes.push_back(stoi(sub));
  }

  auto lf2 = lanternfishes;

  auto start = std::chrono::high_resolution_clock::now();
  int64_t part1_ans = part1(lanternfishes);
  auto end = std::chrono::high_resolution_clock::now();
  auto time1 =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();

  start = std::chrono::high_resolution_clock::now();
  int64_t part2_ans = part2(lf2);
  end = std::chrono::high_resolution_clock::now();
  auto time2 =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();

  std::cout << "\nPart 1: " << part1_ans << "\nTime: " << time1 << "ms"
            << std::endl;
  std::cout << "\nPart 2: " << part2_ans << "\nTime: " << time2 << "ms"
            << std::endl;
}
