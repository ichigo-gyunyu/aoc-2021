#include <algorithm>
#include <chrono>
#include <iostream>
#include <ranges>
#include <vector>

using Image = std::vector<std::string>;

int64_t part1(const std::string& enhancement, const Image& image) {

    auto valid_index = [](int i, int j, int rows, int cols) { return i > 0 && j > 0 && i < rows && j < cols; };

    auto pad_image = [](const Image& image, int amount, char ch) {
        Image       padded_image;
        int         new_width = image[0].size() + 2 * amount;
        std::string pad(new_width, ch);
        std::string sidepad(amount, ch);
        std::ranges::fill_n(std::back_inserter(padded_image), amount, pad);
        std::ranges::transform(image, std::back_inserter(padded_image),
                               [&sidepad](const std::string& s) { return sidepad + s + sidepad; });
        std::ranges::fill_n(std::back_inserter(padded_image), amount, pad);

        return padded_image;
    };

    auto enhance = [&](const Image& from, Image& to, char outer) {
        int rows         = from.size();
        int cols         = from[0].size();
        int light_pixels = 0;
        // go over the image
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int enhancement_idx = 0;

                // go over the 9 cells
                for (int ii = -1; ii <= 1; ii++) {
                    for (int jj = -1; jj <= 1; jj++) {
                        enhancement_idx *= 2;
                        if (valid_index(i + ii, j + jj, rows, cols) && from[i + ii][j + jj] == '#')
                            enhancement_idx++;
                        if (!valid_index(i + ii, j + jj, rows, cols) && outer == '#')
                            enhancement_idx++;
                    }
                }

                // "enhance" the pixel
                to[i][j] = enhancement[enhancement_idx];
                if (to[i][j] == '#')
                    light_pixels++;
            }
        }

        return light_pixels;
    };

    Image padded_image = pad_image(image, 2, '.');
    Image once(padded_image.size(), std::string(padded_image[0].size(), '.'));
    enhance(padded_image, once, '.');
    Image padded_once = pad_image(once, 2, '#');
    Image twice(padded_once.size(), std::string(padded_once[0].size(), '.'));
    int   lights = enhance(padded_once, twice, '#');

    return lights;
}

int64_t part2(const std::string& enhancement, const Image& image) {

    auto valid_index = [](int i, int j, int rows, int cols) { return i > 0 && j > 0 && i < rows && j < cols; };

    auto pad_image = [](const Image& image, int amount, char ch) {
        Image       padded_image;
        int         new_width = image[0].size() + 2 * amount;
        std::string pad(new_width, ch);
        std::string sidepad(amount, ch);
        std::ranges::fill_n(std::back_inserter(padded_image), amount, pad);
        std::ranges::transform(image, std::back_inserter(padded_image),
                               [&sidepad](const std::string& s) { return sidepad + s + sidepad; });
        std::ranges::fill_n(std::back_inserter(padded_image), amount, pad);

        return padded_image;
    };

    auto enhance = [&](const Image& from, Image& to, char outer) {
        int rows         = from.size();
        int cols         = from[0].size();
        int light_pixels = 0;
        // go over the image
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int enhancement_idx = 0;

                // go over the 9 cells
                for (int ii = -1; ii <= 1; ii++) {
                    for (int jj = -1; jj <= 1; jj++) {
                        enhancement_idx *= 2;
                        if (valid_index(i + ii, j + jj, rows, cols) && from[i + ii][j + jj] == '#')
                            enhancement_idx++;
                        if (!valid_index(i + ii, j + jj, rows, cols) && outer == '#')
                            enhancement_idx++;
                    }
                }

                // "enhance" the pixel
                to[i][j] = enhancement[enhancement_idx];
                if (to[i][j] == '#')
                    light_pixels++;
            }
        }

        return light_pixels;
    };

    int enhance_times = 50;

    Image padded_image{image}, next_image;
    int   lights;

    while (enhance_times) {
        padded_image = pad_image(padded_image, 2, '.');
        next_image   = Image{padded_image.size(), std::string(padded_image[0].size(), '.')};
        enhance(padded_image, next_image, '.');
        padded_image = next_image;
        padded_image = pad_image(padded_image, 2, '#');
        next_image   = Image{padded_image.size(), std::string(padded_image[0].size(), '.')};
        lights       = enhance(padded_image, next_image, '#');
        padded_image = next_image;
        enhance_times -= 2;
    }

    return lights;
}

int main() {

    std::string in;
    std::string enhancement;
    Image       image;

    std::cin >> enhancement;
    while (std::cin >> in) {
        image.emplace_back(in);
    }

    auto    start     = std::chrono::high_resolution_clock::now();
    int64_t part1_ans = part1(enhancement, image);
    auto    end       = std::chrono::high_resolution_clock::now();
    auto    time1     = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    start             = std::chrono::high_resolution_clock::now();
    int64_t part2_ans = part2(enhancement, image);
    end               = std::chrono::high_resolution_clock::now();
    auto time2        = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "\nPart 1: " << part1_ans << "\nTime: " << time1 << "ms" << std::endl;
    std::cout << "\nPart 2: " << part2_ans << "\nTime: " << time2 << "ms" << std::endl;
}
