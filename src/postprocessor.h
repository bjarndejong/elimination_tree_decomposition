#ifndef POSTPROCESSOR_H_INCLUDED
#define POSTPROCESSOR_H_INCLUDED

#include <vector>
#include <bitset>

#include "graph.h"
#include "rootedtree.h"
#include "binaryinteger.h"

class PostProcessor
{
public:
    int vertex_to_trim;

    std::vector<BinaryInteger> c;//std::vector<__uint128_t> c;
    std::vector<int> keep;
    std::vector<std::vector<int>>& bags;
    const Graph& G;

    PostProcessor(std::vector<std::vector<int>>& bags, const Graph& G);

    void setup(RootedTree& RT);
    void discover(const int current, RootedTree& RT);
    void finish(const int current, RootedTree& RT);
    void cleanup(RootedTree& RT);

};

#endif // POSTPROCESSOR_H_INCLUDED