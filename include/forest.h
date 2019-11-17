#ifndef _FOREST_H_
#define _FOREST_H_

#include <set>
#include "edge.h"


class Forest{
public:
    int N, M;
private:
    void initRootedTree(vertexId root, std::vector<Edge> &availableEdges){
        currentRoot = root;
        rootedTree = std::vector<rootedTreeNode>(N + 1);

        std::vector<std::pair<vertexId, edgeId>> adj[N + 1];
        for (edgeId e : edgeList) {
            vertexId u = availableEdges[e].u;
            vertexId v = availableEdges[e].v;
            adj[u].push_back({v, e});
            adj[v].push_back({u, e});
        }

        // for(int i = 0; i < N + 1; ++i){
        //     printf("%d: ", adj[i].size());
        //     for(auto p : adj[i]){
        //         printf("%d %d, ", p.first, p.second);
        //     }
        //     printf("\n");
        // }
        //
        // printf("ook\n");

        std::queue<vertexId> q;
        q.push(currentRoot);
        while (!q.empty()) {
            vertexId u = q.front(); q.pop();
            // printf("%d\n", u);
            for (const auto& p : adj[u]) {
                // printf("> %d %d\n", p.first, p.second);
                // printf(">> %d %d\n", rootedTree[p.first].first, rootedTree[p.first].second);
                if(p.first == root) continue;
                if (rootedTree[p.first].first == 0) { // nao ta ligado ainda
                    rootedTree[p.first] = {u, p.second}; // setando atual como pai e a aresta utilizada
                    if (adj[p.first].size() > 1) q.push(p.first); // se tiver filho, otimizacao
                }
            }
        }
        // for(auto e : rootedTree){
        //     printf("%d %d\n", e.first, e.second);
        // }
        // exit(1);
    }

    void rootTree(vertexId newRoot, std::vector<Edge> &availableEdges){
        // if there is no current root, init one
        if(currentRoot == -1){
            // printf("init root tree\n");
            initRootedTree(newRoot, availableEdges);
            // printf("init root tree\n");
        }else{
            // Reroot tree to newRoot.
            vertexId currentVertex = newRoot;
            rootedTreeNode currentPair = rootedTree[currentVertex];
            rootedTree[newRoot] = {0, 0}; // nova raiz
            while (currentVertex != currentRoot) {
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

    void getCycle(std::vector<edgeId> &cycle, vertexId v, vertexId u, std::vector<Edge> &availableEdges){
        // printf("rooting tree\n");
        rootTree(u, availableEdges);
        // printf("rooting tree\n");

        // printf("%d %d\n", v, u);

        // achar caminho de u a v
        vertexId currentVertex = v;
        while (currentVertex != u) {
            // printf("%d\n", currentVertex);
            cycle.push_back(rootedTree[currentVertex].second);
            currentVertex = rootedTree[currentVertex].first;
        }
        // exit(1);
    }

public:
    std::set<edgeId> edgeList;
    std::vector<costType> costs; // vetor d
    costType cost;
    vertexId currentRoot;
    std::vector<rootedTreeNode> rootedTree;

    Forest(int n, int m){
        N = n, M = m;
        costs = std::vector<costType>(M, 0);
        cost = 0;
        currentRoot = -1;
        costs = std::vector<costType>(M, 0);
    }

    void setCostsVector(std::vector<Edge> &availableEdges){
        for(int i = 0; i < availableEdges.size(); ++i){
            costs[i] = availableEdges[i].linearCost;
        }
    }

    void addEdge(edgeId e, std::vector<Edge> &availableEdges){
        currentRoot = -1;
        edgeList.insert(e);

        cost += costs[e];

        for(int i = 0; i < M; ++i){
            costs[i] += 2 * availableEdges[i].quadCosts[e];
        }
    }

    void removeEdge(edgeId e, std::vector<Edge> &availableEdges){
        // printf(">>>>>>>>>>>>>>>>> %d %d\n", e, M);

        currentRoot = -1;
        edgeList.erase(e);

        cost -= costs[e];

        for(int i = 0; i < M; ++i){
            costs[i] -= 2 * availableEdges[i].quadCosts[e];
        }
    }

    void getNeighborhood(std::vector<neighborType> &neighborhood, std::vector<costType> &neighborhoodCosts, int &bestNeighborIndex, std::vector<Edge> &availableEdges){
        bestNeighborIndex = 0; // reset

        for(int i = 0; i < M; ++i){
            // get cycle formed by adding current edge
            std::vector<edgeId> cycle;
            getCycle(cycle, availableEdges[i].v, availableEdges[i].u, availableEdges);

            if(cycle.size() == 1) continue; // when the edge is already on the tree

            for(auto &idEdge : cycle){ // iterating through edges on formed cycle
                neighborType neighbor = {idEdge, i}; // retriving neighbor
                costType neighborCost = costOfNeighbor(neighbor, availableEdges); // and its cost

                // adding neighbor and its cost to the lists
                neighborhood.push_back(neighbor);
                neighborhoodCosts.push_back(neighborCost);

                // maybe updating the best visited neighbor
                if(neighborCost < neighborhoodCosts[bestNeighborIndex]){
                    bestNeighborIndex = neighborhood.size() - 1;
                }
            }
        }
    }

    void goToNeighbor(neighborType &neighbor, std::vector<Edge> &availableEdges){
        edgeId removedEdge = neighbor.first, addedEdge = neighbor.second;
        removeEdge(removedEdge, availableEdges);
        addEdge(addedEdge, availableEdges);
    }

    costType costOfNeighbor(neighborType &neighbor, std::vector<Edge> &availableEdges){
        edgeId removedEdge = neighbor.first, addedEdge = neighbor.second;
        int ans = cost - costs[removedEdge] + costs[addedEdge] - 2 * availableEdges[removedEdge].quadCosts[addedEdge];
        return ans;
    }

    bool isSolution(){
        return edgeList.size() == N - 1;
    }

    bool operator<(const Forest& f) const {
        return cost < f.cost;
    }

    bool operator==(const Forest& f) const {
        return edgeList == f.edgeList;
    }



};

std::ostream& operator<<(std::ostream& os, const Forest& f){
  os << "Floresta: n: " << f.N << " m: " << f.M << " cost: " << f.cost << "\n";
  os << "Ids das arestas na arvore: ";
  for(auto &e : f.edgeList) os << e << " ";
  os << "\n";
  return os;
}

#endif
