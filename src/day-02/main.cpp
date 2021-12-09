#include <chrono>
#include <iostream>

int part1() { return 0; }

int part2() { return 0; }

int main() {

  auto start = std::chrono::high_resolution_clock::now();
  int part1_ans = part1();
  auto end = std::chrono::high_resolution_clock::now();
  auto time1 =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();

  start = std::chrono::high_resolution_clock::now();
  int part2_ans = part2();
  end = std::chrono::high_resolution_clock::now();
  auto time2 =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();

  std::cout << "\nPart 1: " << part1_ans << "\nTime: " << time1 << std::endl;
  std::cout << "\nPart 2: " << part2_ans << "\nTime: " << time2 << std::endl;
}
