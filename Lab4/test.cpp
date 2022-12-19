#include "fib_heap.hpp"
#include "test.hpp"

#include <iostream>
#include <format>

template <typename node>
concept test_fib_node = (
	requires (node * x) {
		std::same_as<decltype(x->right), node*>;
		std::same_as<decltype(x->child), node*>;
		to_string(x->val);
	}
	||
	requires (node * x) {
		std::same_as<decltype(x->right), node*>;
		std::same_as<decltype(x->child), node*>;
		std::to_string(x->val);
	}
);

template <test_fib_node node>
void display_heap(node* root_list) {
	using std::to_string;

	node* x = root_list;
	if (!x) {
		std::cout << "(null)" << std::endl;
		return;
	}
	node* p = x->right;
	std::queue<node*> display_queue;

	// Enqueue root linked list
	display_queue.push(x);
	while (p != x) {
		display_queue.push(p);
		p = p->right;
	}

	// BFS
	while (!display_queue.empty()) {
		node* x = display_queue.front();
		display_queue.pop();
		node* p = x->child;
		if (p) {
			do {
				display_queue.push(p);
				p = p->right;
			} while (p != x->child);
		}
		std::cout << to_string(x->val) << ' ';
	}
	std::cout << std::endl;
}

struct test_pair {
	int first = 0;
	int second = 0;
	auto operator<=>(const test_pair& ano) const {
		return first <=> ano.first;
	}
};

std::string to_string(const test_pair& obj) {
	return std::format("({0}, {1})", obj.first, obj.second);
}

template <typename type>
concept test_fib_val = (
	requires (type x) { 
		fib_heap_val<type>; 
		std::to_string(x); 
	} 
	|| 
	requires (type x) { 
		fib_heap_val<type>; 
		to_string(x); 
	}
);

template <test_fib_val val_type>
void test_fib_heap(std::vector<val_type> tests) {
	using std::to_string;

	fib_heap<val_type> heap;
	
	for (const auto& t : tests)
		heap.insert(t);
	std::cout << "Insert finished." << std::endl;
	std::cout << "heap now: ";
	display_heap(heap.get_min());
	std::cout << std::endl;

	while (heap.count()) {
		val_type val = heap.extract_min()->val;
		std::cout << std::format("extract_min: {0}\n", to_string(val));
		std::cout << std::format("current size: {0}\n", heap.count());
		std::cout << "heap now: ";
		display_heap(heap.get_min());
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void test_fib_heap() {
	std::vector<int> tests_int = { 8, 2, 6, 4, 5, 7, 9, 3, 1 };
	std::vector<test_pair> tests_pair = { { 8, 1 }, { 2, 2 }, { 6, 3 }, { 4, 4 },
		{ 5, 5 }, { 7, 6 }, { 9, 7 }, { 3, 8 }, { 1, 9 } };
	test_fib_heap(tests_int);
	test_fib_heap(tests_pair);
}
