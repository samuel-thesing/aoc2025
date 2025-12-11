#include <utils.h>

#include <bitset>
#include <z3++.h>

typedef Matrix<10, 1, int> Vec10i;

struct BFSNode {
    Vec10i joltage;
    int btn_idx = 0;
    int step = 0;
};

int solve(const std::string &input) {
	int result = 0;

    std::regex pattern("\\[(.+)\\] (.*?) \\{(.+)\\}");
    std::regex pattern2("\\(([\\d,]+)\\)");
    auto lines = split(input, "\n");

    for (const auto& line : lines) {
    	z3::context c;

        auto [light_diagram, button_wirings, joltage_reqs] = extract_data<std::string, std::string, std::string>(line, pattern);
        std::vector<int> target = split_int(joltage_reqs, ",");

        z3::expr_vector btn_exprs(c);
        std::vector<z3::expr_vector> rows{};
        for (int i = 0; i < light_diagram.size(); i++) {
            rows.push_back(z3::expr_vector(c));
        }
        auto btn_s = extract_data_all<std::string>(button_wirings, pattern2);
        for (int i = 0; i < btn_s.size(); i++) {
            std::string name = "x" + std::to_string(i);
            auto e = c.int_const(name.c_str());

            auto but = std::get<0>(btn_s[i]);
            auto b_idxs = split_int(but, ",");
            for (auto idx : b_idxs) {
                rows[idx].push_back(e);
            }
            btn_exprs.push_back(e);
        }

        z3::optimize opt(c);

        for (const auto& e : btn_exprs) {
            opt.add(e >= 0);
        }


        for (int i = 0; i < light_diagram.size(); i++) {
            if (rows[i].empty()) {
                if (target[i] != 0) {
                    Logger::critical("Found non-zero target that is not influenced by any button.");
                }
                continue;
            }
            z3::expr left = z3::sum(rows[i]);
            opt.add(left == target[i]);
        }

        opt.minimize(sum(btn_exprs));

        if (opt.check() != z3::sat) {
            Logger::error("No solution found.");
        } else {
            z3::model m = opt.get_model();
            for (const auto& e : btn_exprs) {
                auto r = m.eval(e).as_int64();
                //std::cout << e.decl().name() << " = " << r << std::endl;
                result += r;
            }
        }
        //std::cout << opt << std::endl;
    }

	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<int>(solve, 2025, 10);
    runner.add_test_file("t1.txt", 33);

    runner.add_input_file("i1.txt");

    runner.run();
}