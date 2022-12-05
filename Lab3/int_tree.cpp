#include "int_tree.hpp"

#include <format>
#include <utility>

int_tree::int_tree() {
	nil = new node;
	nil->parent = nil;
	nil->left = nil->right = nil;
	root = nil;
}

std::string int_tree::interval::to_string() const {
	return std::format("[{0}, {1}]", low, high);
}

std::string int_tree::node::to_string() const {
	return std::format("int = {0}, max = {1}", intl.to_string(), max);
}

bool int_tree::interval::overlap(const int_tree::interval& ano) const {
	return low <= ano.high && ano.low <= high;
}

int_tree::node* int_tree::search_overlap(const interval& intl) const {
	int_tree::node* x = dynamic_cast<int_tree::node*>(root);
	while (x != nil && !intl.overlap(x->intl)) {
		int_tree::node* left = dynamic_cast<int_tree::node*>(x->left);
		if (x->left != nil && left->max >= intl.low) x = left;
		else x = dynamic_cast<int_tree::node*>(x->right);
	}
	return x;
}

int_tree::node* int_tree::search_exact(const interval& intl) const {
	auto res = search(intl.low);
	if (res == nil || dynamic_cast<int_tree::node*>(res)->intl.high != intl.high)
		return dynamic_cast<int_tree::node*>(nil);
	return dynamic_cast<int_tree::node*>(res);
}

void int_tree::node::left_rotate(rb_tree* tree) {
	rb_tree::node::left_rotate(tree);
	auto l = dynamic_cast<int_tree::node*>(left);
	auto r = dynamic_cast<int_tree::node*>(right);
	r->max = max;
	max = std::max(intl.high, std::max(l->max, r->max));
}

void int_tree::node::right_rotate(rb_tree* tree) {
	rb_tree::node::right_rotate(tree);
	auto l = dynamic_cast<int_tree::node*>(left);
	auto r = dynamic_cast<int_tree::node*>(right);
	l->max = max;
	max = std::max(intl.high, std::max(l->max, r->max));
}

void int_tree::insert(const int_tree::interval& intl) {
	node* x = new node(intl);
	rb_tree::insert(x);
}
