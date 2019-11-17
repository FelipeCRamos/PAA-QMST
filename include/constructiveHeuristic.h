#ifndef _CONSTRUCTIVE_H_
#define _CONSTRUCTIVE_H_

#include "edge.h"
#include "forest.h"
#include "unionFind.h"
#include "RandomPoll.h"

class ConstructiveHeuristic{
private:
    double skewFactor;

    void build(Forest &forest, UnionFindNRB &uf, std::vector<Edge> &availableEdges){
        for(auto &e : forest.edgeList){
            vertexId u = availableEdges[e].u;
            vertexId v = availableEdges[e].v;
            uf.join(u, v);
        }
    }

public:

    ConstructiveHeuristic(double sf){skewFactor = sf;}

    void construct(Forest &forest, std::vector<Edge> &availableEdges){
        UnionFindNRB uf(forest.N + 1);

        build(forest, uf, availableEdges); // union find building


        while(!forest.isSolution()){
            // if(forest.edgeList.empty()){
            //     int chosenIndex = rand() % forest.M;
            //     uf.join(availableEdges[chosenIndex].u, availableEdges[chosenIndex].v);
            //     forest.addEdge(chosenIndex, availableEdges);
            // }else{
            std::vector<double> costs(forest.M, 0);

            for(int i = 0; i < forest.M; ++i){
                if(!forest.edgeList.count(i)){
                    costs[i] = (double) (forest.costs[i] + availableEdges[i].linearCost);
                    costs[i] = 1 / costs[i];
                }
            }

            // printf("----------\n");
            // for(auto & c: costs) printf("%lf ", c);
            // printf("\n----------\n");

            RandomPoll rp(costs, skewFactor);
            rp.prepareProbs();
            while(true){
                int chosenIndex = rp.poll();
                // printf("%d\n", chosenIndex);
                int u = availableEdges[chosenIndex].u;
                int v = availableEdges[chosenIndex].v;
                if(uf.find(availableEdges[chosenIndex].u) != uf.find(availableEdges[chosenIndex].v)){
                    uf.join(availableEdges[chosenIndex].u, availableEdges[chosenIndex].v);
                    forest.addEdge(chosenIndex, availableEdges);
                    break;
                }
            }
            // }
        }
    }


};

#endif
