#include <utils.h>

bool check_invalid(long long i) {
	auto len = num_len(i);
	for (int part_lem = len / 2; part_lem >= 1; --part_lem) {
		if (len % part_lem != 0) continue;

		auto part_count = len / part_lem;
		auto d = static_cast<long long>(std::pow(10, part_lem));
		auto step = d;
		bool found = true;
		for (int k = 0; k < part_count-1; ++k) {
			// get bot parts
			auto r = (i % d) / (d / step);
			auto l = (i % (d*step)) / d;

			if (l != r) {
				found = false;
				break;
			}

			i -= r % step; // remove digits of right
			d *= step;
		}
		if (found) {
			return true;
		}
	}

	return false;
}

long long solve(const std::string &input) {
	long long result = 0;

	auto ranges = split(input, ",");

	for (const auto& range : ranges) {
		const auto [a, b] = split_once<long long, long long>(range, "-");

		for (long long i = a; i <= b; ++i) {
			if (check_invalid(i)) {
				result += i;
			}
		}

	}

	return result;
}

int main(int argc, char** argv) {
	auto runner = Runner<long long>(solve, 2025, 2);
	runner.add_test_file("t1.txt", 4174379265L);

	runner.add_input_file("i1.txt");

	runner.run();
}