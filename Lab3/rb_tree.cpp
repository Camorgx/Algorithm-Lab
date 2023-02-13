#include "rb_tree.hpp"

#include <format>

rb_tree::rb_tree() {
	nil = new node;
	nil->parent = nil;
	nil->left = nil->right = nil;
	root = nil;
}

rb_tree::~rb_tree() {
	destruct_tree(root);
	delete nil;
}

void rb_tree::destruct_tree(rb_tree::node* root) {
	if (root == nil) return;
	destruct_tree(root->left);
	destruct_tree(root->right);
	delete root;
}

std::string rb_tree::node::to_string() const {
	return std::format("{0}", key);
}

std::string rb_tree::to_string(const rb_tree::node* rt, 
	const std::string& join) const {
	if (rt == nil) return "";
	return std::format("{0}{1}{2}{3}", to_string(rt->left, join), 
		rt->to_string(), join, to_string(rt->right, join));
}

std::string rb_tree::to_string(const std::string& join) const {
	return to_string(root, join);
}

void rb_tree::node::left_rotate(rb_tree* tree) {
	auto y = right;
	right = y->left;
	if (y->left != tree->nil)
		y->left->parent = this;
	y->parent = parent;
	if (parent == tree->nil)
		tree->root = y;
	else if (this == parent->left)
		parent->left = y;
	else parent->right = y;
	y->left = this;
	parent = y;
}

void rb_tree::node::right_rotate(rb_tree* tree) {
	auto y = left;
	left = y->right;
	if (y->right != tree->nil)
		y->right->parent = this;
	y->parent = parent;
	if (parent == tree->nil)
		tree->root = y;
	else if (this == parent->right)
		parent->right = y;
	else parent->left = y;
	y->right = this;
	parent = y;
}

void rb_tree::insert(rb_tree::node* z) {
	auto y = nil;
	auto x = root;
	while (x != nil) {
		y = x;
		x = (z->key < x->key) ? x->left : x->right;
	}
	z->parent = y;
	if (y == nil) 
		root = z;
	else if (z->key < y->key) 
		y->left = z;
	else y->right = z;
	z->left = nil;
	z->right = nil;
	z->color = node::color::red;
	insert_fixup(z);
}

void rb_tree::insert_fixup(rb_tree::node* z) {
	while (z->parent->color == node::color::red) {
		if (z->parent == z->parent->parent->left) {
			auto y = z->parent->parent->right;
			if (y->color == node::color::red) { // case 1
				z->parent->color = node::color::black;
				y->color = node::color::black;
				z->parent->parent->color = node::color::red;
				z = z->parent->parent;
			}
			else {
				if (z == z->parent->right) { // case 2
					z = z->parent;
					z->left_rotate(this);
				}
				z->parent->color = node::color::black; // case 3
				z->parent->parent->color = node::color::red;
				z->parent->parent->right_rotate(this);
			}
		}
		else {
			auto y = z->parent->parent->left;
			if (y->color == node::color::red) { // case 1
				z->parent->color = node::color::black;
				y->color = node::color::black;
				z->parent->parent->color = node::color::red;
				z = z->parent->parent;
			}
			else {
				if (z == z->parent->left) { // case 2
					z = z->parent;
					z->right_rotate(this);
				}
				z->parent->color = node::color::black; // case 3
				z->parent->parent->color = node::color::red;
				z->parent->parent->left_rotate(this);
			}
		}
	}
	root->color = node::color::black;
}

void rb_tree::transplant(rb_tree::node* u, rb_tree::node* v) {
	if (u->parent == nil)
		root = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else u->parent->right = v;
	v->parent = u->parent;
}

rb_tree::node* rb_tree::minimum(node* root) {
	if (root == nil) return nil;
	while (root->left != nil)
		root = root->left;
	return root;
}

void rb_tree::remove(node* z) {
	auto y = z;
	auto y_origin_color = y->color;
	node* x;
	if (z->left == nil) {
		x = z->right;
		transplant(z, z->right);
	}
	else if (z->right == nil) {
		x = z->left;
		transplant(z, z->left);
	}
	else {
		y = minimum(z->right);
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
	}
	if (y_origin_color == node::color::black)
		remove_fixup(x);
	delete z;
}

void rb_tree::remove_fixup(rb_tree::node* x) {
	while (x != root && x->color == node::color::black) {
		if (x == x->parent->left) {
			auto w = x->parent->right;
			if (w->color == node::color::red) { // case 1
				w->color = node::color::black;
				x->parent->color = node::color::red;
				x->parent->left_rotate(this);
				w = x->parent->right;
			}
			if (w->left->color == node::color::black
				&& w->right->color == node::color::black) { //case 2
				w->color = node::color::red;
				x = x->parent;
			}
			else {
				if (w->right->color == node::color::black) { // case 3
					w->left->color = node::color::black;
					w->color = node::color::red;
					w->right_rotate(this);
					w = x->parent->right;
				}
				w->color = x->parent->color; // case 4
				x->parent->color = node::color::black;
				w->right->color = node::color::black;
				x->parent->left_rotate(this);
				x = root;
			}
		}
		else {
			auto w = x->parent->left;
			if (w->color == node::color::red) { // case 1
				w->color = node::color::black;
				x->parent->color = node::color::red;
				x->parent->right_rotate(this);
				w = x->parent->left;
			}
			if (w->right->color == node::color::black
				&& w->left->color == node::color::black) { //case 2
				w->color = node::color::red;
				x = x->parent;
			}
			else {
				if (w->left->color == node::color::black) { // case 3
					w->right->color = node::color::black;
					w->color = node::color::red;
					w->left_rotate(this);
					w = x->parent->left;
				}
				w->color = x->parent->color; // case 4
				x->parent->color = node::color::black;
				w->left->color = node::color::black;
				x->parent->right_rotate(this);
				x = root;
			}
		}
	}
	x->color = node::color::black;
}

rb_tree::node* rb_tree::search(int key) const {
	auto p = root;
	while (p != nil) {
		if (p->key == key) break;
		p = (p->key > key) ? p->left : p->right;
	}
	return p;
}
