#pragma once

#include <cmath>
#include <queue>
#include <vector>

template <typename val_type>
concept fib_heap_val = requires (val_type val1, val_type val2) {
	{ val1 < val2 } -> std::same_as<bool>;
	{ val1 > val2 } -> std::same_as<bool>;
};

// Only implement insert and extract_min
// Use as priority queue
template <fib_heap_val val_type>
class fib_heap {
	struct node {
		val_type val;
		size_t degree = 0;
		bool mark = false;
		node* parent = nullptr;
		node* child = nullptr;
		node* left = nullptr;
		node* right = nullptr;
	};
public:
	node* get_min() const { return min; }

	size_t count() const { return size; }

	void insert(const val_type& val) {
		insert(new node { val });
	}

	node* extract_min() {
		node* z = min;
		if (z) {
			node* x = z->child;
			if (x) {
				do {
					node* right = x->right;
					insert_circular_list(min, x);
					x->parent = nullptr;
					x = right;
				} while (x != z->child);
			}
			remove_circular_list(min, z);
			if (z == z->right) min = nullptr;
			else {
				min = z->right;
				consolidate();
			}
			--size;
		}
		return z;
	}

	~fib_heap() {
		node* x = min;
		if (!x) return;
		node* p = x->right;
		std::queue<node*> destroy_queue;

		// Enqueue root linked list
		destroy_queue.push(x);
		while (p != x) {
			destroy_queue.push(p);
			p = p->right;
		}

		// BFS
		while (!destroy_queue.empty()) {
			node* x = destroy_queue.front();
			destroy_queue.pop();
			node* p = x->child;
			if (p) {
				do {
					destroy_queue.push(p);
					p = p->right;
				} while (p != x->child);
			}
			delete x;
		}
	}
private:
	node* min = nullptr;
	size_t size = 0;

	node* create_circular_list(node* x) {
		x->right = x->left = x;
		return x;
	}

	// requires list not nullptr
	void insert_circular_list(node* list, node* x) {
		if (!list) return;
		x->right = list->right;
		x->left = list;
		list->right->left = x;
		list->right = x;
	}

	// if list == x, this will also modify list itself
	void remove_circular_list(node* &list, node* x) {
		if (list == x)
			list = (x == x->right) ? nullptr : x->right;
		x->left->right = x->right;
		x->right->left = x->left;
	}

	void consolidate() {
		constexpr double log_phi = 0.481211825059603; // log_e((1 + sqrt(5)) / 2)
		int Dn = static_cast<int>(std::log(size) / log_phi) + 1;
		std::vector<node*> a(Dn);
		node* w = min;
		do {
			node* x = w;
			size_t d = x->degree;
			while (a[d]) {
				node* y = a[d];
				if (x->val > y->val) std::swap(x, y);
				link(y, x);
				a[d++] = nullptr;
			}
			a[d] = x;
			w = x->right;
		} while (w != min);
		min = nullptr;
		for (node* x : a) {
			if (x) {
				if (!min) min = create_circular_list(x);
				else {
					insert_circular_list(min, x);
					if (x->val < min->val) min = x;
				}
			}
		}
	}

	void link(node* y, node* x) {
		remove_circular_list(min, y);
		if (!x->child) x->child = create_circular_list(y);
		else insert_circular_list(x->child, y);
		++x->degree;
		y->mark = false;
		y->parent = x;
	}

	void insert(node* x) {
		if (!min) min = create_circular_list(x);
		else {
			insert_circular_list(min, x);
			if (x->val < min->val) min = x;
		}
		++size;
	}
};
