#include <utils.h>

long long memoized_dfs(std::unordered_map<std::tuple<std::string, bool, bool>, long long>& memo,
		const std::unordered_map<std::string, std::vector<std::string>>& conns, const std::string& node,
		bool dac, bool fft) {
	auto it = memo.find(std::make_tuple(node, dac, fft));
	if (it != memo.end()) {
		return it->second;
	}

	if (node == "dac") dac = true;
	if (node == "fft") fft = true;

	long long count = 0;
	auto outs = conns.find(node)->second;
	for (const auto& out : outs) {
		if (out == "out") {
			if (fft && dac) count++;
		} else {
			count += memoized_dfs(memo, conns, out, dac, fft);
		}
	}
	memo.emplace(std::make_tuple(node, dac, fft), count);
	return count;
}

long long solve(const std::string &input) {
	// no cycle otherwise the answer could be infinity
	std::regex pattern("(\\S+): (.+)");

	auto lines = split(input, "\n");

	std::unordered_map<std::string, std::vector<std::string>> conns{};

	for (const auto& line : lines) {
		const auto [in, outs] = extract_data<std::string, std::string>(line, pattern);
		const auto out_vec = split(outs, " ");

		conns.emplace(in, out_vec);
	}

	std::unordered_map<std::tuple<std::string, bool, bool>, long long> memo{};
	long long result = memoized_dfs(memo, conns, "svr", false, false);

	return result;
}

int main(int argc, char** argv) {
	auto runner = Runner<long long>(solve, 2025, 11);
	runner.add_test_file("t2.txt", 2L);

	runner.add_input_file("i1.txt");

	runner.run();
}