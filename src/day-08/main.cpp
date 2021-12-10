#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

int64_t part1(const std::vector<std::array<std::string, 14>> strs) {
  int count = 0;

  for (auto s : strs) {
    // check the last 4 lengths
    for (int i = 10; i < 14; i++) {
      int n = s[i].length();
      count += (n == 2 || n == 3 || n == 4 || n == 7);
    }
  }

  return count;
}

int64_t part2(const std::vector<std::array<std::string, 14>> strs) {
  int total = 0;

  for (auto s : strs) {
    std::vector<std::string> len5, len6;
    char right1 = 'a', right2 = 'a';
    std::map<std::string, int> digit;
    std::string four;

    for (int i = 0; i < 10; i++) {
      int n = s[i].length();
      if (n == 5)
        len5.push_back(s[i]);
      else if (n == 6)
        len6.push_back(s[i]);
      else if (n == 2)
        right1 = s[i][0], right2 = s[i][1], digit[s[i]] = 1;
      else if (n == 4)
        digit[s[i]] = 4, four = s[i];
      else if (n == 3)
        digit[s[i]] = 7;
      else if (n == 7)
        digit[s[i]] = 8;
    }

    // identify 3
    for (auto it = len5.begin(); it != len5.end(); it++) {
      if (it->find(right1) != std::string::npos &&
          it->find(right2) != std::string::npos) {
        digit[*it] = 3;
        len5.erase(it);
        break;
      }
    }

    // identify 6
    for (auto it = len6.begin(); it != len6.end(); it++) {
      if (it->find(right1) == std::string::npos ||
          it->find(right2) == std::string::npos) {
        digit[*it] = 6;
        len6.erase(it);
        break;
      }
    }

    // missing segments in 0 and 9
    char miss1 = 'a';
    for (char c : {'a', 'b', 'c', 'd', 'e', 'f', 'g'}) {
      if (len6[0].find(c) == std::string::npos)
        miss1 = c;
    }

    // identify 0 and 9 using 2 and 5
    if (len5[0].find(miss1) == std::string::npos ||
        len5[1].find(miss1) == std::string::npos)
      digit[len6[0]] = 9, digit[len6[1]] = 0;
    else
      digit[len6[0]] = 0, digit[len6[1]] = 9;

    // identify 2 and 5 using 4
    std::string four1, four2;
    std::set_intersection(four.begin(), four.end(), len5[0].begin(),
                          len5[0].end(), std::back_inserter(four1));
    std::set_intersection(four.begin(), four.end(), len5[1].begin(),
                          len5[1].end(), std::back_inserter(four2));
    if (four1.length() == 2)
      digit[len5[0]] = 2, digit[len5[1]] = 5;
    else
      digit[len5[0]] = 5, digit[len5[1]] = 2;

    // calculate
    int place = 1000, num = 0;
    for (int i = 10; i < 14; i++) {
      int dig = digit[s[i]];
      num += place * dig;
      place /= 10;
    }
    total += num;
  }

  return total;
}

int main() {
  std::string in;
  std::vector<std::array<std::string, 14>> strs;

  while (std::getline(std::cin, in)) {
    std::stringstream ss(in);
    std::string t;
    std::array<std::string, 14> tmp;

    for (int i = 0; i < 10; i++) {
      ss >> t;
      sort(t.begin(), t.end());
      tmp[i] = t;
    }
    ss >> t;
    for (int i = 10; i < 14; i++) {
      ss >> t;
      sort(t.begin(), t.end());
      tmp[i] = t;
    }
    strs.push_back(tmp);
  }

  auto start = std::chrono::high_resolution_clock::now();
  int64_t part1_ans = part1(strs);
  auto end = std::chrono::high_resolution_clock::now();
  auto time1 =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();

  start = std::chrono::high_resolution_clock::now();
  int64_t part2_ans = part2(strs);
  end = std::chrono::high_resolution_clock::now();
  auto time2 =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();

  std::cout << "\nPart 1: " << part1_ans << "\nTime: " << time1 << "ms"
            << std::endl;
  std::cout << "\nPart 2: " << part2_ans << "\nTime: " << time2 << "ms"
            << std::endl;
}
