#ifndef _DESTRUCTIVE_H_
#define _DESTRUCTIVE_H_

#include "edge.h"
#include "forest.h"
#include "RandomPoll.h"

class DestructiveHeuristic{
public:
    double skewFactor;

    DestructiveHeuristic(double sf){skewFactor = sf;}

    void destruct(Forest &forest, int numRemovals, std::vector<Edge> &availableEdges){
        while(numRemovals--){

            std::vector<double> costs;
            std::vector<edgeId> ids;
            for(auto &e : forest.edgeList){
                costs.push_back((double)(forest.costs[e] + availableEdges[e].linearCost));
                costs.back() = 1 / costs.back();
                ids.push_back(e);
            }


            RandomPoll rp(costs, skewFactor);
            rp.prepareProbs();
            int chosenIndex = rp.poll();
            forest.removeEdge(ids[chosenIndex], availableEdges);
        }
    }

};

#endif
