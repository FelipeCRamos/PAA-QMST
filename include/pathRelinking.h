#ifndef _PATH_RELINLING_H
#define _PATH_RELINLING_H

#include <algorithm>
#include <sstream>
#include <set>
#include <chrono>
#include <random>
#include "forest.h"

#define debugPrints false

/*
    Fluxo pensado:
        -> Obtem arvore original e destino
        -> Obtem a diferença simétrica das arvores
        -> Escolhe uma aresta da diferenca simetrica para adicionar na original
        -> Remove uma aresta do caminho dos dois vertices envolvidos na aresta escolhida (para não criar ciclo)
        -> retorna arvore com o path relinking

        // Pensamento até o momento: Retornar melhor path relink encontrado entre as arvores
*/

namespace ParticleSwarm {
    // ========================================================================
    // AUXILIARY TYPES
    // ========================================================================

    typedef int Vertice;

    typedef struct Edge_s {
/*{{{*/
        Vertice v1; Vertice v2;
        int cost;

        Edge_s() { v1 = 0; v2 = 0; cost = 0; }

        Edge_s(Vertice v1, Vertice v2, int cost) {
            this->v1 = v1;
            this->v2 = v2;
            this->cost = cost;
        }

        std::string print() {
            std::stringstream ss;
            ss << "(" << this->v1 << "," << this->v2 << "|" << this->cost << ")";
            return ss.str();
        }

        bool operator==(const Edge_s &e2) {
            return (this->v1 == e2.v1) && (this->v2 == e2.v2);
        }
/*}}}*/
    } Edge;

    typedef struct QuadraticCost_s {
/*{{{*/
        Edge e1;
        Edge e2;
        int cost;

        QuadraticCost_s(){}

        QuadraticCost_s(Edge e1, Edge e2, int cost) {
            this->e1 = e1;
            this->e2 = e2;
            this->cost = cost;
        }
/*}}}*/
    } QuadraticCost;

    // A structure to represent a subset for union-find  
    typedef struct Subset_s {
/*{{{*/
        int parent;
        int rank;
/*}}}*/
    } Subset;

    // ========================================================================
    // AUXILIARY FUNCTIONS
    // ========================================================================

    // A utility function to find set of an element i  
    // (uses path compression technique)  
    int find(Subset subsets[], int i)  
    {  
/*{{{*/
        // find root and make root as parent of i  
        // (path compression)  
        if (subsets[i].parent != i)  
            subsets[i].parent = find(subsets, subsets[i].parent);  

        return subsets[i].parent;  
/*}}}*/
    }  

    int find(std::vector<Subset> &subsets, int i) {
/*{{{*/
        if (subsets[i].parent != i)  
            subsets[i].parent = find(subsets, subsets[i].parent);  

        return subsets[i].parent;  
/*}}}*/
    }

    // A function that does union of two sets of x and y  
    // (uses union by rank)  
    void Union(Subset subsets[], int x, int y)  
    {  
/*{{{*/
        int xroot = find(subsets, x);  
        int yroot = find(subsets, y);  

        // Attach smaller rank tree under root of high  
        // rank tree (Union by Rank)  
        if (subsets[xroot].rank < subsets[yroot].rank)  
            subsets[xroot].parent = yroot;  
        else if (subsets[xroot].rank > subsets[yroot].rank)  
            subsets[yroot].parent = xroot;  

        // If ranks are same, then make one as root and  
        // increment its rank by one  
        else
        {  
            subsets[yroot].parent = xroot;  
            subsets[xroot].rank++;  
        }  
/*}}}*/
    }  

    void Union(std::vector<Subset> &subsets, int x, int y) {
/*{{{*/
        int xroot = find(subsets, x);
        int yroot = find(subsets, y);


        // Attach smaller rank tree under root of high  
        // rank tree (Union by Rank)  
        if (subsets[xroot].rank < subsets[yroot].rank)  
            subsets[xroot].parent = yroot;  
        else if (subsets[xroot].rank > subsets[yroot].rank)  
            subsets[yroot].parent = xroot;  

        // If ranks are same, then make one as root and  
        // increment its rank by one  
        else
        {  
            subsets[yroot].parent = xroot;  
            subsets[xroot].rank++;  
        }
/*}}}*/
    }

