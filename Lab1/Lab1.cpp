#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <random>
#include <string>

#include "sort.h"

using std::vector;
using std::string;
using std::cout, std::endl;
using std::ifstream, std::ofstream;

void display_vector(const vector<int>& vec) {
	for (int x : vec)
		cout << x << ' ';
	cout << endl;
}

void generate_input_file() {
	std::random_device rd;
	std::mt19937 gen(rd());
	// Produces random integer values uniformly 
	// distributed on the closed interval [0, (1 << 15) - 1]
	std::uniform_int_distribution<> dis(0, (1 << 15) - 1);

	// write to input.txt
	ofstream fout("input/input.txt");
	for (int i = 0; i < (1 << 18); ++i)
		fout << dis(gen) << endl;
	fout.close();
}

using milliseconds = std::chrono::duration<double, std::milli>;
const string names[] = { "heap_sort", "quick_sort", "merge_sort", "counting_sort" };
const std::function<void(vector<int>&)> sort_func[] = {
	heap_sort, quick_sort, merge_sort, counting_sort };
vector<double> times[4];

void test_sort(vector<int>& arr, vector<int>& standard, int exp, int method_id) {
	cout << "Testing: " << names[method_id] << endl;
	auto start = std::chrono::steady_clock::now();
	sort_func[method_id](arr);
	auto finish = std::chrono::steady_clock::now();
	// get duration by ms
	milliseconds dur = finish - start;
	double duration = dur.count();
	size_t len = arr.size();
	for (int i = 0; i < len; ++i) {
		if (arr[i] != standard[i]) {
			cout << "Test failed: ";
			cout << "arr[" << i << "] = " << arr[i] << ", ";
			cout << "standard[" << i << "] = " << standard[i] << endl;
			return;
		}
	}
	cout << "Test passed. Time: " << duration << " ms." << endl;
	times[method_id].emplace_back(duration);
	string path = names[method_id] + "/result_" + std::to_string(exp) + ".txt";
	ofstream fout(path);
	for (int item : arr)
		fout << item << endl;
	fout.close();
}

int main() {
	cout << "Generating random numbers..." << endl;
	generate_input_file();
	cout << "Generation complete." << endl;
	const int exps[] = { 3, 6, 9, 12, 15, 18 };
	vector<double> std_time;
	for (int exp : exps) {
		cout << endl;
		cout << "Reading 2^" << exp << " objects from file." << endl;
		vector<int> standard;
		ifstream fin("input/input.txt");
		for (int i = 0; i < (1 << exp); ++i) {
			int val; fin >> val;
			standard.emplace_back(val);
		}
		if (exp == 3) {
			cout << "Origin array: ";
			display_vector(standard);
		}
		fin.close();
		vector<int> tests[] = { standard, standard, standard, standard };
		auto start = std::chrono::steady_clock::now();
		std::sort(standard.begin(), standard.end());
		auto finish = std::chrono::steady_clock::now();
		// get duration by ms
		milliseconds dur = finish - start;
		double duration = dur.count();
		cout << "std::sort used " << duration << "ms." << endl;
		std_time.emplace_back(duration);
		string path = "std_sort/result_" + std::to_string(exp) + ".txt";
		ofstream fout(path);
		for (int item : standard)
			fout << item << endl;
		fout.close();
		for (int i = 0; i < 4; ++i)
			test_sort(tests[i], standard, exp, i);
		if (exp == 3) {
			cout << "std::sort: ";
			display_vector(standard);
			for (int i = 0; i < 4; ++i) {
				cout << names[i] << ": ";
				display_vector(tests[i]);
			}
		}
	}
	cout << endl;
	
	cout << "Summary: " << endl;
	cout << "std::sort: " << endl;
	ofstream fout("std_sort/time.txt");
	for (int i = 0; i < 6; ++i) {
		cout << "exp = " << exps[i] << ", ";
		cout << "duration = " << std_time[i] << " ms" << endl;
		fout << "duration = " << std_time[i] << " ms" << endl;
	}
	fout.close();
	for (int i = 0; i < 4; ++i) {
		string path = names[i] + "/time.txt";
		fout.open(path);
		cout << names[i] << ": " << endl;
		for (int j = 0; j < 6; ++j) {
			cout << "exp = " << exps[j] << ", ";
			cout << "duration = " << times[i][j] << " ms" << endl;
			fout << "duration = " << times[i][j] << " ms" << endl;
		}
		fout.close();
	}
	return 0;
}
