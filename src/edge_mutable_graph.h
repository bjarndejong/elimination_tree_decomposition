#ifndef EDGE_MUTABLE_GRAPH_H_INCLUDED
#define EDGE_MUTABLE_GRAPH_H_INCLUDED

#include <vector>

#include "graph.h"
#include "rootedtree.h"
#include "treedecomp.h"

class EdgeMutableGraph
{
public:
    std::vector<std::vector<int>> N;

    EdgeMutableGraph(const Graph& G);
    void eliminate_vertex(const int current);
    void print() const;
};


#endif // EDGE_MUTABLE_GRAPH_H_INCLUDED
