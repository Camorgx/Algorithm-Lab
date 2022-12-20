#include "test.hpp"
#include "utils.hpp"

#include <iostream>

int main() {
	std::cout << "Please choose:\n"
		"1. Generate random input.\n"
		"2. Run experiment.\n"
		"3. Run test.\n";
	int choice;
	std::cin >> choice;
	if (choice == 1) generate_random_input();
	else if (choice == 2) run_experiment();
	else if (choice == 3) {
		std::cout << "Please choose:\n"
			"1. test_fib_heap\n"
			"2. test_dijkstra\n"
			"3. test_Johnson\n"
			"4. cmp_Johnson_Floyed\n"
			"5. test_break_negative_circle\n";
		std::cin >> choice;
		if (choice == 1) test_fib_heap();
		else if (choice == 2) test_dijkstra();
		else if (choice == 3) test_Johnson();
		else if (choice == 4) cmp_Johnson_Floyed();
		else if (choice == 5) test_break_negative_circle();
		else std::cout << "Invalid input." << std::endl;
	}
	else std::cout << "Invalid input." << std::endl;
	return 0;
}
