#pragma once

#include <limits>
#include <vector>

class graph {
public:
	struct edge {
		unsigned vertex = 0;
		int weight = 0;
		edge* next = nullptr;
	};

	struct vertex {
		int d = std::numeric_limits<int>::max() >> 1;
		int pi = -1;
	};

	std::vector<vertex> vertexes;
	std::vector<edge*> edges;
	size_t vertex_cnt = 0;
	size_t edge_cnt = 0;

	void add_vertex();
	void add_edge(unsigned from, unsigned to, int weight);
	void init_single_source(unsigned s);
	void relax(unsigned u, unsigned v, int w);
		
	graph() = default;
	graph(size_t vertex_cnt);
	~graph();
};
