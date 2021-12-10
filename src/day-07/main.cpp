#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

int64_t part1(std::vector<int> positions) {
  size_t n = positions.size();
  std::sort(positions.begin(), positions.end());
  int median = positions[n / 2];

  int fuel = 0;
  for (int p : positions) {
    fuel += abs(p - median);
  }

  return fuel;
}

// Input vector A, size n
// O(n * rangeA)
int64_t part2(std::vector<int> positions) {
  int start = *std::min_element(positions.begin(), positions.end());
  int end = *std::max_element(positions.begin(), positions.end());

  // 1 + 2 + 3 + ... + x
  auto f = [](int x) { return (x * x + x) / 2; };

  int best_fuel = std::numeric_limits<int>::max();
  for (int i = start; i <= end; i++) {
    int fuel = 0;
    for (int p : positions) {
      fuel += f(abs(i - p));
    }
    best_fuel = std::min(fuel, best_fuel);
  }

  return best_fuel;
}

// O(n), using bounds on the derivative
int64_t part2_2(std::vector<int> positions) {
  size_t n = positions.size();
  int sum = std::accumulate(positions.begin(), positions.end(), 0);
  double av = sum / (double)n;
  int lo = av - 0.5;
  int hi = av + 1.5;

  // 1 + 2 + 3 + ... + x
  auto f = [](int x) { return (x * x + x) / 2; };

  int best_fuel = std::numeric_limits<int>::max();
  for (int i = lo; i <= hi; i++) {
    int fuel = 0;
    for (int p : positions) {
      fuel += f(abs(i - p));
    }
    best_fuel = std::min(fuel, best_fuel);
  }

  return best_fuel;
}

int main() {
  std::string input;
  std::cin >> input;
  std::stringstream ss(input);
  std::vector<int> positions;
  while (ss.good()) {
    std::string sub;
    std::getline(ss, sub, ',');
    positions.push_back(stoi(sub));
  }

  auto start = std::chrono::high_resolution_clock::now();
  int64_t part1_ans = part1(positions);
  auto end = std::chrono::high_resolution_clock::now();
  auto time1 =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();

  start = std::chrono::high_resolution_clock::now();
  int64_t part2_ans = part2_2(positions);
  end = std::chrono::high_resolution_clock::now();
  auto time2 =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();

  std::cout << "\nPart 1: " << part1_ans << "\nTime: " << time1 << "ms"
            << std::endl;
  std::cout << "\nPart 2: " << part2_ans << "\nTime: " << time2 << "ms"
            << std::endl;
}
