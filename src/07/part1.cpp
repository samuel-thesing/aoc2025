#include <bitset>
#include <utils.h>

int solve(const std::string &input) {
	int result = 0;

	auto lines = split(input, "\n");
	const unsigned w = lines[0].size();

	std::bitset<142> tachyons[2] = {};
	int toggle = 0;
	for (int i = 0; i < lines[0].size(); i++) {
	    if (lines[0][i] == 'S') {
	        tachyons[1 - toggle].set(i);
	    }
	}

    for (int y = 2; y < lines.size(); y+=2) {
    	tachyons[toggle].reset();
    	for (int x = 0; x < w; x++) {
    		if (!tachyons[1-toggle][x]) continue;

    	    if (lines[y][x] == '^') {
    	        tachyons[toggle].set(x-1);
    	        tachyons[toggle].set(x+1);
    	        result += 1;
            } else {
                tachyons[toggle].set(x);
            }
    	}

    	toggle = 1 - toggle;
    }

	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<int>(solve, 2025, 7);
    runner.add_test_file("t1.txt", 21);

    runner.add_input_file("i1.txt");

    runner.run();
}