#ifndef _PATH_RELINLING_H
#define _PATH_RELINLING_H

// maybe i need those #include <vector> #include <list>
#include <algorithm>
#include <sstream>
#include <set>
#include <chrono>
#include <random>
#include "forest.h"

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
    typedef int Vertice;

    typedef struct Edge_s {
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
            ss << "(" << this->v1 << ", " << this->v2 << " | " << this->cost << ")";
            return ss.str();
        }
    } Edge;

    typedef struct QuadraticCost_s {
        Edge e1;
        Edge e2;
        int cost;

        QuadraticCost_s(){}

        QuadraticCost_s(Edge e1, Edge e2, int cost) {
            this->e1 = e1;
            this->e2 = e2;
            this->cost = cost;
        }

    } QuadraticCost;

    class Graph {
        public:
            int numberOfVertices;
            int numberOfEdges;
            int cost;
            std::vector<Edge> edges;
            std::vector<QuadraticCost> quadraticCosts;

            Graph() {/* default */} // default constructor

            Graph(int numberOfVertices, int numberOfEdges, int **costs, std::vector<std::pair<int, int>> edges) {
                this->numberOfVertices = numberOfVertices;
                this->numberOfEdges = numberOfEdges;    

                // inicialize edges
                for(int i = 0; i < numberOfEdges; i++) {
                    auto currEdge = Edge(edges[i].first, edges[i].second, costs[i][i]);
                    // std::cout << "Inicializing edge " << currEdge.print() << std::endl;
                    this->edges.push_back(currEdge);
                }

                // inicialize quadratic costs of edges
                for( int i = 0; i < numberOfEdges; i++ ) {
                    for( int j = 0; j < numberOfEdges; j++ ) {
                        if(i != j) {
                            // quadratic cost between edges #i and #j
                            // std::cout << "\tEdge #" << i+1 << " ~ Edge #" << j+1 << ": " << costs[i][j] << std::endl;
                            auto qCurrCost = QuadraticCost(this->edges[i], this->edges[j], costs[i][j]);
                            this->quadraticCosts.push_back(qCurrCost);
                        }
                    }
                }
            } 

            // return -1 if a edge is not there, else return it's cost
            int getCost(Vertice one, Vertice two) {
                for( auto &edge : this->edges ) {
                    if((edge.v1 == one && edge.v2 == two) || (edge.v2 == one && edge.v1 == two)) {
                        return edge.cost;
                    }
                }
                return -1;
            }

            void updateNumbers() {
                this->numberOfEdges = getNumberOfEdges();
                this->numberOfVertices = getNumberOfVertices();
                this->cost = getCost();
            }

            int getNumberOfVertices() {
                std::set<int> vertices; // to remove duplicateds
                for(auto &edge : this->edges) {
                    vertices.insert(edge.v1);
                    vertices.insert(edge.v2);
                }
                return vertices.size();
            }

            int getNumberOfEdges() {
                return this->edges.size();
            }

            int getCost() {
                int costSum = 0;
                // IS WRONG< FIX THIS!! TODO (TEMPORARY WORKAROUND)
                for(auto &edge : this->edges) { costSum += edge.cost; }

                return costSum;
            }

            bool hasEdge(Vertice one, Vertice two) {
                return getCost(one, two) != -1;
            }

            // Overload to make std::set<Graph> work
            friend bool operator<(const Graph &one, const Graph &another) {
                if(one.cost < another.cost) {
                    return true;
                }
                return false;
            }

            std::string print() {
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
            }
    };

    class PathRelinking {
        private:
            Graph originalGraph; 
            Graph targetGraph;
        public:
            PathRelinking() {}

            PathRelinking(Graph originalGraph, Graph targetGraph) {
                this->originalGraph = originalGraph;
                this->targetGraph = targetGraph;
            }

            // Get a list of `numberOfRelinks` tree's in the way
            Graph relink() {
                std::cout << "[relink] ------------------------------------\n";
                auto dif = getSimmetryDifferenceEdges();

                auto originEdges = dif.first;
                auto targetEdges = dif.second;
                for(auto &edge : originEdges) {
                    std::cout << "\toriginEdge: " << edge.print() << std::endl;
                }

                auto cmpFunction = [](const Edge &lhs, const Edge &rhs){
                    return lhs.cost < rhs.cost;
                };
                std::sort( targetEdges.begin(), targetEdges.end(), cmpFunction);

                for(auto &edge : targetEdges) {
                    std::cout << "\ttargetEdge: " << edge.print() << std::endl;
                }

                // select one from targetEdges (the one with smaller cost)
                // add-it to original graph

                return Graph();
            }

        private:
            // internal funcs
            std::pair<std::vector<Edge>, std::vector<Edge>> getSimmetryDifferenceEdges() {
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
            }

            std::list<Edge> _getPath(Vertice actual, Vertice destiny, std::vector<bool> &visited) {
                // get adjacent vertices
                // recurse into them
            }

            std::list<Edge> getPath(Vertice source, Vertice destiny) {
                // Get all edges of the path from source to destiny
                int numberOfEdges = 0; // stub
                std::vector<bool> visitedEdges(numberOfEdges, false);

                std::list<Edge> currentPath = _getPath(source, destiny, visitedEdges);
                // and return them
                return std::list<Edge>();   // stub
            }

            void removeEdge(std::list<Edge> edges) {
                // remove ONE (or maybe more) edge(s) from the tree based on some
                // criteria
            }
    };

    /* Things used on ParticleSwarm in GFG */
