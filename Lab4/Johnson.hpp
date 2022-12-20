#pragma once

#include "fib_heap.hpp" 
#include "graph.hpp"

#include <tuple>

extern void bellman_ford(graph& g, int s);

// must be called after bellman_ford
// return (0, nullptr) if there is no negative circle
// return (u, e) if e(u, v) is an edge in a negative circle
extern std::tuple<unsigned, graph::edge*> detect_negative_circle(const graph& g);

extern void break_negative_circle(graph& g);

struct ver_pair {
	int v_d = 0;
	int vertex = 0;
	auto operator<=>(const ver_pair& ano) const {
		return v_d <=> ano.v_d;
	}
	bool operator==(const ver_pair& ano) const {
		return v_d == ano.v_d;
	}
};

extern void Dijkstra(graph& g, int s, std::vector<fib_heap<ver_pair>::node*>& nodes);

// allocate space for array that matches vertexes and fib_heap nodes
// must be called before all the Dijkstras
// you should free these space latter
extern std::vector<fib_heap<ver_pair>::node*> prepare_for_dij(const graph& g);

using ans_matrix = std::vector<std::vector<int>>;

// suppose there is no negative circle
extern void Johnson(graph& g, ans_matrix& d, ans_matrix& pi);
