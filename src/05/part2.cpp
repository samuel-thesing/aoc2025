#include <utils.h>

long long solve(const std::string &input) {
    long long result = 0;

    const auto [s_ranges, _] = split_once(input, "\n\n");

    // Create ranges
    auto ranges = std::vector<Vec2l>{};
    for (const auto& s_range : split(s_ranges, "\n")) {
        const auto [a, b] =
            extract_data<long long, long long>(s_range, std::regex("(\\d+)-(\\d+)"));
        ranges.emplace_back(a, b);
    }

    std::sort(ranges.begin(), ranges.end(), [](const Vec2l& a, const Vec2l& b) {
        if (a.x() != b.x()) return a.x() < b.x();
        return a.y() < b.y();
    });

    // merge ranges
    long long start = ranges[0].x();
    long long end = ranges[0].y();
    for (int i = 1; i < ranges.size(); ++i) {
        if (ranges[i].x() <= end) {
            end = max(end, ranges[i].y());
        } else {
            result += end - start + 1;
            start = ranges[i].x();
            end = ranges[i].y();
        }
    }
    result += end - start + 1;

    return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<long long>(solve, 2025, 5);
    runner.add_test_file("t1.txt", 14L);

    runner.add_input_file("i1.txt");

    runner.run();
}