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

std::vector<fib_heap<ver_pair>::node*> prepare_for_dij(const graph& g) {
	std::vector<fib_heap<ver_pair>::node*> nodes;
	nodes.resize(g.vertex_cnt);
	for (int i = 0; i < g.vertex_cnt; ++i) {
		nodes[i] = new fib_heap<ver_pair>::node;
		nodes[i]->val.vertex = i;
	}
	return nodes;
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

void Johnson(graph& g, ans_matrix& d, ans_matrix& pi) {
	int new_ver = static_cast<int>(g.vertex_cnt);
	g.add_vertex();
	for (int i = 0; i < new_ver; ++i)
		g.add_edge(new_ver, i, 0);
	bellman_ford(g, new_ver);
	std::vector<int> h(new_ver + 1);
	for (int i = 0; i <= new_ver; ++i)
		h[i] = g.vertexes[i].d;
	for (int i = 0; i <= new_ver; ++i) {
		for (auto e = g.edges[i]; e; e = e->next)
			e->weight += h[i] - h[e->vertex];
	}
	auto nodes = prepare_for_dij(g);
	for (int u = 0; u < new_ver; ++u) { // for each vertex
		Dijkstra(g, u, nodes);
		for (int v = 0; v < new_ver; ++v) {
			d[u][v] = g.vertexes[v].d + h[v] - h[u];
			pi[u][v] = g.vertexes[v].pi;
		}
	}
	for (auto node : nodes) delete node;
}
