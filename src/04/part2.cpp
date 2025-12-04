#include <utils.h>

int solve(const std::string &input) {
	int result = 0;

	if (input.empty()) return result;

	auto lines = split(input, "\n");

	const unsigned w = lines[0].size();
	const unsigned h = lines.size();
	while (true) {
		std::vector<Vec2i> removable{};
		for (int y = 0; y < lines.size(); ++y) {
			for (int x = 0; x < lines[y].size(); ++x) {
				if (lines[y][x] != '@') continue;

				int surrounding = 0;
				for (auto dir : all_dirs_diag()) {
					int nx = x + dir.x();
					int ny = y + dir.y();
					if (nx < 0 || nx >= w || ny < 0 || ny >= h) continue;
					if (lines[ny][nx] == '@') surrounding++;
				}
				if (surrounding < 4) {
					removable.emplace_back(x, y);
					result++;
				}
			}
		}
		if (removable.empty()) break;
		for (auto pos : removable) {
			lines[pos.y()][pos.x()] = '.';
		}
	}

	return result;
}

int main(int argc, char** argv) {
	auto runner = Runner<int>(solve, 2025, 4);
	runner.add_test_file("t1.txt", 43);

	runner.add_input_file("i1.txt");

	runner.run();
}