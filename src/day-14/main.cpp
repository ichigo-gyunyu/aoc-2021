#include <array>
#include <chrono>
#include <iostream>
#include <map>
#include <vector>

#define STEPS1 10

using std::cin;
using std::cout;
using std::string;

int64_t part1(string polymer_template,
              std::map<std::pair<char, char>, char> pair_insertions) {

    std::vector<char> polymer(polymer_template.begin(), polymer_template.end());

    for (int s = 0; s < STEPS1; s++) {
        size_t n = 2 * polymer.size() - 1;
        std::vector<char> t(n);

        for (size_t i = 0; i < n; i++) {
            t[i] = (i % 2)
                       ? pair_insertions[{polymer[i / 2], polymer[i / 2 + 1]}]
                       : polymer[i / 2];
        }
        polymer = t;
    }

    std::array<int, 26> fq = {0};
    for (auto c : polymer)
        fq[c - 'A']++;
    int maxfq = 0, minfq = std::numeric_limits<int>::max();
    for (auto e : fq) {
        if (e && e < minfq)
            minfq = e;
        if (e > maxfq)
            maxfq = e;
    }

    return maxfq - minfq;
}

int64_t part2() { return 0; }

int main() {
    string polymer_template, in1, in2;
    cin >> polymer_template;
    std::map<std::pair<char, char>, char> pair_insertions;

    while (cin >> in1 >> in2 >> in2) {
        pair_insertions[{in1[0], in1[1]}] = in2[0];
    }

    auto start = std::chrono::high_resolution_clock::now();
    int64_t part1_ans = part1(polymer_template, pair_insertions);
    auto end = std::chrono::high_resolution_clock::now();
    auto time1 =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();

    start = std::chrono::high_resolution_clock::now();
    int64_t part2_ans = part2();
    end = std::chrono::high_resolution_clock::now();
    auto time2 =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();

    cout << "\nPart 1: " << part1_ans << "\nTime: " << time1 << "ms"
         << std::endl;
    cout << "\nPart 2: " << part2_ans << "\nTime: " << time2 << "ms"
         << std::endl;
}
