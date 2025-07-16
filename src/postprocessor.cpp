#include "postprocessor.h"

#include <vector>
#include <algorithm>
#include <iostream>

#include "rootedtree.h"
#include "binaryinteger.h"

using namespace std;

PostProcessor::PostProcessor(vector<vector<int>>& bags, const Graph& G)
    : bags(bags), G(G)
{

}

void PostProcessor::setup(RootedTree& RT)
{
    c = vector<BinaryInteger>(RT.N.size(),BinaryInteger());//vector<__uint128_t>(RT.N.size(),0);
    keep = vector<int>(RT.N.size(),0);
}

void PostProcessor::discover(const int current, RootedTree& RT)
{
    int parent = RT.parents[current-1];
    auto it = lower_bound(bags[current-1].begin(),bags[current-1].end(), RT.root);
    if(it != bags[current-1].end() && *it == RT.root)               
    {
        //cout << current << " contains " << RT.root << endl;
        int j = 0;
        for(int i = 0; i<G.N[RT.root-1].size(); i++)
        {
            while(j < bags[current-1].size() && bags[current-1][j] < G.N[RT.root-1][i])
                j++;
            if(j < bags[current-1].size() && bags[current-1][j] == G.N[RT.root-1][i])
                c[current-1].set_bit(i);//c[current-1] |= static_cast<__uint128_t>(1) << i;
        } 
    }
    else
    {
        //cout << current << " did not contain " << RT.root << endl;
        RT.neighbourIterators[current-1] = RT.N[current-1].end();
    }
}

void PostProcessor::finish(const int current, RootedTree& RT)
{
    int parent = RT.parents[current-1];
    auto it = lower_bound(bags[current-1].begin(),bags[current-1].end(), RT.root);//
    if(it != bags[current-1].end() && *it == RT.root)//(c[current-1] != 0)      //FIXED
    {
        if(keep[current-1] == 0 && current != RT.root && ((c[parent-1] | c[current-1]) == c[parent-1]))//c[current-1].is_subset_of(c[parent-1]))//
        {
            //cout << "Can remove " << RT.root << " from bag " << current << endl;
            //auto it = lower_bound(bags[current-1].begin(), bags[current-1].end(), RT.root);
            //if(it != bags[current-1].end() && *it == RT.root)
            bags[current-1].erase(it);
        }
        else
        {
            keep[current-1] = 1;
            keep[parent-1] = 1;
        }
    }
}

void PostProcessor::cleanup(RootedTree& RT)
{
    int current = RT.root;
    bool keep_reducing = true;
    while(keep_reducing)
    {
        int relevant_child = 0;
        int number_of_relevant_children = 0;
        for(int i = 0; i<RT.N[current-1].size(); i++)
        {
            if(keep[RT.N[current-1][i]-1] && RT.parents[current-1] != RT.N[current-1][i])
            {
                relevant_child = RT.N[current-1][i];
                number_of_relevant_children++;
            }
        }
        if(number_of_relevant_children == 1)
        {
            if((c[current-1] | c[relevant_child-1]) == c[relevant_child-1])//(c[current-1].is_subset_of(c[relevant_child-1]))
            {
                auto it = lower_bound(bags[current-1].begin(), bags[current-1].end(), RT.root);
                if(it != bags[current-1].end() && *it == RT.root)
                    bags[current-1].erase(it);
            }
            else
                keep_reducing = false;
        }
        else
            keep_reducing = false;
        current = relevant_child;
    }
}
