#include <utils.h>

long long solve(const std::string &input) {
	long long result = 0;

	auto lines = split(input, "\n");

	auto red_tiles = std::vector<Vec2l>{};
    for (const auto& line : lines) {
    	const auto [x, y] = extract_data<int, int>(line, std::regex("(\\d+),(\\d+)"));
		red_tiles.emplace_back(x, y);
    }

	for (int i = 0; i < red_tiles.size(); i++) {
		for (int j = i+1; j < red_tiles.size(); j++) {
			auto a = red_tiles[i];
			auto b = red_tiles[j];
			auto size = (abs(a.x() - b.x()) + 1) * (abs(a.y() - b.y()) + 1);
			result = max(result, size);
		}
	}

	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<long long>(solve, 2025, 9);
    runner.add_test_file("t1.txt", 50);

    runner.add_input_file("i1.txt");

    runner.run();
}