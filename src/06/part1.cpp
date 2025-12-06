#include <utils.h>

long long solve(const std::string &input) {
	long long result = 0LL;

    std::regex pattern("\\s+");

	auto lines_s = split(input, "\n");
	auto operands_s = lines_s.back();
	lines_s.pop_back();

	auto operands = split_regex(operands_s, pattern);

	auto lines = std::vector<std::vector<long long>>{};
    for (const auto& line : lines_s) {
		lines.push_back(split_regex<long long>(line, pattern, string_to_generic<long long>));
    }

	for (int i = 0; i < lines[0].size(); i++) {
		std::string op = operands[i];
		long long part = 0LL;
		if (op == "+") {
			for (int j = 0; j < lines.size(); j++) {
				part += lines[j][i];
			}
		} else if (op == "*") {
			part = 1LL;
			for (int j = 0; j < lines.size(); j++) {
				part *= lines[j][i];
			}
		}
		result += part;
	}

	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<long long>(solve, 2025, 6);
    runner.add_test_file("t1.txt", 4277556LL);

    runner.add_input_file("i1.txt");

    runner.run();
}