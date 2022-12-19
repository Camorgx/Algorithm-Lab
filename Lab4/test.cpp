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

template <test_fib_node node, test_fib_val val_type>
node* search_for_val(node* root_list, const val_type& val) {
	using std::to_string;

	node* x = root_list;
	if (!x) return nullptr;
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
		if (x->val == val) return x;
	}
	return nullptr;
}

struct test_pair {
	int first = 0;
	int second = 0;
	auto operator<=>(const test_pair& ano) const {
		return first <=> ano.first;
	}
	bool operator==(const test_pair& ano) const {
		return first == ano.first;
	}
};

std::string to_string(const test_pair& obj) {
	return std::format("({0}, {1})", obj.first, obj.second);
}

template <test_fib_val val_type>
void test_fib_heap(std::vector<val_type> tests, val_type ori, val_type dec) {
	using std::to_string;

	fib_heap<val_type> heap;
	
	for (const auto& t : tests)
		heap.insert(t);
	std::cout << "Insert finished." << std::endl;
	std::cout << "heap now: ";
	display_heap(heap.get_min());
	std::cout << std::endl;

	while (heap.count()) {
		if (heap.count() == tests.size() - 1) { // have called extract_min once
			std::cout << std::format("Decrease {} to {}.\n", 
				to_string(ori), to_string(dec));
			auto pos = search_for_val(heap.get_min(), ori);
			heap.decrease_key(pos, dec);
			std::cout << "heap now: ";
			display_heap(heap.get_min());
			std::cout << std::endl;
		}
		auto min = heap.extract_min();
		val_type val = min->val;
		delete min;
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
	test_fib_heap(tests_int, 5, 0);
	test_fib_heap(tests_pair, test_pair { 5, 5 }, test_pair { 0, 0 });
}
