#include "int_tree.hpp"

#include <format>
#include <iostream>
#include <queue>

using namespace std;

// 输出红黑树的广度优先遍历
void display_tree(const rb_tree& tree) {
	auto p = tree.root;
	queue<rb_tree::node*> q;
	q.push(p);
	while (q.size()) {
		auto p = q.front(); q.pop();
		if (p == tree.nil) continue;
		if (p == p->parent->left)
			cout << "left ";
		else cout << "right ";
		cout << p->to_string() << endl;
		q.push(p->left);
		q.push(p->right);
	}
	cout << endl;
}

void test_int_tree() { // passed
	int_tree tree1;
	int_tree::interval test1[] = { { 2, 2 }, { 1, 1 }, { 4, 4 }, { 3, 3 } };
	for (const auto& intl : test1)
		tree1.insert(intl);
	display_tree(tree1);
	auto p0 = tree1.search_exact(test1[0]);
	tree1.remove(p0);
	display_tree(tree1);
	cout << endl;

	int_tree tree2;
	int_tree::interval test2[] = { { 2, 2 }, { 1, 1 }, { 3, 3 }, { 4, 4 } };
	for (const auto& intl : test2)
		tree2.insert(intl);
	display_tree(tree2);
	auto p4 = tree2.search_exact(test2[3]);
	tree2.remove(p4);
	display_tree(tree2);
}

void test_rb_tree() { // passed
	const int test_insert[] = { 41, 38, 31, 12, 19, 8 };
	const int test_remove[] = { 8, 12, 19, 31, 38, 41 };
	rb_tree tree;
	for (int test : test_insert) {
		cout << format("Inserting {0}\n", test);
		tree.insert(new rb_tree::node(test));
		cout << tree.to_string() << endl;
	}
	cout << "Insertion finished.\n" << endl;
	for (int test : test_remove) {
		auto res = tree.search(test);
		if (res != tree.nil) {
			cout << format("Find item {0}\n", test);
			tree.remove(res);
			cout << format("Item {0} removed\n", test);
			cout << tree.to_string() << endl;
		}
		else {
			cout << format("Item {0} not found!\n", test);
		}
	}
	cout << "Deletion finished.\n" << endl;
}
