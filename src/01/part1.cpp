#include <utils.h>

int solve(const std::string &input) {
	auto lines = split(input, "\n");

	int result = 0;
	int current = 50;
    for (const auto& line : lines) {
    	const auto [c, d] = extract_data<char, int>(line, std::regex("(.?)(\\d+)"));

    	auto dist = c == 'L' ? -d : d;
    	current = (current + dist + 100) % 100;

    	if (current == 0) result++;
    }

	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<int>(solve, 2025, 1);
    runner.add_test_file("t1.txt", 3);

    runner.add_input_file("i1.txt");

    runner.run();
}