    // Compare two edges according to their weights.  
    // Used in qsort() for sorting an array of edges  
    int myComp(const void* a, const void* b) {
/*{{{*/
        Edge* a1 = (Edge*)a;  
        Edge* b1 = (Edge*)b;  
        return a1->cost > b1->cost;  
/*}}}*/
    }

    // ========================================================================
    // AUXILIARY CLASSES
    // ========================================================================

    class Graph {
/*{{{*/
        public:
            int numberOfVertices;
            int numberOfEdges;
            int cost;
            long long int quadraticCost;
            std::vector<Edge> edges;
            std::vector<int> quadraticCosts;
            // std::vector<QuadraticCost> quadraticCosts;

            Graph() {} // empty constructor

            // Initialize a graph with legacy args
            Graph(int numberOfVertices, int numberOfEdges, int **costs, std::vector<std::pair<int, int>> edges) {
/*{{{*/
                if(debugPrints) std::cout << "[Graph] Entered on legacy graph constructor.\n";
                this->numberOfVertices = numberOfVertices;
                this->numberOfEdges = numberOfEdges;    


                // inicialize edges
                for(int i = 0; i < numberOfEdges; i++) {
                    auto currEdge = Edge(edges[i].first, edges[i].second, costs[i][i]);
                    // std::cout << "Inicializing edge " << currEdge.print() << std::endl;
                    this->edges.push_back(currEdge);
                }

                this->quadraticCost = 0;
                this->quadraticCosts = std::vector<int>((numberOfEdges * numberOfEdges) + 2); 
                for( int i = 0; i < numberOfEdges*numberOfEdges; i++ ) quadraticCosts[i] = 0;

                // inicialize quadratic costs of edges
                for( int i = 0; i < numberOfEdges; i++ ) {
                    for( int j = 0; j < numberOfEdges; j++ ) {
                        if(i != j) {
                            // quadratic cost between edges #i and #j
                            if(costs[i][j] < 0) { 
                                if(debugPrints) std::cout << "\n\n\nERROR HERE:\n";
                                if(debugPrints) printf("On edge[%i][%i] cost was %i\n", i, j, costs[i][j]);
                            }
                            this->quadraticCosts[i * numberOfEdges + j] = costs[i][j];
                            // this->quadraticCosts.push_back(qCurrCost);
                        } else {
                            // stub
                            this->quadraticCosts[i * numberOfEdges + j] = 0;
                        }
                    }
                }

                this->quadraticCost = getQuadraticCost();
/*}}}*/
            } 

            void allocQuadraticCosts(long int number) {
/*{{{*/
                if(debugPrints) std::cout << "[Graph:allocQuadraticCosts] Entered on allocQuadraticCosts.\n";
                if(debugPrints) std::cout << "\tAllocating vector, was null.\n";
                this->quadraticCosts = std::vector<int>(number, 0);
                for(int i = 0; i < number; i++)
                    this->quadraticCosts[i] = 0;
/*}}}*/
            }

            int getQuadraticCost() {
/*{{{*/
                if(debugPrints) std::cout << "[Graph:getQuadraticCost] Entered on getQuadraticCost.\n";
                int result = 0;

                // failsafe
                if(quadraticCosts.capacity() == 0)
                    allocQuadraticCosts(this->edges.size());

                int edgeCount = this->edges.size();
                if(debugPrints) std::cout << "\tEdges size: " << edgeCount
                    << std::endl;

                for( int i = 0; i < edgeCount; i++ ) {
                    // add linear cost
                    result += this->edges[i].cost;
                    // std::cout << "\tresult += " <<
                        // this->edges[i].cost << std::endl;

                    // add quadratic cost
                    for( int j = 0; j < edgeCount; j++ ) {
                        int currQCost = this->quadraticCosts[i * edgeCount + j];
                        result += currQCost;
                        // printf("\t\t[%i][%i]: ",i, j);
                        // std::cout << "result += " << currQCost << std::endl;
                    }
                }
                return result;
/*}}}*/
            }

