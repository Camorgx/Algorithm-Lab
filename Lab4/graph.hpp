#pragma once

#include <vector>

class graph {
public:
	struct edge {
		unsigned vertex = 0;
		int weight = 0;
		edge* next = nullptr;
	};

	std::vector<edge*> edges;
	size_t vertex_cnt = 0;
	size_t edge_cnt = 0;

	void add_vertex();
	void add_edge(unsigned from, unsigned to, int weight);

	graph() = default;
	graph(size_t vertex_cnt);
	~graph();
};
