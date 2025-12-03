#include <utils.h>

int solve(const std::string &input) {
	int result = 0;

	auto lines = split(input, "\n");

    for (const auto& line : lines) {
		int x = 0;

    	int max_c = 0;
    	int max_idx = 0;
    	for (int i = 0; i < line.size() - 1; ++i) {
    		if (line[i] - '0' > max_c) {
    			max_c = line[i] - '0';
    			max_idx = i;
    		}
    	}
		x += max_c * 10;

    	max_c = 0;
    	for (int i = max_idx + 1; i < line.size(); ++i) {
			max_c = max(max_c, line[i] - '0');
    	}
    	x += max_c;

		result += x;
    }

	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<int>(solve, 2025, 3);
    runner.add_test_file("t1.txt", 357);

    runner.add_input_file("i1.txt");

    runner.run();
}