            // return -1 if a edge is not there, else return it's cost
            int getCost(Vertice one, Vertice two) {
/*{{{*/
                // std::cout << "[getCost] Entered on getCost.\n";
                for( auto &edge : this->edges ) {
                    if((edge.v1 == one && edge.v2 == two) || (edge.v2 == one && edge.v1 == two)) {
                        return edge.cost;
                    }
                }
                return 0;
/*}}}*/
            }

            void updateNumbers() {
/*{{{*/
                if(debugPrints) std::cout << "[Graph:updateNumbers] Entered on updateNumbers.\n";
                this->numberOfEdges = getNumberOfEdges();
                this->numberOfVertices = getNumberOfVertices();
                this->cost = getCost();
                this->quadraticCost = getQuadraticCost();
/*}}}*/
            }

            int getNumberOfVertices() {
/*{{{*/
                if(debugPrints) std::cout << "[Graph:getNumberOfVertices] Entered on getNumberOfVertices.\n";
                std::set<int> vertices; // to remove duplicateds
                for(auto &edge : this->edges) {
                    vertices.insert(edge.v1);
                    vertices.insert(edge.v2);
                }
                return vertices.size();
/*}}}*/
            }

            int getNumberOfEdges() {
/*{{{*/
                if(debugPrints) std::cout << "[Graph:getNumberOfEdges] Entered on getNumberOfEdges.\n";
                return this->edges.size();
/*}}}*/
            }

            int getCost() {
/*{{{*/
                if(debugPrints) std::cout << "[Graph:getCost] Entered on getCost.\n";
                int costSum = 0;
                // IS WRONG< FIX THIS!! TODO (TEMPORARY WORKAROUND)
                for(auto &edge : this->edges) { costSum += edge.cost; }

                return costSum;
/*}}}*/
            }

            bool hasEdge(Vertice one, Vertice two) {
/*{{{*/
                // if(debugPrints) std::cout << "[Graph:hasEdge] Entered on hasEdge.\n";
                return getCost(one, two) != 0;
/*}}}*/
            }

            // Overload to make std::set<Graph> work
            friend bool operator<(const Graph &one, const Graph &another) {
/*{{{*/
                if(debugPrints) std::cout << "[Graph:operator<] Entered on operator<.\n";
                if(one.cost < another.cost) {
                    return true;
                }
                return false;
/*}}}*/
            }

            std::string print() {
/*{{{*/
                std::stringstream ss;
                ss << "Graph:\n";
                ss << "\tVertices: " << this->numberOfVertices << "\n";
                ss << "\tEdges: " << this->numberOfEdges << "\n";
                ss << "\tCost: " << this->cost << "\n";
                ss << "\tEdges:\n";
                for(auto &edge : this->edges) {
                    ss << "\t\t" << edge.print() << "\n";
                }
                return ss.str();
/*}}}*/
            }
/*}}}*/
    };

    class PathRelinking {
/*{{{*/
        private:
            Graph originalGraph; 
            Graph targetGraph;
        public:
            
            // Default constructor
            PathRelinking() {}

            // Custom constructor
            PathRelinking(Graph originalGraph, Graph targetGraph) {
/*{{{*/
                if(debugPrints) std::cout << "[PathRelinking] Entered on PathRelinking custom constructor.\n";
                this->originalGraph = originalGraph;
                this->targetGraph = targetGraph;
/*}}}*/
            }

