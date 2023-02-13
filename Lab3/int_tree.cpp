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
	return std::format("{0} {1}", low, high);
}

std::string int_tree::node::to_string() const {
	return std::format("{0} {1}", intl.to_string(), max);
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
	auto x = this;
	auto y = dynamic_cast<int_tree::node*>(x->parent);
	auto l = dynamic_cast<int_tree::node*>(x->left);
	auto r = dynamic_cast<int_tree::node*>(x->right);
	y->max = x->max;
	x->max = std::max(x->intl.high, std::max(l->max, r->max));
}

void int_tree::node::right_rotate(rb_tree* tree) {
	rb_tree::node::right_rotate(tree);
	auto y = this;
	auto x = dynamic_cast<int_tree::node*>(y->parent);
	auto l = dynamic_cast<int_tree::node*>(y->left);
	auto r = dynamic_cast<int_tree::node*>(y->right);
	x->max = y->max;
	y->max = std::max(y->intl.high, std::max(l->max, r->max));
}

void int_tree::insert(const int_tree::interval& intl) {
	node* x = new node(intl);
	rb_tree::insert(x);
	node* p = dynamic_cast<int_tree::node*>(x->parent);
	while (p != nil) {
		if (p->max < x->max) p->max = x->max;
		else break;
		p = dynamic_cast<int_tree::node*>(p->parent);
	}
}

void int_tree::max_fix_up(int_tree::node* p) {
	while (p != nil) {
		p->max = std::max(p->intl.high,
			std::max(dynamic_cast<int_tree::node*>(p->left)->max,
				dynamic_cast<int_tree::node*>(p->right)->max));
		p = dynamic_cast<int_tree::node*>(p->parent);
	}
}

void int_tree::remove(rb_tree::node* z) {
	auto y = z;
	auto y_origin_color = y->color;
	rb_tree::node* x;
	if (z->left == nil) {
		x = z->right;
		dynamic_cast<int_tree::node*>(z)->max = 0;
		auto p = dynamic_cast<int_tree::node*>(z->parent);
		max_fix_up(p);
		transplant(z, z->right);
	}
	else if (z->right == nil) {
		x = z->left;
		dynamic_cast<int_tree::node*>(z)->max = 0;
		auto p = dynamic_cast<int_tree::node*>(z->parent);
		max_fix_up(p);
		transplant(z, z->left);
	}
	else {
		y = minimum(z->right);
		auto p = dynamic_cast<int_tree::node*>(y->parent);
		y_origin_color = y->color;
		x = y->right;
		if (y->parent == z)
			x->parent = y;
		else {
			transplant(y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		transplant(z, y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
		max_fix_up(p);
	}
	if (y_origin_color == node::color::black)
		remove_fixup(x);
	delete z;
}
