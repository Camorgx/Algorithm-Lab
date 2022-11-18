#include "DP.h"

#include <chrono>
#include <fstream>
#include <format>
#include <iostream>

using milliseconds = std::chrono::duration<double, std::milli>;

inline void test_mco() {
	std::cout << "ex1: \n";
	std::ifstream fin("ex1/input/2_1_input.txt");
	std::ofstream result("ex1/output/result.txt");
	std::ofstream time("ex1/output/time.txt");
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
		double total_duration = print_duration + solve_duration;
		std::cout << std::format("Time elapsed on printing: {0:.4f} ms\n", print_duration);
		std::cout << std::format("Total time elapsed: {0:.4f} ms\n", total_duration);

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

		result << std::format("{0}\n{1}\n", cnt, res);
		time << std::format("{0:.4f} {1:.4f} {2:.4f}\n", 
			solve_duration, print_duration, total_duration);
	}
	fin.close();
	result.close();
	time.close();
	std::cout << std::endl;
}

inline void test_lcs() {
	std::cout << "ex2: \n";
	std::ifstream fin("ex2/input/2_2_input.txt");
	std::ofstream time("ex2/output/time.txt");
	for (int cas = 1; cas <= 5; ++cas) {
		std::cout << std::format("\nCase {0}:\n", cas);
		size_t n;
		std::string x, y;
		fin >> n >> x >> y;

		std::vector<std::vector<char>> b;
		auto solve_start = std::chrono::steady_clock::now();
		int64_t cnt = longest_common_subsequence::solve(x, y, b);
		auto solve_finish = std::chrono::steady_clock::now();
		milliseconds solve_dur = solve_finish - solve_start;
		double solve_duration = solve_dur.count();
		std::cout << std::format("Time elapsed on solving: {0:.4f} ms\n", solve_duration);

		auto print_start = std::chrono::steady_clock::now();
		std::string res = longest_common_subsequence::get_result(b, x, y);
		auto print_finish = std::chrono::steady_clock::now();
		milliseconds print_dur = print_finish - print_start;
		double print_duration = print_dur.count();
		double total_duration = print_duration + solve_duration;
		std::cout << std::format("Time elapsed on printing: {0:.4f} ms\n", print_duration);
		std::cout << std::format("Total time elapsed: {0:.4f} ms\n", total_duration);

		std::cout << std::format("Max length of LCS: {0}\n", cnt);
		std::cout << std::format("Longest common subsequence: {0}\n", res);

		std::ofstream result(std::format("ex2/output/result_{0}.txt", cas));
		result << std::format("{0}\n{1}\n", cnt, res);
		result.close();
		time << std::format("{0:.4f} {1:.4f} {2:.4f}\n",
			solve_duration, print_duration, total_duration);
	}
	fin.close();
	time.close();
}

int main() {
	test_mco();
	test_lcs();
	return 0;
}