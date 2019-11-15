#ifndef _FOREST_H_
#define _FOREST_H_

#include "edge.h"

class Forest{
    typedef int edgeId;
    typedef int vertexId;
    typedef int costType;
    typedef int neighborCost;
    typedef std::pair<vertexId, edgeId> rootedTreeNode;
    typedef std::pair<edgeId, edgeId> neighborhoodNode;
private:

    static vector<Edge> avaibleEdges;
    vector<edgeId> edgeList;
    vector<costType> costs; // vetor d
    int numNodes;
    int cost;

    int currentRoot;
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

    void addEdge(edgeId i){
        edgeList.push_back(e);
        currentRoot = -1;
    }

    void removeEdge(edgeId e){
        // TODO
        currentRoot = -1;
    }

    bool isEdgeOnTree(edgeId e){
        // TODO
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

    void getNeighborhood(vector<neighborhoodNode> &neighborhood, vector<neighborCost> &neighborhoodCosts, neighborhoodNode &best){
        // para cada aresta que nao ta, boto ela
        neighborCost bestCost = INF;
        for(int i = 0; i < edgeList.size(); ++i){
            if(isEdgeOnTree(i)) continue;

            std::vector<edgeId> cycle;
            getCycle(cycle, edgeList[i].v, edgeList[i].u);

            for(auto &idEdge : cycle){
                neighborhoodNode neighbor = {idEdge, i};
                neighborCost neighborCost = costOfNeighbor(neighbor)
                neighborhood.push_back(neighbor);
                neighborhoodCosts.push_back(neighborCost);

                if(neighborCost < bestCost){
                    bestCost = neighborCost;
                    best = neighbor;
                }
            }
        }
    }

    void goToNeighbor(neighborhoodNode &neighbor){
        // addEdge;
        // removeEdge;
        // updt custos;
    }

    int costOfNeighbor(neighborhoodNode &neighbor){
        // TODO
    }



};

#endif
