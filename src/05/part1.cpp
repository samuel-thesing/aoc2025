#include <utils.h>

long long solve(const std::string &input) {
	long long result = 0;

	const auto [s_ranges, s_ids] = split_once(input, "\n\n");
	const auto ids = split<long long>(s_ids, "\n");

	// Create ranges
	auto ranges = std::vector<Vec2l>{};
	for (const auto& s_range : split(s_ranges, "\n")) {
        const auto [a, b] =
        	extract_data<long long, long long>(s_range, std::regex("(\\d+)-(\\d+)"));
		ranges.emplace_back(a, b);
	}

	// check id against ranges
    for (const auto& id : ids) {
    	for (const auto& range : ranges) {
    		if (id >= range.x() && id <= range.y()) {
    			result++;
    			break;
    		}
    	}
    }

	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<long long>(solve, 2025, 5);
    runner.add_test_file("t1.txt", 3L);

    runner.add_input_file("i1.txt");

    runner.run();
}