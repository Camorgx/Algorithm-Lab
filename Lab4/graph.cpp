#include "graph.hpp"

graph::graph(size_t vertex_cnt) : vertex_cnt(vertex_cnt) {
	edges.resize(vertex_cnt);
}

void graph::add_vertex() {
	++vertex_cnt;
	edges.emplace_back(nullptr);
}

void graph::add_edge(unsigned from, unsigned to, int weight) {
	edge* new_edge = new edge { to, weight };
	if (!edges[from]) edges[from] = new_edge;
	else {
		edge* e = edges[from];
		while (e->next) e = e->next;
		e->next = new_edge;
	}
	++edge_cnt;
}

graph::~graph() {
	for (edge* e : edges) {
		while (e) {
			edge* tmp = e->next;
			delete e;
			e = tmp;
		}
	}
}