            // Get the graph between the two graphs (original and target)
            Graph relink() {
                /*{{{*/
                if(debugPrints) std::cout << "[PathRelinking:Relink] Entered.\n";
                // std::cout << "[relink] ------------------------------------\n";
                auto dif = getSimmetryDifferenceEdges();

                auto originEdges = dif.first;
                auto targetEdges = dif.second;
                // std::cout << "-----------nn--\n";
                // for(auto &edge : originEdges) {
                    // std::cout << "\toriginEdge: " << edge.print() << std::endl;
                // }

                auto cmpFunction = [](const Edge &lhs, const Edge &rhs){
                    return lhs.cost < rhs.cost;
                };

                auto inverseCmpFuncion = [](const Edge &lhs, const Edge &rhs) {
                    return lhs.cost > rhs.cost;
                };

                std::sort( targetEdges.begin(), targetEdges.end(), cmpFunction);

                // for(auto &edge : targetEdges) {
                    // std::cout << "\ttargetEdge: " << edge.print() << std::endl;
                // }
                // std::cout << "-----------nn--\n";

                // select one from targetEdges (the one with smaller cost)
                std::sort(originEdges.begin(), originEdges.end(), inverseCmpFuncion);
                // std::sort(targetEdges.begin(), targetEdges.end(), inverseCmpFuncion);

                if( originEdges.size() != 0 && targetEdges.size() ) {
                    // if there are some vertics
                    // std::cout << "Removing vertices!\n";
                    goToNeigh(originEdges[0], targetEdges[0], this->originalGraph.edges);
                }

                // add-it to original graph

                return this->originalGraph;
/*}}}*/
            }

        private:

            // Get simmetry diff between the two graphs
            std::pair<std::vector<Edge>, std::vector<Edge>> getSimmetryDifferenceEdges() {
                /*{{{*/
                if(debugPrints) std::cout << "[PathRelinking:getSimmetryDifferenceEdges] Entered.\n";
                // std::vector<Edge> accEdges;
                std::vector<Edge> originEdges;
                std::vector<Edge> targetEdges;

                
                // Get all edges that are not in the intersec of both tree's
                for(auto &edge : this->originalGraph.edges) {
                    if( !this->targetGraph.hasEdge(edge.v1, edge.v2) ) {
                        originEdges.push_back(edge);
                    }
                }

                for(auto &edge : this->targetGraph.edges) {
                    if( !this->originalGraph.hasEdge(edge.v1, edge.v2) ) {
                        targetEdges.push_back(edge);
                    }
                }

                // and return them
                return std::make_pair(originEdges, targetEdges);
/*}}}*/
            }

            // Remove one original edge and then add a target edge
            void goToNeigh(Edge e1, Edge e2, std::vector<Edge> &edges){
                /*{{{*/
                if(debugPrints) std::cout << "[PathRelinking:goToNeigh] Entered.\n";
                Edge removededge = e1, addededge = e2;
                /*
                std::cout << "Edges before relink: [ ";
                for( auto &it : edges ) {
                    std::cout << it.print() << " ";
                }
                std::cout << "]\n";
                */

                removeEdge(removededge, edges);
                addEdge(addededge, edges);

                /*
                std::cout << "Edges after relink: [ ";
                for( auto &it : edges ) {
                    std::cout << it.print() << " ";
                }
                std::cout << "]\n";
                */
/*}}}*/
            }

            // Perform removal of an edge
            void removeEdge(Edge e, std::vector<Edge> &availableEdges){
/*{{{*/
                if(debugPrints) std::cout << "[PathRelinking:removeEdge] Entered.\n";
                // currentRoot = -1;
                // edgeList.erase(e);
                for(
                    std::vector<Edge>::iterator it = this->originalGraph.edges.begin();
                    it != this->originalGraph.edges.end();
                    it++
                ) {
                    if(*it == e) {
                        // std::cout << "[removeEdge] Removing edge " << it->print() << std::endl;
                        availableEdges.erase(it);
                        break;
                    }
                }
                // availableEdges.erase()
                // this->originalGraph.
                // cost -= costs[e];
                // for(int i = 0; i < M; ++i){
                    // costs[i] -= 2 * availableEdges[i].quadCosts[e];
                // }
                /*}}}*/
            }

            // Perform the addition of an edge
            void addEdge(Edge e, std::vector<Edge> &availableEdges) {
                /*{{{*/
                if(debugPrints) std::cout << "[PathRelinking:addEdge] Entered.\n";
                // std::cout << "[addEdge] Adding edge " << e.print() << std::endl;
                availableEdges.push_back(e);
/*}}}*/
            }
/*}}}*/
    };


