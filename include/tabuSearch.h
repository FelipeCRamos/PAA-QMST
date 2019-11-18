#ifndef _TABUSEARCH_H_
#define _TABUSEARCH_H_

#include "constructiveHeuristic.h"
#include "destructiveHeuristic.h"
#include "forest.h"
#include "SolutionsPoller.h"
#include "NeighborsPoller.h"
#include "pathRelink.h"

struct TabuSearchParameters{
    iterationType maxItrs, maxItrsNoImprove;
    int numTopSols, numTopNeighbors;
    int tabuTenureMin, tabuTenureMax;
    double skewFactor;
};

class TabuSearch{
private:
    std::random_device rd;
    std::mt19937 mt;
    std::uniform_int_distribution<> dist;

    //
    int N, M;
    std::vector<Edge> availableEdges;

    //
    double skewFactor;
    int numTopSols, numTopNeighbors;

    // tabu tenures logic
    iterationType tabuTenureMin, tabuTenureMax, tabuTenure;
    std::vector<iterationType> inLog, outLog;

    // algorithm limits
    iterationType maxItrs, maxItrsNoImprove;

    void prepSearch(){
        inLog = std::vector<iterationType>(M, 0); // init tabu in marker
        outLog = std::vector<iterationType>(M, 0); // init tabu out marker
        // tabu tenure value
        tabuTenure = (tabuTenureMin + tabuTenureMax) / 2;
    }

    bool isNeighborTabu(neighborType &neighbor, iterationType itr){
        return (itr <= tabuTenure + inLog[neighbor.second] || itr <= tabuTenure + outLog[neighbor.first]);
    }

    void updateTabu(neighborType neighborChosen, Forest &forest, iterationType iteration, int &itrsNoImprove, Forest &bestSolution){
        // printf("%d %\n", );
        if(forest.cost < bestSolution.cost && iteration > 0){
            // if we can decrease tabu tenure for in and out
            if(tabuTenure > tabuTenureMin) tabuTenure--;
        }else{
            // if we can increase tabu tenure for in and out
            if(tabuTenure < tabuTenureMax) tabuTenure++;
        }

        if(forest.cost < bestSolution.cost) itrsNoImprove = 0; // 0 iters without improving, just improved
        else itrsNoImprove++; // increasing the num of iters without improving


        // retrieving edges from the chosen neighbor
        edgeId removedEdge = neighborChosen.first;
        edgeId addedEdge = neighborChosen.second;
        inLog[addedEdge] = iteration;
        outLog[removedEdge] = iteration;
    }

    void maybeUpdtBest(Forest &forest, Forest &bestSolution){
        if(forest < bestSolution){
            bestSolution = forest;
        }
    }

    bool _findNeighbor(neighborType &nextNeighbor, Forest &forest, int itr){

        std::vector<neighborType> neighborhood;
        std::vector<costType> neighborhoodCosts;

        // best seen neighbor
        int bestNeighborIndex;

        // retrive neighbors
        forest.getNeighborhood(neighborhood, neighborhoodCosts, bestNeighborIndex, availableEdges);

        if(neighborhoodCosts[bestNeighborIndex] < forest.cost){ // troca de best neighbor eh melhor q melhor global
            nextNeighbor = neighborhood[bestNeighborIndex];
        }else{
            bool chosen = false;
            NeighborsPoller neighborsPoller(numTopNeighbors, skewFactor);
            for(int i = 0; i < neighborhood.size(); ++i){
                if(!isNeighborTabu(neighborhood[i], itr)){
                    chosen = true;
                    neighborsPoller.add(neighborhood[i], neighborhoodCosts[i]);
                }
            }
            if(!chosen) return false;
            nextNeighbor = neighborsPoller.get();
        }
        return true;
    }

public:

    TabuSearch(int n, int m, TabuSearchParameters tsp, std::vector<Edge> &allEdges){
        N = n;
        M = m;
        maxItrs = tsp.maxItrs;
        maxItrsNoImprove = tsp.maxItrsNoImprove;
        skewFactor = tsp.skewFactor;
        numTopSols = tsp.numTopSols;
        numTopNeighbors = tsp.numTopNeighbors;
        tabuTenureMin = tsp.tabuTenureMin;
        tabuTenureMax = tsp.tabuTenureMax;
        availableEdges = allEdges;

        mt = std::mt19937(rd());
        dist = std::uniform_int_distribution<>(1, M);
    }


    Forest run(){
        // heuristics
        ConstructiveHeuristic constructiveHeuristic(skewFactor);
        DestructiveHeuristic destructiveHeuristic(skewFactor);

        // pollers
        SolutionsPoller solutionsPoller(numTopSols, skewFactor);

        // tree that is going to be used for searching
        Forest forest(N, M, availableEdges);

        constructiveHeuristic.construct(forest, availableEdges);

        Forest bestSolution = forest; // best seen solution

        for(iterationType itr = 0; itr < maxItrs; ++itr){
            prepSearch(); // init tabu search parameters
            int itrsNoImprove = 0; // started counting num of iters without improving


            while(itrsNoImprove < maxItrsNoImprove && itr < maxItrs){
                // std::cout << forest << std::endl;
                solutionsPoller.addSolution(forest); // add forest to poller

                neighborType nextNeighbor;
                _findNeighbor(nextNeighbor, forest, itr);

                forest.goToNeighbor(nextNeighbor, availableEdges); // change solution
                updateTabu(nextNeighbor, forest, itr, itrsNoImprove, bestSolution); // updt tabu parameters
                maybeUpdtBest(forest, bestSolution); // maybe update best seen solution
                itr++;
            }

            // leave if reached max of iters
            if(itr == maxItrs) break;

            // strategic oscillation:
            // get an already visited solution from poller
            forest = solutionsPoller.getSolution();
            // choose (uniformly) how many edges will be removed
            int remEdges = (((int)dist(mt)) % (N-1)) + 1;

            // desctruct than construct
            // printf("=================== STRATEGIC OSCILLATION (%d)===================\n", remEdges);
            destructiveHeuristic.destruct(forest, remEdges, availableEdges);
            constructiveHeuristic.construct(forest, availableEdges);
        }
        return bestSolution;
    }

    Forest runLocal(Forest &forest, Forest &globalBest){
        prepSearch();
        Forest bestSolution = forest; // best seen solution

        int itr = 0, itrsNoImprove = 0;
        while(itrsNoImprove < maxItrsNoImprove && itr < maxItrs){
            neighborType nextNeighbor;
            if(!_findNeighbor(nextNeighbor, forest, itr)) break;
            // printf("%d\n", lastCost);
            forest.goToNeighbor(nextNeighbor, availableEdges); // change solution
            updateTabu(nextNeighbor, forest, itr, itrsNoImprove, globalBest); // updt tabu parameters
            maybeUpdtBest(forest, bestSolution); // maybe update best seen solution
            itr++;
        }
        return bestSolution;
    }


};

#endif
