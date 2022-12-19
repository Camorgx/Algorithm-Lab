#include "Johnson.hpp"

void bellman_ford(graph& g, int s) {
	g.init_single_source(s);
	for (int i = 1; i < g.vertex_cnt; ++i)
		for (int v = 0; v < g.vertex_cnt; ++v)
			for (auto e = g.edges[v]; e; e = e->next)
				g.relax(v, e->vertex, e->weight);
}

std::tuple<unsigned, graph::edge*> detect_negative_circle(const graph& g) {
	for (int u = 0; u < g.vertex_cnt; ++u)
		for (auto e = g.edges[u]; e; e = e->next) {
			if (g.vertexes[e->vertex].d > g.vertexes[u].d + e->weight)
				return std::tuple(u, e);
		}
	return std::tuple(0, nullptr);
}

void break_negative_circle(graph& g) {
	while (true) {
		bellman_ford(g, 0);
		auto [u, e] = detect_negative_circle(g);
		if (e) {
			if (g.edges[u] == e) g.edges[u] = e->next;
			else {
				auto p = g.edges[u];
				while (p->next != e) p = p->next;
				p->next = e->next;
			}
			--g.edge_cnt;
			delete e;
		}
		else break;
	}
}
