#include <array>
#include <chrono>
#include <iostream>
#include <vector>

#define MAXN 2000
#define MAXX MAXN
#define MAXY MAXN

struct fold {
  int l;
  char dir;

  fold(int l, char dir) : l(l), dir(dir) {}
};

// can imporve by removing initial if
int64_t part1(const std::array<std::array<int, MAXX>, MAXY> grid,
              const fold f) {

  int count = 0;
  if (f.dir == 'x') {
    for (int i = 0; i < MAXY; i++) {
      for (int j = 0; j < MAXX; j++) {
        if (!grid[i][j])
          continue;
        int check_j = f.l - (j - f.l);

        if (j < f.l || (check_j >= 0 && !grid[i][check_j])) {
          count++;
        }
      }
    }
  }

  else {
    for (int i = 0; i < MAXY; i++) {
      for (int j = 0; j < MAXX; j++) {
        if (!grid[i][j])
          continue;
        int check_i = f.l - (i - f.l);

        if (i < f.l || (check_i >= 0 && !grid[check_i][j])) {
          count++;
        }
      }
    }
  }

  return count;
}

int64_t part2(std::array<std::array<int, MAXX>, MAXY> grid,
              const std::vector<fold> folds) {

  int cur_x = MAXX;
  int cur_y = MAXY;
  for (auto f : folds) {
    if (f.dir == 'x') {
      cur_x = f.l;

      for (int i = 0; i < cur_y; i++) {
        for (int j = 0; j < cur_x; j++) {
          if (grid[i][f.l + (f.l - j)])
            grid[i][j] = 1;
        }
      }
    }

    else {
      cur_y = f.l;

      for (int i = 0; i < cur_y; i++) {
        for (int j = 0; j < cur_x; j++) {
          if (grid[f.l + (f.l - i)][j])
            grid[i][j] = 1;
        }
      }
    }
  }

  // folded output
  std::cout << std::endl;
  for (int i = 0; i < cur_y; i++) {
    for (int j = 0; j < cur_x; j++) {
      if (j && j % 5 == 0)
        std::cout << "  ";
      std::cout << ((grid[i][j]) ? '#' : '.');
    }
    std::cout << std::endl;
  }

  return 0;
}

int main() {
  std::string in;
  std::array<std::array<int, MAXX>, MAXY> grid = {};
  std::vector<fold> folds;
  int maxx = 0, maxy = 0;
  while (std::cin >> in) {
    if (in[0] == 'x' || in[0] == 'y') {
      folds.emplace_back(stoi(in.substr(2)), in[0]);
    } else if (in[0] == 'f' || in[0] == 'a') {
      continue;
    } else {
      size_t pos = in.find(',');
      int x = stoi(in.substr(0, pos));
      int y = stoi(in.substr(pos + 1));
      maxx = std::max(maxx, x);
      maxy = std::max(maxy, y);
      grid[y][x] = 1;
    }
  }

  auto start = std::chrono::high_resolution_clock::now();
  int64_t part1_ans = part1(grid, folds[0]);
  auto end = std::chrono::high_resolution_clock::now();
  auto time1 =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();

  start = std::chrono::high_resolution_clock::now();
  int64_t part2_ans = part2(grid, folds);
  end = std::chrono::high_resolution_clock::now();
  auto time2 =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();

  std::cout << "\nPart 1: " << part1_ans << "\nTime: " << time1 << "ms"
            << std::endl;
  std::cout << "\nPart 2: " << part2_ans << "\nTime: " << time2 << "ms"
            << std::endl;
}
