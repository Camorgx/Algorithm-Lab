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

#include "fib_heap.hpp"

void prepare_for_dij(const graph& g, std::vector<fib_heap<ver_pair>::node*>& nodes) {
	nodes.resize(g.vertex_cnt);
	for (int i = 0; i < g.vertex_cnt; ++i) {
		nodes[i] = new fib_heap<ver_pair>::node;
		nodes[i]->val.vertex = i;
	}
}

void Dijkstra(graph& g, int s, std::vector<fib_heap<ver_pair>::node*>& nodes) {
	g.init_single_source(s);
	fib_heap<ver_pair> heap_queue;
	for (int i = 0; i < g.vertex_cnt; ++i) {
		nodes[i]->val.v_d = (i == s) ? 0 : std::numeric_limits<int>::max() >> 1;
		heap_queue.insert(nodes[i]);
	}
	while (heap_queue.count()) {
		auto u = heap_queue.extract_min();
		int u_ver = u->val.vertex;
		for (auto v = g.edges[u_ver]; v; v = v->next) {
			int v_ver = v->vertex;
			int new_d = g.vertexes[u_ver].d + v->weight;
			if (g.vertexes[v_ver].d > new_d) {
				g.vertexes[v_ver].d = new_d;
				g.vertexes[v_ver].pi = u_ver;	
				heap_queue.decrease_key(nodes[v_ver], ver_pair { new_d, v_ver });
			}
		}
	}
}
