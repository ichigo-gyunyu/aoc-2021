#include <chrono>
#include <iostream>
#include <vector>

int64_t part1(int start1, int start2) {
    int rolls  = 0;
    int score1 = 0, score2 = 0;
    int die = 1;
    start1--, start2--; // 0 based indexing

    auto roll = [&rolls](int& pos, int& score, int& die) {
        pos = (pos + die) % 10;
        die = (die == 100) ? 1 : die + 1;
        pos = (pos + die) % 10;
        die = (die == 100) ? 1 : die + 1;
        pos = (pos + die) % 10;
        die = (die == 100) ? 1 : die + 1;
        score += pos + 1;
        rolls += 3;
    };

    int losingscore;

    for (;;) {
        roll(start1, score1, die);
        if (score1 >= 1000) {
            losingscore = score2;
            break;
        }
        roll(start2, score2, die);
        if (score2 >= 1000) {
            losingscore = score1;
            break;
        }
    }

    return losingscore * rolls;
}

int64_t part2(int start1, int start2) {
    // stores {die roll value, num ways}
    std::vector<std::pair<int, int>> distribution = {
        {3, 1},
        {4, 3},
        {5, 6},
        {6, 7},
        {7, 6},
        {8, 3},
        {9, 1}
    };

    // state: [player1 pos][player2 pos][player1 score][player2 score]
    struct DP4 {
        // flatttened 4D vector
        std::vector<uint64_t> dp;
        // dimensions
        int i;
        int j;
        int k;
        int l;

        DP4(int i, int j, int k, int l) : dp(i * j * k * l, 0), i{i}, j{j}, k{k}, l{l} {}
        DP4()           = default;
        DP4(DP4 const&) = default;

        uint64_t get(int a, int b, int c, int d) { return dp[a * (j * k * l) + b * (k * l) + c * (l) + d]; }
        void set(int a, int b, int c, int d, uint64_t val) { dp[a * (j * k * l) + b * (k * l) + c * (l) + d] = val; }
    };

    const int maxscore = 31;
    const int maxpos   = 10;

    auto init_dp4 = []() { return DP4{maxpos + 1, maxpos + 1, maxscore + 1, maxscore + 1}; };

    DP4 dp = init_dp4();
    dp.set(start1, start2, 0, 0, 1);

    uint64_t player1wins = 0, player2wins = 0;
    int      turns = 21; // game can last at most 21 turns
    while (turns--) {

        //  player 1's possible moves
        DP4 dpnow = init_dp4();
        for (int pos2 = 1; pos2 <= maxpos; pos2++) {
            for (int score2 = 0; score2 <= maxscore; score2++) {
                for (int pos1 = 1; pos1 <= maxpos; pos1++) {
                    for (int score1 = 0; score1 <= maxscore; score1++) {

                        if (score1 - pos1 < 0)
                            continue;

                        // update dp
                        for (auto [roll, times] : distribution) {
                            int      backpos = (pos1 - roll <= 0) ? pos1 - roll + maxpos : pos1 - roll;
                            uint64_t current = dpnow.get(pos1, pos2, score1, score2);
                            uint64_t update  = dp.get(backpos, pos2, score1 - pos1, score2) * times;
                            dpnow.set(pos1, pos2, score1, score2, current + update);
                        }
                    }
                }
            }
        }

        // update player 1's wins
        for (int pos1 = 1; pos1 <= maxpos; pos1++) {
            for (int score1 = 21; score1 <= maxscore; score1++) {
                for (int pos2 = 1; pos2 <= maxpos; pos2++) {
                    for (int score2 = 0; score2 <= maxscore; score2++) {
                        player1wins += dpnow.get(pos1, pos2, score1, score2);
                        dpnow.set(pos1, pos2, score1, score2, 0);
                    }
                }
            }
        }

        // save the updated dp
        dp = dpnow;

        // player 2's possible moves
        dpnow = init_dp4();
        for (int pos1 = 1; pos1 <= maxpos; pos1++) {
            for (int score1 = 0; score1 <= maxscore; score1++) {
                for (int pos2 = 1; pos2 <= maxpos; pos2++) {
                    for (int score2 = 0; score2 <= maxscore; score2++) {

                        if (score2 - pos2 < 0)
                            continue;

                        // update dp
                        for (auto [roll, times] : distribution) {
                            int      backpos = (pos2 - roll <= 0) ? pos2 - roll + maxpos : pos2 - roll;
                            uint64_t current = dpnow.get(pos1, pos2, score1, score2);
                            uint64_t update  = dp.get(pos1, backpos, score1, score2 - pos2) * times;
                            dpnow.set(pos1, pos2, score1, score2, current + update);
                        }
                    }
                }
            }
        }

        // update player 2's wins
        for (int pos2 = 1; pos2 <= maxpos; pos2++) {
            for (int score2 = 21; score2 <= maxscore; score2++) {
                for (int pos1 = 1; pos1 <= maxpos; pos1++) {
                    for (int score1 = 0; score1 <= maxscore; score1++) {
                        player2wins += dpnow.get(pos1, pos2, score1, score2);
                        dpnow.set(pos1, pos2, score1, score2, 0);
                    }
                }
            }
        }

        // save the updated dp
        dp = dpnow;
    }

    return std::max(player1wins, player2wins);
}

int main() {

    auto parse_line = [](std::string line) {
        auto pos = line.find(':');
        auto val = line.substr(pos + 2);
        return std::stoi(val);
    };

    std::string in;
    std::getline(std::cin, in);
    int start1 = parse_line(in);
    std::getline(std::cin, in);
    int start2 = parse_line(in);

    auto    start     = std::chrono::high_resolution_clock::now();
    int64_t part1_ans = part1(start1, start2);
    auto    end       = std::chrono::high_resolution_clock::now();
    auto    time1     = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    start             = std::chrono::high_resolution_clock::now();
    int64_t part2_ans = part2(start1, start2);
    end               = std::chrono::high_resolution_clock::now();
    auto time2        = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "\nPart 1: " << part1_ans << "\nTime: " << time1 << "ms" << std::endl;
    std::cout << "\nPart 2: " << part2_ans << "\nTime: " << time2 << "ms" << std::endl;
}
