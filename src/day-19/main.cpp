#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <ranges>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Point3D {
    int x;
    int y;
    int z;

    Point3D() : x{0}, y{0}, z{0} {}
    Point3D(int x, int y, int z) : x{x}, y{y}, z{z} {}

    bool    operator==(const Point3D   &p) const { return x == p.x && y == p.y && z == p.z; }
    Point3D operator-(const Point3D& p) const { return {x - p.x, y - p.y, z - p.z}; }
    Point3D operator+(const Point3D& p) const { return {x + p.x, y + p.y, z + p.z}; }

    void print() const { std::cout << "(" << x << "," << y << "," << z << ")"; }

    int manhattan_dist(const Point3D& p) const { return abs(x - p.x) + abs(y - p.y) + abs(z - p.z); }
};

template <> struct std::hash<Point3D> {
    std::size_t operator()(const Point3D& p) const noexcept {
        std::size_t h1 = std::hash<int>{}(p.x);
        std::size_t h2 = std::hash<int>{}(p.y);
        std::size_t h3 = std::hash<int>{}(p.z);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

namespace transforms {

// clang-format off
auto RotateZ0   = [](const Point3D& p) -> Point3D { return p; };
auto RotateZ90  = [](const Point3D& p) -> Point3D { return Point3D{-p.y, p.x, p.z}; };
auto RotateZ180 = [](const Point3D& p) -> Point3D { return Point3D{-p.x, -p.y, p.z}; };
auto RotateZ270 = [](const Point3D& p) -> Point3D { return Point3D{p.y, -p.x, p.z}; };
auto FaceFront  = [](const Point3D& p) -> Point3D { return p; };
auto FaceBack   = [](const Point3D& p) -> Point3D { return Point3D{-p.x, p.y, -p.z}; };
auto FaceLeft   = [](const Point3D& p) -> Point3D { return Point3D{-p.z, p.y, p.x}; };
auto FaceRight  = [](const Point3D& p) -> Point3D { return Point3D{p.z, p.y, -p.x}; };
auto FaceUp     = [](const Point3D& p) -> Point3D { return Point3D{p.x, -p.z, p.y}; };
auto FaceDown   = [](const Point3D& p) -> Point3D { return Point3D{p.x, p.z, -p.y}; };
// clang-format on

using Transformation = std::function<Point3D(const Point3D&)>;

std::vector<Transformation> rotations{RotateZ0, RotateZ90, RotateZ180, RotateZ270};
std::vector<Transformation> orientations{FaceFront, FaceBack, FaceLeft, FaceRight, FaceUp, FaceDown};

} // namespace transforms

struct Scanner {
    std::vector<Point3D> beacons;
    bool                 isNormalized;
    Point3D              normalized_offset;

    Scanner() : isNormalized{false} {}

    void normalize(const transforms::Transformation& rotation, const transforms::Transformation& orientation,
                   const Point3D& offset) {
        auto normalized_beacons = beacons | std::views::transform(rotation) | std::views::transform(orientation) |
                                  std::views::transform([&offset](const Point3D& p) { return p + offset; });
        std::ranges::copy(normalized_beacons, beacons.begin());
        isNormalized      = true;
        normalized_offset = offset;
    }
};

using Scanners = std::vector<Scanner>;

int64_t part1(Scanners& scanners) {

    using namespace transforms;

    auto normalizeIf12 = [](const Scanner& s1, Scanner& s2) -> bool {
        for (const auto& orientation : orientations) {
            for (const auto& rotation : rotations) {

                // apply one of the 24 transformations
                auto s2beacon_transform =
                    s2.beacons | std::views::transform(rotation) | std::views::transform(orientation);

                // check if we have >= 12 intersecting scanners
                std::unordered_map<Point3D, int> offset_count;
                Point3D                          best_offset;
                int                              max_count = 0;
                for (const auto& p1 : s1.beacons) {
                    for (const auto& p2 : s2beacon_transform) {
                        Point3D offset = p1 - p2;
                        offset_count[offset]++;
                        if (offset_count[offset] > max_count) {
                            best_offset = offset;
                            max_count   = offset_count[offset];
                        }
                    }
                }

                // normalize scanner s2 according to scanner 0
                if (max_count >= 12) {
                    s2.normalize(rotation, orientation, best_offset);
                    return true;
                }
            }
        }

        return false;
    };

    int num_scanners = scanners.size();
    scanners.front().normalize(RotateZ0, FaceFront, {0, 0, 0});
    int num_normalized = 1;

    // normalize all scanners wrt to scanner 0
    while (num_normalized < num_scanners) {
        for (const auto& norm_s : scanners) {
            if (!norm_s.isNormalized)
                continue;
            for (auto& s : scanners) {
                if (s.isNormalized)
                    continue;
                if (normalizeIf12(norm_s, s))
                    num_normalized++;
            }
        }
    }

    // get a list of all the unique beacons
    std::unordered_set<Point3D> all_beacons;
    for (const auto& scanner : scanners) {
        std::ranges::for_each(scanner.beacons, [&all_beacons](const Point3D& p) { all_beacons.insert(p); });
    }

    return all_beacons.size();
}

int64_t part2(Scanners& scanners) {
    int largest_manhattan = 0;
    for (const auto& s1 : scanners)
        for (const auto& s2 : scanners)
            largest_manhattan = std::max(largest_manhattan, s1.normalized_offset.manhattan_dist(s2.normalized_offset));

    return largest_manhattan;
}

int main() {

    std::string      in;
    std::string_view header = "---";

    Scanners scanners;

    while (std::getline(std::cin, in)) {
        if (in.starts_with(header)) {
            scanners.emplace_back();
            continue;
        }

        if (in == "") {
            continue;
        }

        std::stringstream ss;
        int               x, y, z;
        char              c;
        ss.str(in);
        ss >> x >> c >> y >> c >> z;
        scanners.back().beacons.emplace_back(x, y, z);
    }

    auto    start     = std::chrono::high_resolution_clock::now();
    int64_t part1_ans = part1(scanners);
    auto    end       = std::chrono::high_resolution_clock::now();
    auto    time1     = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    start             = std::chrono::high_resolution_clock::now();
    int64_t part2_ans = part2(scanners);
    end               = std::chrono::high_resolution_clock::now();
    auto time2        = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "\nPart 1: " << part1_ans << "\nTime: " << time1 << "ms" << std::endl;
    std::cout << "\nPart 2: " << part2_ans << "\nTime: " << time2 << "ms" << std::endl;
}
