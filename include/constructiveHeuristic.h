#ifndef _CONSTRUCTIVE_H_
#define _CONSTRUCTIVE_H_

#include "edge.h"
#include "forest.h"
#include "unionFind.h"

class ConstructiveHeuristic{
private:
    void build(Forest &forest, UnionFind &uf){
        for(auto &e : forest.edgeList){
            vertexId u = forest.avaibleEdges[e].u;
            vertexId v = forest.avaibleEdges[e].v;
            uf.join(u, v);
        }
    }

public:

    ConstructiveHeuristic(){}

    void construct(Forest &forest){
        UnionFind uf;

        build(forest, uf); // union find building

        while(!forest.isSolution()){
            vector<costType> costs = forest.costs;
            for(auto &e : edgeList) costs[e] = 0;
            for(auto &cost : costs){
                if(cost != 0) cost = 1 / cost;
            }

            RandomPoll rp(cost);
            rp.prepareProbs();
            int chosenIndex = rp.poll();
            forest.addEdge(costs[chosenIndex]);
        }
    }


};

#endif
