#ifndef _PATH_RELINKING_H_
#define _PATH_RELINKING_H_


#include "forest.h"

struct PathRelinkingParameters{
    double skewFactor;
    int numTopSols;
};

class PathRelinking{
private:
    int numTopSols;
    double skewFactor;

    void getMissingEdges(Forest &f1, Forest &f2, std::set<edgeId> &missingEdges){
        for(auto e : f2.edgeList){
            if(!f1.edgeList.count(e))
                missingEdges.insert(e);
        }
    }

public:
    PathRelinking(PathRelinkingParameters pathRelinkingParameters){
        numTopSols = pathRelinkingParameters.numTopSols;
        skewFactor = pathRelinkingParameters.skewFactor;
    }

    Forest run(Forest &f1, Forest &f2, costType best, std::vector<Edge> &availableEdges){
        SolutionsPoller solutionsPoller(numTopSols, skewFactor);

        std::set<edgeId> missingEdges;
        getMissingEdges(f1, f2, missingEdges);

        while(!missingEdges.empty()){
            // neighbors vector
            std::vector<neighborType> neighborhood;
            // neighbors costs
            std::vector<costType> neighborhoodCosts;

            // best seen neighbor and neighbor going to be chosen
            neighborType nextNeighbor;
            int bestNeighborIndex;

            f1.getRestrictedNeighborhood(neighborhood, neighborhoodCosts, bestNeighborIndex, availableEdges, missingEdges);
            nextNeighbor = neighborhood[bestNeighborIndex];

            missingEdges.erase(nextNeighbor.second);

            f1.goToNeighbor(nextNeighbor, availableEdges);

            solutionsPoller.addSolution(f1);
        }

        if(solutionsPoller.getBestCost() < best){
            return solutionsPoller.getBest();
        }else{
            return solutionsPoller.getSolution();
        }
    }
};

#endif
