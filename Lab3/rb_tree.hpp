#pragma once

#include <string>

// 红黑树
class rb_tree {
public:
	// 红黑树节点
	class node {
	public:
		int key = 0;
		// 红黑树节点的颜色
		enum class color : bool {
			red,
			black
		};
		color color = color::black;
		node* left = nullptr;
		node* right = nullptr;
		node* parent = nullptr;

		node() = default;
		node(int value) : key(value) { }
		virtual ~node() = default;

		virtual void left_rotate(rb_tree* tree);
		virtual void right_rotate(rb_tree* tree);
		virtual std::string to_string() const;
	};
	node* nil;
	node* root;

	void insert(node* z);
	virtual void remove(node* z);
	node* search(int key) const;
	// 输出中序遍历
	std::string to_string(const std::string& join = " ") const;
	std::string to_string(const rb_tree::node* rt,
		const std::string& join = " ") const;

	rb_tree();
	virtual ~rb_tree();
protected:
	node* minimum(node* root);
	void transplant(node* u, node* v);
	void remove_fixup(node* x);
	void insert_fixup(node* z);
	void destruct_tree(node* root);
};
