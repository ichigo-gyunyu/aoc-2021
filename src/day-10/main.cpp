#include <algorithm>
#include <chrono>
#include <iostream>
#include <stack>
#include <vector>

int64_t part1(const std::vector<std::string> &lines) {
    int64_t ans = 0;

    for (auto line : lines) {
        std::stack<char> s;

        for (auto c : line) {
            if (c == '(' || c == '[' || c == '{' || c == '<')
                s.push(c);

            else if (c == ')') {
                if (s.empty() || s.top() != '(') {
                    ans += 3;
                    break;
                } else
                    s.pop();
            }

            else if (c == ']') {
                if (s.empty() || s.top() != '[') {
                    ans += 57;
                    break;
                } else
                    s.pop();
            }

            else if (c == '}') {
                if (s.empty() || s.top() != '{') {
                    ans += 1197;
                    break;
                } else
                    s.pop();
            }

            else if (c == '>') {
                if (s.empty() || s.top() != '<') {
                    ans += 25137;
                    break;
                } else
                    s.pop();
            }
        }
    }

    return ans;
}

int64_t part2(const std::vector<std::string> &lines) {
    std::vector<int64_t> scores;

    for (auto line : lines) {
        std::stack<char> s;
        bool corrupted = false;

        for (auto c : line) {
            if (c == '(' || c == '[' || c == '{' || c == '<')
                s.push(c);

            else if (c == ')') {
                if (s.empty() || s.top() != '(') {
                    corrupted = true;
                    break;
                } else
                    s.pop();
            }

            else if (c == ']') {
                if (s.empty() || s.top() != '[') {
                    corrupted = true;
                    break;
                } else
                    s.pop();
            }

            else if (c == '}') {
                if (s.empty() || s.top() != '{') {
                    corrupted = true;
                    break;
                } else
                    s.pop();
            }

            else if (c == '>') {
                if (s.empty() || s.top() != '<') {
                    corrupted = true;
                    break;
                } else
                    s.pop();
            }
        }

        if (corrupted)
            continue;

        int64_t score = 0;
        while (!s.empty()) {
            score *= 5;
            char c = s.top();
            s.pop();

            switch (c) {
            case '(':
                score += 1;
                break;
            case '[':
                score += 2;
                break;
            case '{':
                score += 3;
                break;
            case '<':
                score += 4;
                break;
            }
        }
        scores.push_back(score);
    }

    std::sort(scores.begin(), scores.end());
    size_t n = scores.size();
    return scores[n / 2];
}

int main() {
    std::string in;
    std::vector<std::string> lines;
    while (std::cin >> in) {
        lines.push_back(in);
    }

    auto start = std::chrono::high_resolution_clock::now();
    int64_t part1_ans = part1(lines);
    auto end = std::chrono::high_resolution_clock::now();
    auto time1 =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();

    start = std::chrono::high_resolution_clock::now();
    int64_t part2_ans = part2(lines);
    end = std::chrono::high_resolution_clock::now();
    auto time2 =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();

    std::cout << "\nPart 1: " << part1_ans << "\nTime: " << time1 << "ms"
              << std::endl;
    std::cout << "\nPart 2: " << part2_ans << "\nTime: " << time2 << "ms"
              << std::endl;
}
