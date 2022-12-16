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
	val_type min() const { return min->val; }
	size_t size() const { return size; }

	void insert(val_type val) {

	}

	val_type extract_min() {

	}

	~fib_heap() {

	}
private:
	node* min = nullptr;
	size_t size = 0;

	void insert(node* node) {

	}
};