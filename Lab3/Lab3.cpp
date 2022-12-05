#include "int_tree.hpp"

#include <algorithm> // shuffle
#include <format>
#include <fstream>
#include <iostream>
#include <random>

void test_rb_tree() { // passed
	const int test_insert[] = { 41, 38, 31, 12, 19, 8 };
	const int test_remove[] = { 8, 12, 19, 31, 38, 41 };
	rb_tree tree;
	for (int test : test_insert) {
		std::cout << std::format("Inserting {0}\n", test);
		tree.insert(new rb_tree::node(test));
		std::cout << tree.to_string() << std::endl;
	}
	std::cout << "Insertion finished.\n" << std::endl;
	for (int test : test_remove) {
		auto res = tree.search(test);
		if (res != tree.nil) {
			std::cout << std::format("Find item {0}\n", test);
			tree.remove(res);
			std::cout << std::format("Item {0} removed\n", test);
			std::cout << tree.to_string() << std::endl;
		}
		else {
			std::cout << std::format("Item {0} not found!\n", test);
		}
	}
	std::cout << "Deletion finished.\n" << std::endl;
}

void generate_random_input() {
	int_tree::interval test_intl[30];
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<> dis_1_5(1, 5);
	for (int i = 0; i < 15; ++i) {
		test_intl[i].low = i;
		test_intl[i + 15].low = 30 + i;
		test_intl[i].high = i + dis_1_5(gen);
		test_intl[i + 15].high = 30 + i + dis_1_5(gen);
	}
	std::shuffle(test_intl, test_intl + 30, gen);
	std::ofstream fout("ex1/input/input.txt");
	for (const auto& intl : test_intl)
		fout << intl.to_string() << std::endl;
	fout.close();
}

void test_int_tree() { // passed
	int_tree::interval test_intl[30];
	std::ifstream fin("ex1/input/input.txt");
	if (!fin.is_open()) {
		std::cerr << "Failed to open input file." << std::endl;
		return;
	}
	for (int i = 0; i < 30; ++i)
		fin >> test_intl[i].low >> test_intl[i].high;
	fin.close();
	std::cout << "Intervals are read from input file." << std::endl;
	int_tree tree;
	for (const auto& intl : test_intl)
		tree.insert(intl);
	std::ofstream fout("ex1/output/inorder.txt");
	fout << tree.to_string("\n");
	fout.close();
	std::cout << "Interval tree written into inorder.txt.\n" << std::endl;

	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<> dis_0_29(0, 29);
	int intl_remove[] = { dis_0_29(gen), dis_0_29(gen), dis_0_29(gen) };
	fout.open("ex1/output/delete_data.txt");
	for (int i = 0; i < 3; ++i) {
		std::cout << std::format("Removing interval {0}: {1}\n",
			intl_remove[i], test_intl[intl_remove[i]].to_string());
		fout << test_intl[intl_remove[i]].to_string() << std::endl;
		auto pos = tree.search_exact(test_intl[intl_remove[i]]);
		if (pos == tree.nil) {
			std::cerr << "Target not found!" << std::endl;
			return;
		}
		tree.remove(pos);
		std::cout << "Target removed." << std::endl;
	}
	fout << tree.to_string("\n");
	fout.close();
	std::cout << "Interval tree after deletion written into delete_data.txt.\n" << std::endl;

	std::uniform_int_distribution<> dis_0_5(0, 5);
	std::uniform_int_distribution<> dis_0_45(0, 45);
	int base[] = { 25, dis_0_45(gen), dis_0_45(gen) };
	fout.open("ex1/output/search.txt");
	for (int i = 0; i < 3; ++i) {
		int_tree::interval test_search(base[i], base[i] + dis_0_5(gen));
		std::cout << std::format("Try searching for interval: {0}\n", test_search.to_string());
		auto pos = tree.search_overlap(test_search);
		if (pos == tree.nil) {
			std::cout << "Target not found." << std::endl;
			fout << std::format("{0} NULL\n", test_search.to_string());
		}
		else {
			std::cout << std::format("Target found: {0}", pos->intl.to_string()) << std::endl;
			fout << std::format("{0} {1}\n", test_search.to_string(), pos->intl.to_string());
		}
	}
	fout.close();
	std::cout << "Search result written into search.txt." << std::endl;
};

int main() {
	test_int_tree();
	return 0;
}
