#include "edge_mutable_graph.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>

#include "graph.h"
#include "test.h"

using namespace std;

EdgeMutableGraph::EdgeMutableGraph(const Graph& G) : N(G.N)
{

}

void EdgeMutableGraph::eliminate_vertex(const int current)
{
    for(int j = 0; j < N[current-1].size(); j++)
    {
        int neighbour = N[current-1][j];
        vector<int>::const_iterator it_current = N[current-1].begin();
        vector<int>::const_iterator it_neighbour = N[N[current-1][j]-1].begin();

        // New neighbourhood of neighbour of current after fill
        vector<int> v(0);

        while(it_current != N[current-1].end() && it_neighbour != N[N[current-1][j]-1].end())
        {
            if(*it_current == neighbour)
            {
                it_current++;
            }
            else
            {
                if(*it_neighbour == current)
                {
                    it_neighbour++;
                }
                else
                {
                    if(*it_current < *it_neighbour)
                    {
                        v.push_back(*it_current);
                        it_current++;
                    }
                    else
                    {
                        if(*it_current == *it_neighbour)
                        {
                            v.push_back(*it_current);
                            it_current++;
                            it_neighbour++;
                        }
                        else
                        {
                            v.push_back(*it_neighbour);
                            it_neighbour++;
                        }
                    }
                }
            }
        }
        while(it_current != N[current-1].end())
        {
            if(*it_current != neighbour)
                v.push_back(*it_current);
            it_current++;
        }
        while(it_neighbour != N[N[current-1][j]-1].end())
        {
            if(*it_neighbour != current)
                v.push_back(*it_neighbour);
            it_neighbour++;
        }
        //assert(is_sorted(v.begin(), v.end()));
        //assert(is_unique(v));
        N[N[current-1][j]-1] = move(v);
    }
}

void EdgeMutableGraph::print() const
{
    cout << "Edge Mutable Graph:" << endl;
    for(vector<vector<int>>::const_iterator vertex_it = N.begin(); vertex_it != N.end(); vertex_it++)
    {
        cout << "N(" << vertex_it-N.begin()+1 << ") = {";
        for(vector<int>::const_iterator adj_it = vertex_it->begin(); adj_it != vertex_it->end(); adj_it++)
        {
            cout << *adj_it << " ";
        }
        cout << "}" << endl;
    }
}
