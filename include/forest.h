#ifndef _FOREST_H_
#define _FOREST_H_

#include "edge.h"

class Forest{

private:

    static vector<Edge> avaibleEdges;
    vector<edgeId> edgeList;
    vector<costType> costs; // vetor d
    costType cost;

    vertexId currentRoot;
    vector<rootedTreeNode> rootedTree;

    void initRootedTree(vertexId root){
        currentRoot = root;

        std::vector<std::pair<vertexId, edgeId>> adj[n + 1];
        for (edgeId e : edgeList) {
    		vertexId u = avaibleEdges[e].u;
    		vertexId v = avaibleEdges[e].v;
    		adj[u].push_back({v, e});
    		adj[v].push_back({u, e});
    	}

    	std::queue<vertexId> q;
    	q.push(currentRoot);
    	while (!q.empty()) {
    		vertexId u = q.front(); q.pop();
    		for (const auto& p : adj[u]) {
                if(p.first == root) continue;
    			if (rootedTree[p.first].first == 0) { // nao ta ligado ainda
    				rootedTree[p.first] = {u, p.second}; // setando atual como pai e a aresta utilizada
    				if (adj[p.first].size() > 1) q.push(p.first); // se tiver filho, otimizacao
    			}
    		}
    	}
    }

    void addEdge(edgeId e, int pos = -1){
        currentRoot = -1;
        edgeList.push_back(e);
        for(int i = 0; i < avaibleEdges; ++i){
            costs[i] += avaibleEdges[i].quadCosts[e];
        }

        if(pos != -1) edgeList[pos] = e;
        else if(edgeList.size() == n - 1){
            for(int i = 0; i < edgeList.size(); ++i){
                if(edgeList[i] == -1){
                    edgeList[i] = e;
                }
            }
        }else{
            edgeList.push_back(e);
        }
    }

    int removeEdge(edgeId e){
        currentRoot = -1;

        for(int i = 0; i < avaibleEdges; ++i)
            costs[i] -= avaibleEdges[i].quadCosts[e];

        for(int i = 0; i < edgeList.size(); ++i){
            if(e == edgeList[i]){
                edgeList[i] = -1;
            }
        }
    }

    void rootTree(vertexId newRoot){
        // if there is no current root, init one
        if(currentRoot == -1) initRootedTree(newRoot);
        else{
            // Reroot tree to newRoot.
            vertexId currentVertex = newRoot;
            rootedTreeNode currentPair = rootedTree[currentVertex];
            rootedTree[newRoot] = {0, 0}; // nova raiz
            while (prev != currentRoot) {
                // back up of the pair the parent from the current vertex pointed to
                rootedTreeNode nextPair = rootedTree[currentPair.first];

                // overwriting the pair the parent from the current vertex pointed to
                // to: the current vertex, the edge the current vertex used to
                // connect to its parents
                rootedTree[currentPair.first] = {currentVertex, currentPair.second};

                // update the current vertex to its parent
                currentVertex = currentPair.first;

                // visit the pair that the parent from the current vertex pointed to
                currentPair = nextPair;
            }
            currentRoot = newRoot; // updt current roo
        }
    }

    void getCycle(vector<edgeId> &cycle, vertexId v, vertexId u){
        rootTree(u);

        // achar caminho de u a v
        vertexId currentVertex = v;
        while (currentVertex != u) {
            cycle.push_back(rootedTree[i].second);
            currentVertex = rootedTree[i].first;
        }
    }

    void getNeighborhood(vector<neighborType> &neighborhood, vector<costType> &neighborhoodCosts, int &bestNeighborIndex){
        bestNeighborIndex = 0; // reset

        for(int i = 0; i < edgeList.size(); ++i){
            // get cycle formed by adding current edge
            std::vector<edgeId> cycle;
            getCycle(cycle, edgeList[i].v, edgeList[i].u);

            if(cycle.size() == 1) continue; // when the edge is already on the tree

            for(auto &idEdge : cycle){ // iterating through edges on formed cycle
                neighborType neighbor = {idEdge, i}; // retriving neighbor
                costType neighborCost = costOfNeighbor(neighbor); // and its cost

                // adding neighbor and its cost to the lists
                neighborhood.push_back(neighbor);
                neighborhoodCosts.push_back(neighborCost);

                // maybe updating the best visited neighbor
                if(neighborCost < neighborhoodCosts[bestNeighborIndex]){
                    bestCost = neighborCost;
                    bestNeighborIndex = neighborhood.size() - 1;
                }
            }
        }
    }

    void goToNeighbor(neighborhoodNode &neighbor){
        edgeId removedEdge = neighbor.first, addedEdge = neighbor.second;
        cost += costs[addedEdge] - costs[removedEdge] - 2 * avaibleEdges[e1].quadCosts[addedEdge];
        removeEdge(removedEdge);
        addEdge(addedEdge);
    }

    costType costOfNeighbor(neighborhoodNode &neighbor){
        edgeId removedEdge = neighbor.first, addedEdge = neighbor.second;
        return cost + costs[addedEdge] - costs[removedEdge] - 2 * avaibleEdges[e1].quadCosts[addedEdge];
    }

    bool isSolution(){
        return edgeList.size() == N - 1;
    }



};

#endif
