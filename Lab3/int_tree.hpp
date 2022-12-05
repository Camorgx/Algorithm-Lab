#pragma once

#include "rb_tree.hpp"

// 区间树，派生自红黑树
class int_tree : public rb_tree {
public:
	struct interval {
		int low = 0;
		int high = 0;

		bool overlap(const interval& ano) const;
		std::string to_string() const;

		interval() = default;
		interval(int l, int r) : low(l), high(r) { }
	};
	// 区间树节点，派生自红黑树节点
	class node : public rb_tree::node {
	public:
		interval intl;
		int max = 0;

		node() : rb_tree::node() { }
		node(const interval& intl) : rb_tree::node(intl.low), intl(intl), max(intl.high) { }

		void left_rotate(rb_tree* tree) override;
		void right_rotate(rb_tree* tree) override;
		std::string to_string() const override;

		~node() = default;
	};
	node* search_overlap(const interval& intl) const;
	node* search_exact(const interval& intl) const;
	void insert(const interval& intl);

	int_tree();
	~int_tree() = default;
};