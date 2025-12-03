#include <utils.h>

long long solve(const std::string &input) {
	long long result = 0;

	auto lines = split(input, "\n");

	for (const auto& line : lines) {
		long long x = 0;
		int max_idx = -1;
		for (int j = 12-1; j >= 0; --j) {
			x *= 10L;
			long long max_c = 0;
			for (int i = max_idx + 1; i < line.size() - j; ++i) {
				if (line[i] - '0' > max_c) {
					max_c = line[i] - '0';
					max_idx = i;
				}
			}
			x += max_c;
		}
		result += x;
	}

	return result;
}

int main(int argc, char** argv) {
	auto runner = Runner<long long>(solve, 2025, 3);
	runner.add_test_file("t1.txt", 3121910778619L);

	runner.add_input_file("i1.txt");

	runner.run();
}