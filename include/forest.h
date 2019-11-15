#ifndef _FOREST_H_
#define _FOREST_H_

#include "edge.h"

class Forest{
    private:
        void initRootedTree(int x){
            currentRoot = x;
            // TODO
        }

        static vector<Edge> avaibleEdges;

        vector<int> edgeList;
        vector<int> costs; // vetor d
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

        void getNeighbourhood(vector<pair<int,int>> &neighbourhood, vector<int> &neighbourhoodCosts){

        }

        void goToNeighbour(pair<int,int> n){

        }



};

#endif
