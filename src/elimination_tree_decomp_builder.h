/*
#include <vector>
#include <functional>

#include "graph.h"
#include "addressable_priority_queue.h"
#include "treedecomp.h"
#include "edge_mutable_graph.h"

class EliminationTreeDecompBuilder
{
public:
    const Graph& G;
    std::vector<int> elimination_order;
    std::vector<int> position_in_elimination_order;
    //AddressablePriorityQueue APQ;

    EliminationTreeDecompBuilder(const Graph& G) : G(G)
    {
        elimination_order = std::vector<int>(G.N.size(),-1);
        position_in_elimination_order = std::vector<int>(G.N.size(),-1);
    }

    TreeDecomp f(const int seed)//std::function<float(EdgeMutableGraph&, const Graph&, int)> initial_key_fn;
    //std::function<float(EdgeMutableGraph&, const Graph&, int)> update_key_fn;);
    {
        EdgeMutableGraph EMG(G);
        AddressablePriorityQueue<float,greater<float>,plus<float>> APQ(G.N.size());

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
        
    }
};
*/