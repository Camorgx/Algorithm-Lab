#pragma once

#include "graph.hpp"

#include <tuple>

extern void bellman_ford(graph& g, int s);

// must be called after bellman_ford
// return (0, nullptr) if there is no negative circle
// return (u, e) if e(u, v) is an edge in a negative circle
extern std::tuple<unsigned, graph::edge*> detect_negative_circle(const graph& g);

extern void break_negative_circle(graph& g);
