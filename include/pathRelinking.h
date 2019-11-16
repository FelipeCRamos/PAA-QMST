#ifndef _PATH_RELINLING_H
#define _PATH_RELINLING_H

// maybe i need those
#include <vector>
#include <list>
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
        Vertice v1;
        Vertice v2;
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
            std::list<Graph> relink(int numberOfRelinks) {}

        private:
            // internal funcs
            std::list<Edge> getSimmetryDifferenceEdges() {
                std::list<Edge> accEdges;
                
                // Get all edges that are not in the intersec of both tree's
                for(auto &edge : this->originalGraph.edges) {
                    if( !this->targetGraph.hasEdge(edge.v1, edge.v2) ) { accEdges.push_back(edge); }
                }

                for(auto &edge : this->targetGraph.edges) {
                    if( !this->originalGraph.hasEdge(edge.v1, edge.v2) ) { accEdges.push_back(edge); }
                }

                // and return them
                return accEdges;
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
            std::set<Graph> trees;

        public:
            ParticleSwarm(Graph originalGraph) {
                this->originalGraph = originalGraph;

                // generate initial order
                for(int i = 0; i < this->originalGraph.numberOfEdges; i++) {
                    this->edgeOrder.push_back(i);
                }

                std::cout << "[particleSwarm] Original graph stored!\n";
                std::cout << "[particleSwarm] Current Order:" << std::endl;
                std::cout << printCurrentKruskalOrder() << std::endl;

                // Generate the initial set of spanning trees
                // and then wait to run the algorithm
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

/*{{{*/
            /* TRY GFG
            Graph generateKruskal() {
                // this->getNextOrderPermutation();
                Graph genTree = Graph();
                genTree.numberOfVertices = this->originalGraph.numberOfVertices;
                genTree.numberOfEdges = this->originalGraph.numberOfEdges;

                int e = 0, i = 0; // index variables
                subset *subsets = new subset[(genTree.numberOfVertices * sizeof(subset))];

                for(int v = 0; v < genTree.numberOfVertices - 1; v++) {
                    subsets[v].parent = v;
                    subsets[v].rank = 0;
                }
                
                int whileCounter = 0;
                while( e < genTree.numberOfVertices && i < genTree.numberOfEdges) {
                    // std::cout << "Entered on while() loop " << whileCounter++ << " times.\n";
                    auto nextEdgeNumber = this->edgeOrder[i];
                    i += 1;
                    // std::cout << "Next edge of the list: " << nextEdgeNumber << std::endl;
                    Edge nextEdge = this->originalGraph.edges[nextEdgeNumber];
                    // std::cout << "Working..." << std::endl;

                    int x = find(subsets, nextEdge.v1);
                    int y = find(subsets, nextEdge.v2);

                    if(x != y) {
                        // result[e++] = nextEdge;
                        e++;
                        genTree.edges.push_back(nextEdge);
                        Union(subsets, x, y);
                    }
                    // else {
                        // std::cout << "x == y on edge: " << nextEdge.print() << std::endl;
                    // }
                }

                genTree.updateNumbers();

                // std::cout << "result: " << std::endl;
                // std::cout << genTree.print() << std::endl;
                delete[] subsets;
                return genTree;
            }
            */
            /*}}}*/

            Graph generateKruskal() {
                Graph genTree = Graph();

                genTree.numberOfVertices = this->originalGraph.numberOfVertices;
                genTree.numberOfEdges = this->originalGraph.numberOfEdges;

                // Create spanning tree here // TODO PAULO

                return genTree;
                // TEMPORARY WORKAROUND
                bool success = false;
                while(!success) {
                    // auto seed = std::chrono::system_clock::now().time_since_epoch().count();
                    // shuffle(edgeOrder.begin(), edgeOrder.end(), std::default_random_engine(seed));
                    this->getNextOrderPermutation();
                    // for( auto &it : edgeOrder ) { std::cout << it << ", "; } std::cout << std::endl;

                    // std::cout << "originalGraph.numberOfVertices() : " << originalGraph.numberOfVertices << std::endl;
                    for(int i = 0; i < originalGraph.numberOfVertices - 1; i++) {
                        genTree.edges.push_back(this->originalGraph.edges[edgeOrder[i++]]);
                    }

                    if(genTree.getNumberOfEdges() != this->originalGraph.numberOfVertices - 1) {
                        success = false;
                    } else {
                        success = true;
                        genTree.updateNumbers();
                    }
                }
                return genTree;
            }
    };
}

#endif
