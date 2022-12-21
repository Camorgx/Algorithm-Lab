#include "fib_heap.hpp"
#include "test.hpp"

#include <concepts>
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

void test_fib_op() {
	int op[] = { 1, 2, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 2, 1, 2, 2, 1, 1, 2, 1, 1, 1, 2 };
	int ins[] = { 95, 0, 16, 0, 24, 68, 0, 57, 19, 88, 75, 52, 0, 76, 0, 82, 54, 66, 29, 92, 73, 93, 31, 100, 0, 40, 0, 0, 89, 0, 0, 13, 27, 0, 32, 4, 71, 0, };
	fib_heap<int> heap;
	std::vector<int> arr = { 20,10,12,9,8,0,3,11,19,5,2,4,15,6,14,1,13,17,17,7,16, };
	for (int i = 0; i < arr.size(); ++i)
		heap.insert(arr[i]);
	for (int i = 0; i < arr.size(); ++i) {
		auto p = heap.extract_min();
		display_heap(heap.get_min());
		delete p;
	}
}

#include "Johnson.hpp"

void test_dijkstra() {
	unsigned from[] = { 1, 2, 2, 1, 3, 1 };
	unsigned to[] = { 2, 3, 4, 3, 4, 4 };
	int weight[] = { 2, 2, 1, 5, 3, 4 };
	graph g(5);
	for (int i = 0; i < 6; ++i) {
		g.add_edge(from[i], to[i], weight[i]);
	}
	auto nodes = prepare_for_dij(g);
	Dijkstra(g, 1, nodes);
	for (int i = 1; i < 5; ++i) {
		const auto& ver = g.vertexes[i];
		std::cout << std::format("v = {}, d = {}, pi = {}\n",
			i, ver.d, ver.pi);
	}
	std::cout << std::endl;
	for (auto node : nodes) delete node;
}

#include "utils.hpp"

void test_Johnson(int ver_cnt, int edge_cnt, 
	unsigned from[], unsigned to[], int weight[]) {
	graph g(ver_cnt);
	for (int i = 0; i < edge_cnt; ++i) {
		g.add_edge(from[i], to[i], weight[i]);
	}
	ans_matrix d, pi;
	for (int i = 0; i < ver_cnt; ++i) {
		d.emplace_back(ver_cnt);
		pi.emplace_back(ver_cnt);
	}
	Johnson(g, d, pi);
	std::cout << "d:\n";
	for (int i = 0; i < ver_cnt; ++i) {
		for (int j = 0; j < ver_cnt; ++j)
			std::cout << d[i][j] << ' ';
		std::cout << std::endl;
	}
	std::cout << "pi:\n";
	for (int i = 0; i < ver_cnt; ++i) {
		for (int j = 0; j < ver_cnt; ++j)
			std::cout << pi[i][j] << ' ';
		std::cout << std::endl;
	}
	display_output(std::cout, d, pi);
}

void test_Johnson() {
	unsigned from[] = { 1, 2, 2, 1, 3, 1 };
	unsigned to[] = { 2, 3, 4, 3, 4, 4 };
	int weight[] = { 2, 2, 1, 5, 3, 4 };
	test_Johnson(5, 6, from, to, weight);
}

void test_Floyed(int ver_cnt, int edge_cnt,
	unsigned from[], unsigned to[], int weight[]) {
	ans_matrix d(ver_cnt, std::vector<int>(ver_cnt, 
		std::numeric_limits<int>::max() >> 1));
	ans_matrix pi(ver_cnt, std::vector<int>(ver_cnt, -1));
	for (int i = 0; i < ver_cnt; ++i)
		d[i][i] = 0;
	for (int i = 0; i < edge_cnt; ++i) {
		d[from[i]][to[i]] = weight[i];
		pi[from[i]][to[i]] = from[i];
	}
	for (int k = 0; k < ver_cnt; ++k) {
		for (int i = 0; i < ver_cnt; ++i) {
			for (int j = 0; j < ver_cnt; ++j) {
				if (d[i][j] > d[i][k] + d[k][j]) {
					d[i][j] = d[i][k] + d[k][j];
					pi[i][j] = pi[k][j];
				}
			}
		}
	}
	std::cout << "d:\n";
	for (int i = 0; i < ver_cnt; ++i) {
		for (int j = 0; j < ver_cnt; ++j)
			std::cout << d[i][j] << ' ';
		std::cout << std::endl;
	}
	std::cout << "pi:\n";
	for (int i = 0; i < ver_cnt; ++i) {
		for (int j = 0; j < ver_cnt; ++j)
			std::cout << pi[i][j] << ' ';
		std::cout << std::endl;
	}
}

void cmp_Johnson_Floyed() {
	unsigned from[] = { 1, 2, 2, 3, 3, 4, 4, 5, 6, 6 };
	unsigned to[] = { 5, 1, 4, 2, 6, 1, 5, 2, 3, 2 };
	int weight[] = { -1, 1, 2, 2, -8, -4, 3, 7, 10, 5 };
	
	std::cout << "Floyed\n\n";
	test_Floyed(7, 10, from, to, weight);
	std::cout << std::endl;

	std::cout << "Johnson\n\n";
	test_Johnson(7, 10, from, to, weight);
	std::cout << std::endl;
}

void test_break_negative_circle() {
	unsigned from[] = { 0, 1, 3, 1, 1, 0, 2, 4 };
	unsigned to[] = { 1, 2, 1, 3, 4, 2, 3, 3 };
	int weight[] = { -1, 3, 1, 2, 2, 4, -5, -3 };
	// negative circle: 1 -> 2 -> 3 -> 1
	graph g(5);
	for (int i = 0; i < 8; ++i)
		g.add_edge(from[i], to[i], weight[i]);
	break_negative_circle(g);
	for (int u = 0; u < g.vertex_cnt; ++u) {
		for (auto e = g.edges[u]; e; e = e->next) {
			std::cout << std::format("{} -> {}, w = {}\n", u, e->vertex, e->weight);
		}
	}
	std::cout << std::endl;
}
