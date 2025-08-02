#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <string>
#include <cassert>
#include <utility>
#include <numeric> 

#include "../src/graph.h"
#include "../src/edge_mutable_graph.h"
#include "../src/addressable_priority_queue.h"
#include "../src/rootedtree.h"
#include "../src/treedecomp.h"
#include "../src/postprocessor.h"
#include "../src/binaryinteger.h"
#include "../src/general.h"
#include "../src/cli.h"

using namespace std;

template<typename T>
T key_value(EdgeMutableGraph& EMG, const Graph& G, int current);

template<typename T>
T initial_key_value(EdgeMutableGraph& EMG, const Graph& G, int current);

int main(int argc, char* argv[])
{
    // Input
    string filename_graph;
    bool print_td = false;
    int seed = 0;

    // CLI dealings
    vector<string> cliArguments;
    for(int i = 1; i<argc; i++)
        cliArguments.push_back(argv[i]);

    int graphStatus = check_for_graph_file(cliArguments,filename_graph);
    if(graphStatus==1)
        return graphStatus;

    int seedStatus = check_for_seed(cliArguments, seed);
    if(seedStatus == 1)
        return seedStatus;

    //Check for options --print_td_filecontents=true
    int optionStatus = check_for_options(cliArguments,print_td);
    if(optionStatus==1)
        return optionStatus;

    ///////////////////////////////////////////////////////////////////////////
    Graph G = Graph::from_file(filename_graph);

    vector<int> position_in_elimination_order(G.N.size(),-1);
    vector<int> elimination_order(G.N.size(),-1);


    EdgeMutableGraph EMG(G);                                                                    // Copy of G whose edges we can mutate

    //AddressablePriorityQueue<int, greater<int>,plus<int>> APQ(G.N.size());                        //MCS
    //AddressablePriorityQueue<int,less<int>,minus<int>> APQ(G.N.size());                           //MinDeg
    AddressablePriorityQueue<float,greater<float>,plus<float>> APQ(G.N.size());                    //MaxDens

    for(int i = 0; i< G.N.size(); i++)
    {
        if(i+1 == seed)
        {
            //APQ.insertElement(i+1,1);                                                             //MCS
            //APQ.insertElement(i+1,0);                                                             //MinDeg
            APQ.insertElement(i+1,2);                                                               //MaxDens
        }
        else
        {
            //APQ.insertElement(i+1,0);                                                             //MCS
            //APQ.insertElement(i+1,G.N[i].size());                                                 //MinDeg
            APQ.insertElement(i+1,initial_key_value<float>(EMG,G,i+1));                                     //MaxDens
        }
    }

    for(int i = 0; i<G.N.size(); i++)
    {
        int current = APQ.deleteRoot();
        //cout << APQ.v[current-1].second << endl;
        elimination_order[i] = current;
        position_in_elimination_order[current-1] = i;
        EMG.eliminate_vertex(current);
        for(int j = 0; j < EMG.N[current-1].size(); j++)
        {
            //APQ.updateKey(EMG.N[current-1][j],1);                                                         //MCS
            //APQ.updateKey(EMG.N[current-1][j],
                //APQ.v[APQ.p[EMG.N[current-1][j]-1]].second - EMG.N[EMG.N[current-1][j]-1].size());        //MinDeg
            if(!(APQ.v[APQ.p[EMG.N[current-1][j]-1]].second > 1))
                APQ.updateKey(EMG.N[current-1][j],
                    key_value<float>(EMG,G,EMG.N[current-1][j]) - APQ.v[APQ.p[EMG.N[current-1][j]-1]].second);  //MaxDens
        }
    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    vector<pair<int,int>> edges;
    for(int i = 0; i < G.N.size(); i++)
    {
        int least_order = G.N.size();
        int neighbour = G.N.size()+1;
        for(int j = 0; j< EMG.N[i].size(); j++)
        {
            if(position_in_elimination_order[EMG.N[i][j]-1]<least_order)
            {
                least_order = position_in_elimination_order[EMG.N[i][j]-1];
                neighbour = EMG.N[i][j];
            }
        }
        if(neighbour!=G.N.size()+1)
        {   
            if(i+1 < neighbour)
                edges.push_back({i+1,neighbour});
            else
                edges.push_back({neighbour,i+1});
        }
        else
        {
            if(i+1!=elimination_order.back())
            {
                neighbour = elimination_order.back();
                if(i+1 < neighbour)
                    edges.push_back({i+1,neighbour});
                else
                    edges.push_back({neighbour,i+1});
            }
        }
    }

    for(int i = 0; i<G.N.size(); i++)
    {
        auto it = lower_bound(EMG.N[i].begin(),EMG.N[i].end(),i+1);
        EMG.N[i].insert(it, i+1);
        //EMG.N[i].push_back(i+1);
        //sort(EMG.N[i].begin(),EMG.N[i].end());
    }
    sort(edges.begin(),edges.end());
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    vector<vector<int>> ADJ(G.N.size());
    for(size_t i = 0; i < edges.size(); i++)
    {
        ADJ[edges[i].first-1].push_back(edges[i].second);
        ADJ[edges[i].second-1].push_back(edges[i].first);
    }

    ///////////////////////////////// TreeDecomposition(move(),move()) ///////////////////////////////////////////////////////////////////

    RootedTree RT(move(ADJ),1);

    PostProcessor P(EMG.N,G);

    for(int i = 0; i<G.N.size(); i++)
    {
        RT.reroot(i+1);
        P.vertex_to_trim = i + 1;
        BinaryInteger::set_number_of_blocks(G.N[i+1 - 1].size());
        RT.df_traversal(
            bind(&PostProcessor::setup, &P, placeholders::_1),
            bind(&PostProcessor::discover, &P, placeholders::_1, placeholders::_2),
            bind(&PostProcessor::finish, &P, placeholders::_1, placeholders::_2),  //[](const int, RootedTree&){} -- lambda dummy
            bind(&PostProcessor::cleanup, &P, placeholders::_1)//[](RootedTree&){}////
        );
    }
       
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    size_t max_bag_size = 0;
    for(size_t i = 0; i < G.N.size(); i++)
        max_bag_size = max(max_bag_size,P.bags[i].size());

    ////////////////////////////////////////////////    Print .td format    //////////////////////////////////////////////////////////////////////////////////////
    cout << 's' << ' ' << "td" << ' ' << P.bags.size() << ' ' << max_bag_size << ' ' << P.bags.size() << endl;
    if(print_td == true)
    {
        for(size_t i = 0; i < G.N.size(); i++)
        {
            cout << 'b' << ' ' << i+1 << ' ';
            print_vector(P.bags[i]);
        }
        for(size_t i = 0;i<edges.size(); i++)
            cout << edges[i].first << " " << edges[i].second << endl;
    }
    return 0;
}

//   ADD current to the bag
template<typename T>
T key_value(EdgeMutableGraph& EMG, const Graph& G, int current)
{
    vector<int> v;
    v.reserve(EMG.N[current-1].size()+1);
    int i = 0;
    for(; i< EMG.N[current-1].size() && EMG.N[current-1][i]<current; i++)
        v.push_back(EMG.N[current-1][i]);
    v.push_back(current);
    for(;i<EMG.N[current-1].size();i++)
        v.push_back(EMG.N[current-1][i]);
    T numerator = 0;
    for(int i = 0; i< v.size(); i++)
    {
        int k = 0;
        for(int j = i+1; j<v.size(); j++)
        {
            //if(G.adjacent(v[i],v[j]))
            //{
                //adjacency_matrix[i][j] = 1;
            //    numerator++;
            //}
            while(k<G.N[v[i]-1].size() && G.N[v[i]-1][k] < v[j])
                k++;
            if(k<G.N[v[i]-1].size() && G.N[v[i]-1][k] == v[j])
                numerator+=2;

        }
    }
    T denominator = v.size()*v.size(); //(v.size()-1);
    if(v.size()-1 == 0 || numerator == v.size()*(v.size()-1))
    {
        return T(1);
    }
    else
        return numerator/denominator;
}

template<typename T>
T initial_key_value(EdgeMutableGraph& EMG, const Graph& G, int current)
{
    vector<int> v;
    v.reserve(EMG.N[current-1].size()+1);
    int i = 0;
    for(; i< EMG.N[current-1].size() && EMG.N[current-1][i]<current; i++)
        v.push_back(EMG.N[current-1][i]);
    v.push_back(current);
    for(;i<EMG.N[current-1].size();i++)
        v.push_back(EMG.N[current-1][i]);
    T numerator = 0;
    for(int i = 0; i< v.size(); i++)
    {
        int k = 0;
        for(int j = i+1; j<v.size(); j++)
        {
            //if(G.adjacent(v[i],v[j]))
            //{
                //adjacency_matrix[i][j] = 1;
            //    numerator++;
            //}
            while(k<G.N[v[i]-1].size() && G.N[v[i]-1][k] < v[j])
                k++;
            if(k<G.N[v[i]-1].size() && G.N[v[i]-1][k] == v[j])
                numerator+=2;

        }
    }
    T denominator = v.size()*v.size(); //v.size()*(v.size()-1);
    if(v.size()-1 == 0 || numerator == v.size()*(v.size()-1))
    {
        return T(2);
    }
    else
        return numerator/denominator;
}