    class ParticleSwarm{
/*{{{*/
        private:
            Graph originalGraph;            //!< Original graph
            std::vector<int> edgeOrder;     //!< Current edge order to feed kruskal
            std::vector<Graph> trees;       //!< Current generation tree's (particles)
            long int bestQuadraticCost;     //!< Current generation best quadratic cost (GBEST)

        public:
            // Default constructor
            ParticleSwarm(Graph originalGraph, int numberOfParticles = 10) {
/*{{{*/
                if(debugPrints) std::cout << "[ParticleSwarm:ParticleSwarm] Entered.\n";

                this->originalGraph = originalGraph;
                // this->bestQuadraticCost = this->originalGraph.quadraticCost;

                // generate initial order
                for(int i = 0; i < this->originalGraph.numberOfEdges; i++) {
                    this->edgeOrder.push_back(i);
                }

                if(debugPrints) std::cout << "\tOriginal graph stored!\n";

                // Generate the initial set of spanning trees
                for(int i = 0; i < numberOfParticles; i++) {
                    auto tree = generateKruskal();
                    if(debugPrints) std::cout << "\tTree has been"
                        << " generated, updating it's values.\n";

                    tree.updateNumbers();
                    this->trees.push_back(tree);
                }

                if(debugPrints) std::cout << "\tGenerated " << trees.size() 
                    << " initial trees." << std::endl;
                // and then wait to run the algorithm
/*}}}*/
            }

            // Get the current edge position in this->originalGraph.edges
            int getEdgePosition(Edge e) {
/*{{{*/
                auto begin = this->originalGraph.edges.begin();
                auto end = this->originalGraph.edges.end();
                int j = 0;
                for( std::vector<Edge>::iterator it = begin; it != end; it++ ) {
                    if(*it == e) { return j; }
                    j++;
                }
                return -1;
/*}}}*/
            }

            // Advance current particle generation
            int advanceGeneration() {
/*{{{*/
                if(debugPrints) std::cout << "[ParticleSwarm:advanceGeneration] Entered.\n";
                auto compareFunc = [](Graph &one, Graph &another){
                    return one.getQuadraticCost() < another.getQuadraticCost();
                };

                // std::sort(this->trees.begin(), this->trees.end(), compareFunc);
                if(debugPrints) std::cout << "[advanceGeneration] Sorting done!" << std::endl;
                {
                    if(debugPrints) std::cout << "Sorted trees: [ ";
                    int i = 0;
                    for( auto &tree : this->trees ) {
                        if(debugPrints) std::cout << tree.quadraticCost << "^" << tree.edges.size() << " ";
                        if( i++ > 30 ) { if(debugPrints) std::cout << "... "; break; }
                    }
                    if(debugPrints) std::cout << "]\n";
                }

                // make every particle walk towards the global best
                auto globalBestCost = trees[0].quadraticCost;

                std::vector<Graph> nextGenerationTrees;

                // path relink between all <-> best tree
                int currTreeNumber = 0; 
                for(auto &tree : this->trees) {
                    if(debugPrints) std::cout << "\tUpdating tree " << currTreeNumber++ << ".\n";
                    auto relink = PathRelinking(tree, trees[0]);
                    auto relinkedTree = relink.relink();
                    relinkedTree.updateNumbers();
                    nextGenerationTrees.push_back(relinkedTree);
                    // std::cout << "Best relinked tree cost: "
                        // << relinkedTree.quadraticCost << std::endl;
                }

                std::sort(nextGenerationTrees.begin(), nextGenerationTrees.end(), compareFunc);
                std::cout << "Global best generation tree qCost: "
                    << nextGenerationTrees[0].quadraticCost << std::endl;

                this->trees = std::vector<Graph>(nextGenerationTrees);
                return nextGenerationTrees[0].quadraticCost;
/*}}}*/
            }

            // Auxiliar funcion to get next permutation (used on kruskal)
            void getNextOrderPermutation() {
/*{{{*/
                if(debugPrints) std::cout << "[ParticleSwarm:getNextOrderPermutation] Entered.\n";
                // std::next_permutation(edgeOrder.begin(), edgeOrder.end());
                // std::cout << "Made permutation.\n";
                auto seed = std::chrono::system_clock::now().time_since_epoch().count();
                std::mt19937 g1 (seed);

                // auto r = g1() % g2();
                // std::cout << "new seed: " << g1() << std::endl;
                shuffle(edgeOrder.begin(), edgeOrder.end(), std::mt19937(g1()));
/*}}}*/
            }

