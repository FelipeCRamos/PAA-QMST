#ifndef _DESTRUCTIVE_H_
#define _DESTRUCTIVE_H_

#include "edge.h"
#include "forest.h"

class DestructiveHeuristic{

    DestructiveHeuristic(){}

    void desctruct(Forest &forest, int numRemovals){
        while(numRemovals--){
            vector<costType> costs = forest.costs;
            for(auto &e : edgeList) costs[e] = 0;
            for(auto &cost : costs){
                if(cost != 0) cost = 1 / cost;
            }

            RandomPoll rp(cost);
            rp.prepareProbs();
            int chosenIndex = rp.poll();
            forest.removeEdge(costs[chosenIndex]);
        }
    }

};

#endif
