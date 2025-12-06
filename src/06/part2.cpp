#include <utils.h>

long long solve(const std::string &input) {
	long long result = 0LL;

	std::regex pattern("\\s+");

	auto lines = split(input, "\n", false);
	auto operands = lines.back();
	lines.pop_back();

	// i don't want to handle special cases so pad all lines to the same length
	unsigned w = 0;
	for (int i = 0; i < lines.size(); i++) {
		w = max(w, static_cast<unsigned>(lines[i].size()));
	}
	for (int i = 0; i < lines.size(); i++) {
		lines[i] = pad_right(lines[i], w);
	}
	operands = pad_right(operands, w);

	// go column by column. Reset when empty column
	long long part = 0LL;
	char op = ' ';
	for (int i = 0; i < w; i++) {
		if (operands[i] != ' ') {
			result += part;
			op = operands[i];
			part = op == '+' ? 0LL : 1LL;
		}
		// read number from column
		long long num = 0LL;
		bool empty = true;
		for (int j = 0; j < lines.size(); ++j) {
			if (lines[j][i] != ' ') {
				num *= 10LL;
				num += lines[j][i] - '0';
				empty = false;
			}
		}
		// continue on empty col because it could multiply by 0
		if (empty) continue;

		if (op == '+') {
			part += num;
		} else if (op == '*') {
			part *= num;
		}
	}
	result += part;

	return result;
}

int main(int argc, char** argv) {
	auto runner = Runner<long long>(solve, 2025, 6);
	runner.add_test_file("t1.txt", 3263827LL);

	runner.add_input_file("i1.txt");

	runner.run();
}