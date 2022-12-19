#include "graph.hpp"

graph::graph(size_t vertex_cnt) 
	: vertex_cnt(vertex_cnt) ,
	edges(vertex_cnt),
	vertexes(vertex_cnt)
{ }

void graph::add_vertex() {
	++vertex_cnt;
	edges.emplace_back(nullptr);
	vertexes.emplace_back();
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

void graph::init_single_source(unsigned s) {
	for (auto& v : vertexes) {
		v.d = std::numeric_limits<int>::max() >> 1;
		v.pi = -1;
	}
	vertexes[s].d = 0;
}

void graph::relax(unsigned u, unsigned v, int w) {
	if (vertexes[v].d > vertexes[u].d + w) {
		vertexes[v].d = vertexes[u].d + w;
		vertexes[v].pi = u;
	}
}