/*{{{*/
    // A structure to represent a subset for union-find  
    class subset  
    {  
        public: 
        int parent;  
        int rank;  
    };  
      
    // A utility function to find set of an element i  
    // (uses path compression technique)  
    int find(subset subsets[], int i)  
    {  
        // find root and make root as parent of i  
        // (path compression)  
        if (subsets[i].parent != i)  
            subsets[i].parent = find(subsets, subsets[i].parent);  
      
        return subsets[i].parent;  
    }  

    int find(std::vector<subset> &subsets, int i) {
        if (subsets[i].parent != i)  
            subsets[i].parent = find(subsets, subsets[i].parent);  

        return subsets[i].parent;  
    }

    // A function that does union of two sets of x and y  
    // (uses union by rank)  
    void Union(subset subsets[], int x, int y)  
    {  
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
    }  

    void Union(std::vector<subset> &subsets, int x, int y) {
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
    }

    // Compare two edges according to their weights.  
    // Used in qsort() for sorting an array of edges  
    int myComp(const void* a, const void* b)  
    {  
        Edge* a1 = (Edge*)a;  
        Edge* b1 = (Edge*)b;  
        return a1->cost > b1->cost;  
    }
    /*}}}*/
    /* eof */

    class ParticleSwarm{
        private:
            Graph originalGraph;
            std::vector<int> edgeOrder;
            std::vector<Graph> trees;

        public:
            ParticleSwarm(Graph originalGraph, int numberOfParticles = 10) {
                this->originalGraph = originalGraph;

                // generate initial order
                for(int i = 0; i < this->originalGraph.numberOfEdges; i++) {
                    this->edgeOrder.push_back(i);
                }

                std::cout << "[particleSwarm] Original graph stored!\n";
                // std::cout << "[particleSwarm] Current Order:" << std::endl;
                // std::cout << printCurrentKruskalOrder() << std::endl;

                // Generate the initial set of spanning trees
                for(int i = 0; i < numberOfParticles; i++) {
                    auto tree = generateKruskal();
                    this->trees.push_back(tree);
                }

                std::cout << "[ParticleSwarm] Generated " << trees.size() 
                    << " initial trees." << std::endl;
                // and then wait to run the algorithm
            }

            void advanceGeneration() {
                auto compareFunc = [](Graph &one, Graph &another){
                    return one.cost < another.cost;
                };

                std::sort(this->trees.begin(), this->trees.end(), compareFunc);
                std::cout << "[advanceGeneration] Sorting done!" << std::endl;

                std::cout << "Sorted trees: [ ";
                int i = 0;
                for( auto &tree : this->trees ) {
                    std::cout << tree.cost << " ";
                    if( i++ > 30 ) { std::cout << "... "; break; }
                }
                std::cout << "]\n";

                // make every particle walk towards the global best
                auto globalBestCost = trees[0].cost;

                // path relink between all <-> best tree
                for(auto &tree : this->trees) {
                    auto relink = PathRelinking(tree, trees[0]);
                    auto relinkedTree = relink.relink();
                    // std::cout << "Cost of relinked tree: " << relinkedTree.cost
                        // << std::endl;
                }

            }

            void getNextOrderPermutation() {
                std::next_permutation(edgeOrder.begin(), edgeOrder.end());
                // std::cout << "Made permutation.\n";
                // auto seed = std::chrono::system_clock::now().time_since_epoch().count();
                // shuffle(edgeOrder.begin(), edgeOrder.end(), std::default_random_engine(seed));
            }

            std::string printCurrentKruskalOrder() {
                std::stringstream ss; 
                ss << "[ ";
                for( auto &it : this->edgeOrder ) { ss << it << " "; }
                ss << "]";
                return ss.str();
            }

            Graph generateKruskal() {
                this->getNextOrderPermutation();

                Graph genTree = Graph();
                genTree.numberOfVertices = this->originalGraph.numberOfVertices;
                genTree.numberOfEdges = this->originalGraph.numberOfEdges;

                int e = 0, i = 0; // index variables
                // subset *subsets = new subset[(genTree.numberOfVertices * sizeof(subset))];
                std::vector<subset> subsets(genTree.numberOfVertices + 2);

                for(int v = 0; v < genTree.numberOfVertices - 1; v++) {
                    subsets[v].parent = v;
                    subsets[v].rank = 0;
                }

                while( e < genTree.numberOfVertices && i < genTree.numberOfEdges) {
                    auto nextEdgeNumber = this->edgeOrder[i];
                    i += 1;
                    // std::cout << "Next edge of the list: " << nextEdgeNumber << std::endl;
                    Edge nextEdge = this->originalGraph.edges[nextEdgeNumber];
                    // std::cout << "Working..." << std::endl;

                    int x = find(subsets, nextEdge.v1);
                    int y = find(subsets, nextEdge.v2);

                    if(x != y) {
                        e++;
                        genTree.edges.push_back(nextEdge);
                        // std::cout << subsets.capacity() << std::endl;
                        Union(subsets, x, y);
                    }
                }

                genTree.updateNumbers();

                // delete[] subsets;
                return genTree;
            }

            /*
               Graph generateKruskal() {
               Graph genTree = Graph();

               genTree.numberOfVertices = this->originalGraph.numberOfVertices;
               genTree.numberOfEdges = this->originalGraph.numberOfEdges;

            // Create spanning tree here // TODO PAULO

            return genTree;
            }
            */
    };
}

#endif
