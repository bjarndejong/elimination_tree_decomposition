#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

//INCLUDE STD
#include <vector>
#include <string>
#include <functional>

class Graph
{
public:
    const std::vector<std::vector<int>> N;          //N[vertex-1]: is the Neighborhood of vertex
    //std::vector<std::vector<int>::const_iterator> neighbourIterators;
    const std::vector<int> weights;

    static Graph from_file(const std::string& ifname);

    //Graph(std::string ifname);
    void print() const;

    //void get_MCS_ordering(const int start, std::vector<int>& ordering);

    void dfs(
        std::function<void(const Graph&)> setup,
        std::function<void(const int&, const Graph&)> discover,
        std::function<void(const int&, const Graph&)> finish,
        std::function<void(const Graph&)> cleanup
    );
    void bfs(const int start) const;        //Doesn't exist

    bool adjacent(int u, int v) const;
    bool independent_set(const std::vector<int>& subset) const;
    int weight_set(const std::vector<int>& subset) const;
    int get_vertex_of_max_degree() const;

private:
    Graph(std::vector<std::vector<int>>&& N, std::vector<int>&& weights);
};

#endif // GRAPH_H_INCLUDED
