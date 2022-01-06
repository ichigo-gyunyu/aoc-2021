#include <array>
#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

#define STEPS1 10
#define STEPS2 40

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

int64_t part2(string polymer_template,
              std::map<std::pair<char, char>, char> pair_insertions) {

    std::array<int64_t, 26> fq = {0};

    struct memo_obj {
        char l;
        char r;
        int depth;

        memo_obj(char l, char r, int depth) : l(l), r(r), depth(depth) {}

        bool operator==(const memo_obj &o) const {
            return l == o.l && r == o.r && depth == o.depth;
        }
    };
    struct hashfn {
        size_t operator()(const memo_obj &o) const {
            auto h1 = std::hash<char>()(o.l);
            auto h2 = std::hash<char>()(o.r);
            auto h3 = std::hash<int>()(o.depth);

            return h1 ^ h2 ^ h3;
        }
    };
    std::unordered_map<memo_obj, std::array<int64_t, 26>, hashfn> memo;

    std::function<std::array<int64_t, 26>(char, char, int)> f =
        [&](char l, char r, int depth) {
            if (depth == 0) {
                std::array<int64_t, 26> t = {0};
                t[l - 'A']++, t[r - 'A']++;
                return t;
            }

            memo_obj mo(l, r, depth);
            if (memo.contains(mo))
                return memo[mo];

            char baby = pair_insertions[{l, r}];
            auto fq = f(l, baby, depth - 1);
            auto fqr = f(baby, r, depth - 1);
            for (size_t i = 0; auto e : fqr) {
                fq[i] += e;
                ++i;
            }
            fq[baby - 'A']--; // remove extra

            return memo[mo] = fq;
        };

    size_t n = polymer_template.size();
    for (size_t i = 0; i < n - 1; i++) {
        auto t = f(polymer_template[i], polymer_template[i + 1], STEPS2);
        for (size_t j = 0; auto e : t) {
            fq[j] += e;
            ++j;
        }

        if (i)
            fq[polymer_template[i] - 'A']--;
    }

    int64_t maxfq = 0, minfq = std::numeric_limits<int64_t>::max();
    for (auto e : fq) {
        if (e && e < minfq)
            minfq = e;
        if (e > maxfq)
            maxfq = e;
    }

    return maxfq - minfq;
}

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
    int64_t part2_ans = part2(polymer_template, pair_insertions);
    end = std::chrono::high_resolution_clock::now();
    auto time2 =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();

    cout << "\nPart 1: " << part1_ans << "\nTime: " << time1 << "ms"
         << std::endl;
    cout << "\nPart 2: " << part2_ans << "\nTime: " << time2 << "ms"
         << std::endl;
}
