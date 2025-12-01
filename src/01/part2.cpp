#include <utils.h>

int solve(const std::string &input) {
	const auto WHEEL_SIZE = 100;

	auto lines = split(input, "\n");

	int result = 0;
	int previous = 50;
	for (const auto& line : lines) {
		const auto [c, d] = extract_data<char, int>(line, std::regex("(.?)(\\d+)"));

		result += std::abs(d / WHEEL_SIZE); // full wraps

		auto dist = (c == 'L' ? -1 : 1) * (d % WHEEL_SIZE);
		int next = previous + dist;

		// wrapped or on 0 and wasn't on 0 before
		if ((next <= 0 && previous != 0) || next > WHEEL_SIZE - 1) {
			result++;
		}

		previous = (next + WHEEL_SIZE) % WHEEL_SIZE;
	}

	return result;
}

int main(int argc, char** argv) {
	auto runner = Runner<int>(solve, 2025, 1);
	runner.add_test_file("t1.txt", 6);
	runner.add_test_string("R1000", 10);
	runner.add_test_file("t2.txt", 1); // false double count
	runner.add_test_file("t3.txt", 1); // stay on 0

	runner.add_input_file("i1.txt");

	runner.run();
}