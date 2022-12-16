#pragma once

// Only implement insert and extract_min.
template<class val_type>
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
	val_type min_val() const { return min->val; }
	size_t size() const { return size; }

	void insert(val_type val);
	node* extract_min();
	// return false if val > x->val
	bool decrease_key(node* x, val_type val);
	~fib_heap();
private:
	node* min = nullptr;
	size_t size = 0;

	node* create_circular_list(node* x);
	void insert_circular_list(node* list, node* x);
	void remove_circular_list(node* list, node* x);
	void consolidate();
	void link(node* y, node* x);
	void insert(node* x);
	void cut(node* x, node* y);
	void cascading_cut(node* y);
};

template<class val_type>
void fib_heap<val_type>::insert(val_type val) {
	insert(new node { val });
}

template<class val_type>
fib_heap<val_type>::node* fib_heap<val_type>::extract_min() {
	node* z = min;
	if (z) {
		node* x = z->child;
		do {
			node* right = x->right;
			insert_circular_list(min, x);
			x->parent = nullptr;
		} while (x != z->child);
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

#include <queue>

template<class val_type>
fib_heap<val_type>::~fib_heap() {
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

template<class val_type>
fib_heap<val_type>::node* fib_heap<val_type>::create_circular_list(node* x) {
	x->right = x->left = x;
	return x;
}

template<class val_type>
void fib_heap<val_type>::insert_circular_list(node* list, node* x) {
	if (!list) return;
	x->right = list->right;
	x->left = list;
	list->right->left = x;
	list->right = x;
}

template<class val_type>
void fib_heap<val_type>::remove_circular_list(node* list, node* x) {
	x->left->right = x->right;
	x->right->left = x->left;
}

#include <cmath>
#include <vector>

template<class val_type>
void fib_heap<val_type>::consolidate() {
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

template<class val_type>
void fib_heap<val_type>::link(node* y, node* x) {
	remove_circular_list(min, y);
	insert_circular_list(x->child, y);
	++x->degree;
	y->mark = false;
}

template<class val_type>
void fib_heap<val_type>::insert(node* x) {
	if (!min) min = create_circular_list(x);
	else {
		insert_circular_list(min, x);
		if (x->val < min->val) min = x;
	}
	++size;
}

template<class val_type>
bool fib_heap<val_type>::decrease_key(node* x, val_type val) {
	if (val > x->val) return false;
	x->val = val;
	node* y = x->parent;
	if (y && x->val < y->val) {
		cut(x, y);
		cascading_cut(y);
	}
	if (x->val < min->val) min = x;
	return true;
}

template<class val_type>
void fib_heap<val_type>::cut(node* x, node* y) {
	remove_circular_list(y->child, x);
	--y->degree;
	insert_circular_list(min, x);
	x->parent = nullptr;
	x->mark = false;
}

template<class val_type>
void fib_heap<val_type>::cascading_cut(node* y) {
	node* z = y->parent;
	if (z) {
		if (!y->mark) y->mark = true;
		else {
			cut(y, z);
			cascading_cut(z);
		}
	}
}
