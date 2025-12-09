#include <utils.h>

/**
 * That was a bit more complex than i thought (and probably by far not the best way to solve this)
 */
long long solve(const std::string &input) {
	long long result = 0;

	auto lines = split(input, "\n");

	auto red_tiles = std::vector<Vec2l>{};
	for (const auto& line : lines) {
		const auto [x, y] = extract_data<int, int>(line, std::regex("(\\d+),(\\d+)"));
		red_tiles.emplace_back(x, y);
	}

	for (int i = 0; i < red_tiles.size(); i++) {
		for (int j = i+1; j < red_tiles.size(); j++) {
			auto a = red_tiles[i];
			auto b = red_tiles[j];

			auto tl = a.min(b);
			auto br = a.max(b);

			// ray check if inside (going right)
			if (br.x() - tl.x() > 1 && br.y() - tl.y() > 1) {
				auto p = tl + Vec2l(1, 1);
				bool inside = true;
				for (int k = 0; k < red_tiles.size(); k++) {
					auto c = red_tiles[k];
					auto d = red_tiles[(k+1) % red_tiles.size()];
					if (c.y() == d.y()) continue; // ignore horizontal edges
					if (c.y() > d.y()) { // sort to reduce cases
						auto tmp = c;
						c = d;
						d = tmp;
					}
					// hitting a TL corner flips the state; hitting a BR corner does not
					if (c.x() > p.x() && c.y() <= p.y() && p.y() < d.y()) inside = !inside;
				}
				if (inside) continue;
			}

			// check if any edge intersects with the rect. (Example: edge 9,5-9,7 intersects with rect 2,5-11,7)
			bool edge_inside = false;
			for (int k = 0; k < red_tiles.size(); k++) {
				if (k == i || k == j) continue;
				auto c = red_tiles[k];
				auto d = red_tiles[(k+1) % red_tiles.size()];
				auto s = c.min(d);
				auto e = c.max(d);

				// edge is horizontal and between upper and lower edge of rect
				if (e.y() == s.y() && e.y() > tl.y() && e.y() < br.y()) {
					// range collision with rect
					if (s.x() < br.x() && tl.x() < e.x()) {
						edge_inside = true;
						break;
					}
				// edge is vertical and between left and right edge of rect
				} else if (e.x() == s.x() && e.x() > tl.x() && e.x() < br.x()) {
					// range collision with rect
					if (s.y() < br.y() && tl.y() < e.y()) {
						edge_inside = true;
						break;
					}
				}
			}
			if (edge_inside) continue;
			auto size = (abs(a.x() - b.x()) + 1) * (abs(a.y() - b.y()) + 1);
			result = max(result, size);
		}
	}

	return result;
}

int main(int argc, char** argv) {
	auto runner = Runner<long long>(solve, 2025, 9);
	runner.add_test_file("t1.txt", 24);

	runner.add_input_file("i1.txt");

	runner.run();
}