#include <utils.h>

struct Junction {
	Vec3l pos;
	int group = 0;
};

struct Distance {
	double dist;
	int j1, j2;
};

long long solve(const std::string &input) {
	auto lines = split(input, "\n");
    std::vector<Junction> junctions;

    for (const auto& line : lines) {
    	const auto [x, y, z] = extract_data<int, int, int>(line, std::regex("(\\d+),(\\d+),(\\d+)"));
    	junctions.emplace_back(Vec3l(x, y, z), -1);
    }

	std::vector<Distance> distances{};
	for (int i = 0; i < junctions.size(); i++) {
		for (int j = i+1; j < junctions.size(); j++) {
			// i used Vec2i here and had an overflow which i noticed way too late :(
			distances.emplace_back((junctions[i].pos - junctions[j].pos).norm(), i, j);
		}
	}
	std::sort(distances.begin(), distances.end(), [](const auto& a, const auto& b) { return a.dist < b.dist; });

	std::vector<std::vector<int>> groups{};
	for (auto& d : distances) {
		auto& j1 = junctions[d.j1];
		auto& j2 = junctions[d.j2];
		if (j1.group != -1 && j2.group != -1) {
			if (j1.group == j2.group) continue;

			// merge 2 existing groups
			auto remove_group = j2.group;
			for (auto k : groups[remove_group]) {
				junctions[k].group = j1.group;
			}
			groups[j1.group].insert(groups[j1.group].end(), groups[remove_group].begin(), groups[remove_group].end());
			groups[remove_group].clear();
		} else if (j1.group == -1 && j2.group != -1) {
			// add node to existing group
			j1.group = j2.group;
			groups[j2.group].push_back(d.j1);
		} else if (j1.group != -1 && j2.group == -1) {
			// add node to existing group
			j2.group = j1.group;
			groups[j1.group].push_back(d.j2);
		} else if (j1.group == -1 && j2.group == -1) {
			// create new group
			auto new_group = groups.size();
			j1.group = new_group;
			j2.group = new_group;
			groups.push_back({d.j1, d.j2});
		}
		// updated group contains all junctions?
		if (groups[j1.group].size() == junctions.size()) {
			return j1.pos.x() * j2.pos.x();
		}
	}

	Logger::critical("Something went wrong");
}

int main(int argc, char** argv) {
    auto runner = Runner<long long>(solve, 2025, 8);
    runner.add_test_file("t1.txt", 25272);

    runner.add_input_file("i1.txt");

    runner.run();
}