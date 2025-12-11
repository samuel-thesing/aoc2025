#include <utils.h>

#include <bitset>

struct BFSNode {
    std::bitset<10> lights;
    int step = 0;
};

int solve_bfs(const std::vector<std::bitset<10>>& buttons, const std::bitset<10>& target) {
    std::queue<BFSNode> q{};
    std::unordered_set<std::bitset<10>> visited{};
    q.emplace(std::bitset<10>{});
    while (!q.empty()) {
        auto c = q.front(); q.pop();
        auto curr = c.lights;
        auto step = c.step;
        visited.insert(curr);
        if (curr == target) return step;
        for (const auto& button : buttons) {
            auto next = curr ^ button;
            if (visited.contains(next)) continue;
            if (next == target) {
                return step+1;
            }
            q.emplace(next, step+1);
        }
    }
    Logger::error("No solution found");
    return -1;
}

int solve(const std::string &input) {
	int result = 0;

    std::regex pattern("\\[(.+)\\] (.*?) \\{(.+)\\}");
    std::regex pattern2("\\(([\\d,]+)\\)");
    auto lines = split(input, "\n");

    for (const auto& line : lines) {
    	auto [light_diagram, button_wirings, _] = extract_data<std::string, std::string, std::string>(line, pattern);
        std::bitset<10> lights{};
        for (int i = 0; i < light_diagram.size(); i++) {
            if (light_diagram[i] == '#') lights.set(i);
        }

        std::vector<std::bitset<10>> buttons{};
        for (const auto& button : extract_data_all<std::string>(button_wirings, pattern2)) {
            auto but = std::get<0>(button);
            std::bitset<10> b{};
            auto b_idxs = split_int(but, ",");
            for (int idx : b_idxs) {
                b.set(idx);
            }
            buttons.push_back(b);
        }

        auto count = solve_bfs(buttons, lights);
        result += count;
    }

	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<int>(solve, 2025, 10);
    runner.add_test_file("t1.txt", 7);

    runner.add_input_file("i1.txt");

    runner.run();
}