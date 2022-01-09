#include <chrono>
#include <cmath>
#include <iostream>

// assuming the target area is below us
int64_t part1(int x1, int x2, int y1, int y2) {
    int maxvel = abs(y1) - 1;

    return (maxvel * maxvel + maxvel) / 2;
}

// simulate
int64_t part2(int x1, int x2, int y1, int y2) {
    int minx = sqrt(2 * x1) - 1;
    int maxx = x2;
    int miny = y1;
    int maxy = abs(y1) - 1;
    int count = 0;

    for (int i = minx; i <= maxx; i++) {
        for (int j = miny; j <= maxy; j++) {
            int x = 0, y = 0, vx = i, vy = j;
            while (y >= y1) {
                if (x >= x1 && x <= x2 && y <= y2 && y >= y1) {
                    count++;
                    break;
                }
                x += vx, y += vy, vx = (vx) ? (vx - 1) : 0, vy--;
            }
        }
    }

    return count;
}

int main() {
    std::string in;
    int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
    while (std::cin >> in) {
        size_t pos = in.find("..");
        if (pos != std::string::npos) {
            if (in[0] == 'x') {
                x1 = stoi(in.substr(2, pos));
                x2 = stoi(in.substr(pos + 2));
            } else if (in[0] == 'y') {
                y1 = stoi(in.substr(2, pos));
                y2 = stoi(in.substr(pos + 2));
            }
        }
    }

    auto start = std::chrono::high_resolution_clock::now();
    int64_t part1_ans = part1(x1, x2, y1, y2);
    auto end = std::chrono::high_resolution_clock::now();
    auto time1 =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();

    start = std::chrono::high_resolution_clock::now();
    int64_t part2_ans = part2(x1, x2, y1, y2);
    end = std::chrono::high_resolution_clock::now();
    auto time2 =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();

    std::cout << "\nPart 1: " << part1_ans << "\nTime: " << time1 << "ms"
              << std::endl;
    std::cout << "\nPart 2: " << part2_ans << "\nTime: " << time2 << "ms"
              << std::endl;
}
