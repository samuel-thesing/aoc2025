#include <utils.h>

long long solve(const std::string &input) {
	auto lines = split(input, "\n");
	const unsigned w = lines.size();

    std::vector<long long> tachyons[] = {
	    std::vector(w, 0LL),
	    std::vector(w, 0LL)
    };
	int toggle = 0;

	for (int i = 0; i < w; i++) {
	    if (lines[0][i] == 'S') {
	        tachyons[1-toggle][i] = 1LL;
	    }
	}

    for (int y = 2; y < lines.size(); y+=2) {
        std::fill(tachyons[toggle].begin(), tachyons[toggle].end(), 0LL);
    	for (int x = 0; x < w; x++) {
    	    auto count = tachyons[1-toggle][x];
    	    if (count == 0) continue;

    	    if (lines[y][x] == '^') {
                tachyons[toggle][x-1] += count;
                tachyons[toggle][x+1] += count;
            } else {
                tachyons[toggle][x] += count;
            }
    	}

    	toggle = 1 - toggle;
    }

	return sum(tachyons[1-toggle]);
}

int main(int argc, char** argv) {
    auto runner = Runner<long long>(solve, 2025, 7);
    runner.add_test_file("t1.txt", 40);

    runner.add_input_file("i1.txt");

    runner.run();
}