#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <numeric>
#include <unordered_map>

int64_t part1(std::string s,
              std::unordered_map<std::string, int> &bin3_to_int) {
    int64_t vno_sum = 0;

    auto binstr_to_int = [](std::string s) {
        int64_t res = 0;
        size_t n = s.length();
        for (char c : s) {
            --n;
            res += (int64_t)((c - '0') * pow(2, n));
        }
        return res;
    };

    // returns the number of bits parsed
    std::function<int64_t(std::string)> parse_packet = [&](std::string s) {
        int64_t vno = bin3_to_int[s.substr(0, 3)];
        int64_t type_id = bin3_to_int[s.substr(3, 3)];
        vno_sum += vno;
        int64_t parsed = 6;

        if (type_id != 4) {
            int ltid = s[parsed++] - '0';
            if (ltid) {
                int len = binstr_to_int(s.substr(parsed, 11));
                parsed += 11;
                while (len--) {
                    parsed += parse_packet(s.substr(parsed));
                }
            } else {
                int len = binstr_to_int(s.substr(parsed, 15));
                parsed += 15;
                while (len > 0) {
                    int x = parse_packet(s.substr(parsed));
                    parsed += x, len -= x;
                }
            }
        }

        else {
            while (s[parsed] - '0') {
                parsed += 5;
            }
            parsed += 5;
        }

        return parsed;
    };

    parse_packet(s);

    return vno_sum;
}

int64_t part2(std::string s,
              std::unordered_map<std::string, int> &bin3_to_int) {
    int64_t vno_sum = 0;

    auto binstr_to_int = [](std::string s) {
        int64_t res = 0;
        size_t n = s.length();
        for (char c : s) {
            --n;
            res += (int64_t)((c - '0') * pow(2, n));
        }
        return res;
    };

    struct return_info {
        int64_t parsed;
        int64_t res;
    };

    // returns the number of bits parsed
    std::function<return_info(std::string)> parse_packet = [&](std::string s) {
        int64_t vno = bin3_to_int[s.substr(0, 3)];
        int64_t type_id = bin3_to_int[s.substr(3, 3)];
        vno_sum += vno;
        return_info info;
        info.parsed = 6;

        if (type_id != 4) {
            int ltid = s[info.parsed++] - '0';
            std::vector<int64_t> vals;

            if (ltid) {
                int len = binstr_to_int(s.substr(info.parsed, 11));
                info.parsed += 11;
                while (len--) {
                    auto o = parse_packet(s.substr(info.parsed));
                    info.parsed += o.parsed;
                    vals.push_back(o.res);
                }
            } else {
                int len = binstr_to_int(s.substr(info.parsed, 15));
                info.parsed += 15;
                while (len > 0) {
                    auto o = parse_packet(s.substr(info.parsed));
                    info.parsed += o.parsed, len -= o.parsed;
                    vals.push_back(o.res);
                }
            }

            assert(!vals.empty());

            switch (type_id) {
            case 0: {
                info.res = std::accumulate(vals.begin(), vals.end(), 0ll);
                break;
            }
            case 1: {
                int64_t product = 1;
                for (auto e : vals)
                    product *= e;
                info.res = product;
                break;
            }
            case 2: {
                info.res = *std::min_element(vals.begin(), vals.end());
                break;
            }
            case 3: {
                info.res = *std::max_element(vals.begin(), vals.end());
                break;
            }
            case 5: {
                info.res = vals[0] > vals[1];
                break;
            }
            case 6: {
                info.res = vals[0] < vals[1];
                break;
            }
            case 7: {
                info.res = vals[0] == vals[1];
                break;
            }
            }
        }

        else {
            std::string valstring = "";
            while (s[info.parsed] - '0') {
                valstring += s.substr(info.parsed + 1, 4);
                info.parsed += 5;
            }
            valstring += s.substr(info.parsed + 1, 4);
            info.parsed += 5;
            info.res = binstr_to_int(valstring);
        }

        return info;
    };

    auto o = parse_packet(s);

    return o.res;
}

int main() {
    std::string in, binstr = "";
    std::cin >> in;

    std::unordered_map<char, std::string> hex_to_bin = {
        {'0', "0000"}, {'1', "0001"}, {'2', "0010"}, {'3', "0011"},
        {'4', "0100"}, {'5', "0101"}, {'6', "0110"}, {'7', "0111"},
        {'8', "1000"}, {'9', "1001"}, {'A', "1010"}, {'B', "1011"},
        {'C', "1100"}, {'D', "1101"}, {'E', "1110"}, {'F', "1111"},
    };

    std::unordered_map<std::string, int> bin3_to_int = {
        {"000", 0}, {"001", 1}, {"010", 2}, {"011", 3},
        {"100", 4}, {"101", 5}, {"110", 6}, {"111", 7},
    };

    for (char c : in)
        binstr += hex_to_bin[c];

    auto start = std::chrono::high_resolution_clock::now();
    int64_t part1_ans = part1(binstr, bin3_to_int);
    auto end = std::chrono::high_resolution_clock::now();
    auto time1 =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();

    start = std::chrono::high_resolution_clock::now();
    int64_t part2_ans = part2(binstr, bin3_to_int);
    end = std::chrono::high_resolution_clock::now();
    auto time2 =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();

    std::cout << "\nPart 1: " << part1_ans << "\nTime: " << time1 << "ms"
              << std::endl;
    std::cout << "\nPart 2: " << part2_ans << "\nTime: " << time2 << "ms"
              << std::endl;
}
