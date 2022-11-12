#include "DP.h"

#include <chrono>
#include <fstream>
#include <format>
#include <iostream>

inline void test_mco() {
	std::cout << "ex1: \n";
	using milliseconds = std::chrono::duration<double, std::milli>;
	std::ifstream fin("ex1/input/2_1_input.txt");
	for (int cas = 1; cas <= 5; ++cas) {
		std::cout << std::format("\nCase {0}:\n", cas);
		size_t n; 
		fin >> n;
		std::vector<int> p(n + 1);
		for (int i = 0; i <= n; ++i)
			fin >> p[i];
		std::vector<std::vector<int64_t>> m;
		std::vector<std::vector<int>> s;

		auto solve_start = std::chrono::steady_clock::now();
		int64_t cnt = matrix_chain_order::solve(p, m, s);
		auto solve_finish = std::chrono::steady_clock::now();
		milliseconds solve_dur = solve_finish - solve_start;
		double solve_duration = solve_dur.count();
		std::cout << std::format("Time elapsed on solving: {0:.4f} ms\n", solve_duration);

		auto print_start = std::chrono::steady_clock::now();
		std::string res = matrix_chain_order::get_result(s);
		auto print_finish = std::chrono::steady_clock::now();
		milliseconds print_dur = print_finish - print_start;
		double print_duration = print_dur.count();
		std::cout << std::format("Time elapsed on printing: {0:.4f} ms\n", print_duration);

		if (n == 5) {
			std::cout << "Table m:\n";
			for (int i = 1; i <= n; ++i) {
				for (int j = i; j <= n; ++j) {
					std::cout << m[i][j] << ' ';
				}
				std::cout << std::endl;
			}
			std::cout << "Table s:\n";
			for (int i = 1; i < n; ++i) {
				for (int j = i + 1; j <= n; ++j) {
					std::cout << s[i][j] << ' ';
				}
				std::cout << std::endl;
			}
		}
		std::cout << std::format("Minimum number of operations: {0}\n", cnt);
		std::cout << std::format("Optimized solution: {0}\n", res);
	}
}

inline void test_lcs() {

}

int main() {
	test_mco();
	return 0;
}