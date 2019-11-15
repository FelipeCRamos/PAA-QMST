#ifndef _FOREST_H_
#define _FOREST_H_

#include "edge.h"
#include <vector>
#include <utility>
#include <algorithm>

class Forest{
    private:
        void initRootedTree(int x){
            currentRoot = x;
            // TODO
        }

        static std::vector<Edge> avaibleEdges;

        std::vector<int> edgeList;
        std::vector<int> costs; // vetor d
        int numNodes;
        int cost;
        int currentRoot;

        void addEdge(int i){
            // TODO
            currentRoot = -1;
        }

        void removeEdge(int i){
            // TODO
            currentRoot = -1;
        }

        void rootTree(int x){
            if(currentRoot == -1) initRootedTree(x);
            else{
                // TODO: trocar a raiz da rooted tree
            }
        }

        void getNeighbourhood(std::vector<std::pair<int,int>> &neighbourhood, std::vector<int> &neighbourhoodCosts){

        }

        void goToNeighbour(std::pair<int,int> n){

        }



};

#endif