            // Auxiliar function to print current permutation (used on kruskal)
            std::string printCurrentKruskalOrder() {
/*{{{*/
                if(debugPrints) std::cout << "[ParticleSwarm:printCurrentKruskalOrder] Entered.\n";
                std::stringstream ss; 
                ss << "[ ";
                for( auto &it : this->edgeOrder ) { ss << it << " "; }
                ss << "]";
                return ss.str();
/*}}}*/
            }

            // Generate one spanning tree with kruskal algorithm
            Graph generateKruskal() {
/*{{{*/
                if(debugPrints) std::cout << "[ParticleSwarm:generateKruskal] Entered.\n";
                this->getNextOrderPermutation();

                // for( auto &it : edgeOrder ) {
                    // std::cout << it << " ";
                // }
                // std::cout << std::endl;

                // create Graph (that's a tree)
                Graph genTree = Graph();

                // Configure number of edges and vertices
                genTree.numberOfVertices = this->originalGraph.numberOfVertices;
                genTree.numberOfEdges = this->originalGraph.numberOfEdges;

                // Attempt to generate tree with kruskal algorithm
                // make room on subsets vector to hold all elements
                std::vector<Subset> subsets(genTree.numberOfVertices * genTree.numberOfVertices);

                if(debugPrints) std::cout << "\tCurrent subsets capacity: " << subsets.capacity()
                    << std::endl;

                // initialize union find
                for(int v = 0; v <= genTree.numberOfVertices; v++) {
                    subsets[v].parent = v;
                    subsets[v].rank = 0;
                }

                // kruskal algorithm
                int e = 0, i = 0;
                while( e < genTree.numberOfVertices && i < genTree.numberOfEdges) {

                    // Get next edge to add to the tree
                    Edge nextEdge = this->originalGraph.edges[this->edgeOrder[i++]];

                    // check union find
                    int x = find(subsets, nextEdge.v1);
                    int y = find(subsets, nextEdge.v2);

                    // if they don't form a cicle, add them
                    if(x != y) {
                        e++;
                        genTree.edges.push_back(nextEdge);
                        Union(subsets, x, y);
                    }
                }
                
                // Make room for all quadratic costs
                genTree.allocQuadraticCosts(this->originalGraph.numberOfEdges * this->originalGraph.numberOfEdges);

                // in this moment, is all zeros...
                if(debugPrints) std::cout << "\tqCosts: [ ";
                for(auto &it : genTree.quadraticCosts) if(debugPrints) std::cout << it << " ";
                if(debugPrints) std::cout << "]\n";

                // update quadratic costs
                for( auto &edge : genTree.edges ) {
                    // for each edge on added edges, get its index
                    int index = getEdgePosition(edge);
                    for( auto &edgeIt : genTree.edges ) {
                        // for every other edge, get the quadratic cost between them
                        int indexIt = getEdgePosition(edgeIt);

                        int offset = index * this->originalGraph.numberOfEdges + indexIt;
                        genTree.quadraticCosts[offset] = this->originalGraph.quadraticCosts[offset];
                    }
                }

                if(debugPrints) std::cout << "\tupdated qCosts: [ ";
                for(auto &it : genTree.quadraticCosts) if(debugPrints) std::cout << it << " ";
                if(debugPrints) std::cout << "]\n";

                /*if(debugPrints)*/ std::cout << "\tgenerated kruskal tree: [ ";
                for(auto &it : genTree.edges) {
                    /*if(debugPrints)*/ std::cout << it.print() << "^" << getEdgePosition(it) << " ";
                }
                /*if(debugPrints)*/ std::cout << "] " << genTree.edges.size() << " ";
                std::cout << genTree.getQuadraticCost() << std::endl;

                genTree.updateNumbers();

                return genTree;
/*}}}*/
            }

/*}}}*/
    };
}

#endif
