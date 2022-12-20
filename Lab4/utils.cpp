#include "utils.hpp"

#include <cmath>
#include <chrono>
#include <format>
#include <fstream>
#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>

void generate_random_input() {
	constexpr int test_N[] = { 27, 81, 243, 729 };
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution weight(-10, 50);
	for (int test_id = 0; test_id < 4; ++test_id) {
		int N = test_N[test_id];
		std::uniform_int_distribution to(0, N - 1);
		// log_e 5, log_e 7
		constexpr double logs[] = { 1.6094379124341, 1.9459101490553 };
		for (int edge_id = 0; edge_id < 2; ++edge_id) {
			std::string output_file = std::format("input/input{}{}.txt",
				test_id + 1, edge_id + 1);
			std::ofstream fout(output_file);
			int edge_cnt = static_cast<int>(std::log(N) / logs[edge_id]);
			std::cout << std::format("Generating file: {}\n", output_file);
			std::cout << std::format("Vertex count: {}\n", N);
			std::cout << std::format("Edge count: {}\n", N * edge_cnt);
			for (int v = 0; v < N; ++v) {
				std::unordered_set<int> set;
				for (int e = 0; e < edge_cnt; ++e) {
					int target;
					do {
						target = to(gen);
					} while (set.find(target) != set.end());
					int w = weight(gen);
					fout << std::format("{} {} {}\n", v, target, w);
				}
			}
			fout.close();
			std::cout << "Generation complete." << std::endl;
		}
	}
}

void display_output(std::ostream& file, const ans_matrix& d, const ans_matrix& pi) {
	int v_cnt = static_cast<int>(d.size());
	for (int u = 0; u < v_cnt; ++u) {
		for (int v = 0; v < v_cnt; ++v) {
			if (u == v) {
				file << std::format("({} {})\n", u, 0);
				continue;
			}
			std::vector<int> path;
			int ver = v;
			while (ver != u && ver != -1) {
				path.push_back(ver);
				ver = pi[u][ver];
			}
			if (ver != u)
				file << std::format("({} {} Unreachable)\n", u, v);
			else {
				std::string o_str = std::format("({},", u);
				for (int i = static_cast<int>(path.size() - 1); i >= 1; --i)
					o_str += std::format("{},", path[i]);
				o_str += std::format("{} {})\n", path[0], d[u][v]);
				file << o_str;
			}
		}
	}
}

void run_experiment() {
	constexpr int test_N[] = { 27, 81, 243, 729 };
	std::ofstream exc_time("output/time.txt");
	for (int test_id = 0; test_id < 4; ++test_id) {
		int ver_cnt = test_N[test_id];
		// log_e 5, log_e 7
		constexpr double logs[] = { 1.6094379124341, 1.9459101490553 };
		for (int edge_id = 0; edge_id < 2; ++edge_id) {
			std::string input_file = std::format("input/input{}{}.txt",
				test_id + 1, edge_id + 1);
			std::string output_file = std::format("output/result{}{}.txt",
				test_id + 1, edge_id + 1);
			std::ifstream input(input_file);
			std::ofstream output(output_file);
			int edge_cnt = static_cast<int>(std::log(ver_cnt) / logs[edge_id]) * ver_cnt;
			std::cout << std::format("Reading data from {}.\n", input_file);
			graph g(ver_cnt);
			for (int i = 0; i < edge_cnt; ++i) {
				unsigned from, to;
				int weight;
				input >> from >> to >> weight;
				g.add_edge(from, to, weight);
			}
			std::cout << std::format("Total vertex count: {}\nTotal edge count: {}\n",
				ver_cnt, edge_cnt);
			input.close();
			// input data finished
			
			break_negative_circle(g);
			if (g.edge_cnt != edge_cnt) {
				std::cout << "Negative circle detected. "
					<< std::format("{} edges left.\n", 
						(edge_cnt = static_cast<int>(g.edge_cnt)));
			}
			// break negative circle finished

			std::cout << "Start executing." << std::endl;
			ans_matrix d, pi;
			for (int i = 0; i < ver_cnt; ++i) {
				d.emplace_back(ver_cnt);
				pi.emplace_back(ver_cnt);
			}
			using milliseconds = std::chrono::duration<double, std::milli>;
			auto start = std::chrono::steady_clock::now();
			// start clock
			Johnson(g, d, pi);
			// end clock
			auto finish = std::chrono::steady_clock::now();
			double duration = milliseconds(finish - start).count();
			std::cout << "Execution finished. "
				<< std::format("Time elapsed: {:.4f}ms.", duration);
			exc_time << std::format("vertex = {}, edge = {}, time = {:.4f}\n",
				ver_cnt, edge_cnt, duration);
			display_output(output, d, pi);
			std::cout << std::format("Result written into {}.\n", output_file);
			// johnson algorithm execution finished

			std::cout << std::endl;
		}
	}
}
