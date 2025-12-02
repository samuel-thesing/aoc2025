#include <utils.h>

long long solve(const std::string &input) {
	long long result = 0;

	auto ranges = split(input, ",");

    for (const auto& range : ranges) {
    	const auto [a, b] = split_once<long long, long long>(range, "-");

		for (long long i = a; i <= b; ++i) {
			auto len = num_len(i);
			if (len % 2 != 0) continue; // can be split into 2 equal-length parts

			auto d = static_cast<long long>(std::pow(10, len / 2));
			auto left = i / d;
			auto right = i % d;

			if (left == right) {
				result += i;
			}
		}

    }

	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<long long>(solve, 2025, 2);
    runner.add_test_file("t1.txt", 1227775554L);

    runner.add_input_file("i1.txt");

    runner.run();
}