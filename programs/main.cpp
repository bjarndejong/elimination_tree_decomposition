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

using namespace std;

template<typename T>
T key_value(EdgeMutableGraph& EMG, const Graph& G, int current);

template<typename T>
T initial_key_value(EdgeMutableGraph& EMG, const Graph& G, int current);

int main(int argc, char* argv[])
{
    // Input
    string filename_graph;
    bool print_treedecomposition = false;
    filename_graph = argv[1];
    int seed = stoi(argv[2]);

    Graph G = Graph::from_file(filename_graph);

    size_t max_degree = 0;
    for(int i = 0; i<G.N.size(); i++)
    {
        max_degree = max(max_degree,G.N[i].size());
    }
    BinaryInteger::set_number_of_blocks(max_degree);

    EdgeMutableGraph EMG(G);

    vector<int> position_in_elimination_order(G.N.size(),-1);
    vector<int> elimination_order(G.N.size(),-1);
    vector<pair<int,int>> edges;

 

    //AddressablePriorityQueue<int, greater<int>,plus<int>> APQ(G.N.size());                        //MCS
    //AddressablePriorityQueue<int,less<int>,minus<int>> APQ(G.N.size());                           //MinDeg
    AddressablePriorityQueue<float, greater<float>,plus<float>> APQ(G.N.size());                    //MaxDens

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
        EMG.N[i].push_back(i+1);
        sort(EMG.N[i].begin(),EMG.N[i].end());
    }
    sort(edges.begin(),edges.end());
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    size_t max_bag_size = 0;
    for(int i = 0; i < G.N.size(); i++)
        max_bag_size = max(max_bag_size,EMG.N[i].size());

    ////////////////////////////////////////////////    Print .td format    //////////////////////////////////////////////////////////////////////////////////////
    /*
    cout << 's' << ' ' << "td" << ' ' << EMG.N.size() << ' ' << max_bag_size << ' ' << EMG.N.size() << endl;
    for(int i = 0; i<G.N.size(); i++)
    {
        cout << 'b' << ' ' << i+1 << ' ';
        print_vector(EMG.N[i]);
    }
    for(int i = 0;i<edges.size();i++)
        cout << edges[i].first << " " << edges[i].second << endl;
    */
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    vector<vector<int>> ADJ(G.N.size());
    for(int i = 0; i < edges.size(); i++)
    {
        ADJ[edges[i].first-1].push_back(edges[i].second);
        ADJ[edges[i].second-1].push_back(edges[i].first);
    }
    RootedTree RT(move(ADJ),1);

    PostProcessor P(move(EMG.N),G);

    for(int i = 0; i<G.N.size(); i++)
    {
        RT.reroot(i+1);
        RT.df_traversal(
            bind(&PostProcessor::setup, &P, placeholders::_1),
            bind(&PostProcessor::discover, &P, placeholders::_1, placeholders::_2),
            bind(&PostProcessor::finish, &P, placeholders::_1, placeholders::_2),  //[](const int, RootedTree&){} -- lambda dummy
            bind(&PostProcessor::cleanup, &P, placeholders::_1)//[](RootedTree&){}////
        );
    }
       
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    max_bag_size = 0;
    for(int i = 0; i < G.N.size(); i++)
        max_bag_size = max(max_bag_size,P.bags[i].size());

    ////////////////////////////////////////////////    Print .td format    //////////////////////////////////////////////////////////////////////////////////////
    cout << 's' << ' ' << "td" << ' ' << P.bags.size() << ' ' << max_bag_size << ' ' << P.bags.size() << endl;
    //for(int i = 0; i < G.N.size(); i++)
    //{
    //    cout << 'b' << ' ' << i+1 << ' ';
    //    print_vector(P.bags[i]);
    //}
    //for(int i = 0;i<edges.size();i++)
    //    cout << edges[i].first << " " << edges[i].second << endl;

    return 0;
}

//  NEEDS TO ADD current to the bag
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
    //vector<vector<int>> adjacency_matrix(v.size(),vector<int>(v.size(),0));
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
    /*
    //Set up validcandidates for current bag
    vector<T> validcandidates(1,0);
    for(int i = 0; i<S.bags[current-1].size(); i++)
    {
        int vSize = validcandidates.size();
        validcandidates.push_back(T(1)<<i);
        for(int index = 1; index<vSize; index++)
        {
            //add validcandidates[index]+(T(1)<<i) if it is an independent set
            int j = countr_zero(validcandidates[index]);

            if(
                binary_search(validcandidates.begin(),    //CONSIDER DIFFERENT START AND END
                    validcandidates.end(),
                    validcandidates[index] + (T(1)<<i) - (T(1)<<j)) == true
                    &&
                    adjacency_matrix[i][j]==0
            )
            {
                validcandidates.push_back(validcandidates[index]+(T(1)<<i));
                //cout << current << ": " << validcandidates.size() << endl;
            }
        }
    }
    
    validcandidates.size();
    */
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
    //vector<vector<int>> adjacency_matrix(v.size(),vector<int>(v.size(),0));
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
        return T(2);
    }
    else
        return numerator/denominator;
    /*
    //Set up validcandidates for current bag
    vector<T> validcandidates(1,0);
    for(int i = 0; i<S.bags[current-1].size(); i++)
    {
        int vSize = validcandidates.size();
        validcandidates.push_back(T(1)<<i);
        for(int index = 1; index<vSize; index++)
        {
            //add validcandidates[index]+(T(1)<<i) if it is an independent set
            int j = countr_zero(validcandidates[index]);

            if(
                binary_search(validcandidates.begin(),    //CONSIDER DIFFERENT START AND END
                    validcandidates.end(),
                    validcandidates[index] + (T(1)<<i) - (T(1)<<j)) == true
                    &&
                    adjacency_matrix[i][j]==0
            )
            {
                validcandidates.push_back(validcandidates[index]+(T(1)<<i));
                //cout << current << ": " << validcandidates.size() << endl;
            }
        }
    }
    
    validcandidates.size();
    */
}