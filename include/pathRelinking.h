#ifndef _PATH_RELINLING_H
#define _PATH_RELINLING_H

// maybe i need those
#include <vector>
#include <list>
#include <algorithm>
#include <sstream>

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

    class Tree {
        public:
            int numberOfVertices;
            int numberOfEdges;
            int cost;
            std::vector<Edge> edges;
            std::vector<QuadraticCost> quadraticCosts;

            Tree() {/* default */} // default constructor

            Tree(int numberOfVertices, int numberOfEdges, int **costs, std::vector<std::pair<int, int>> edges) {
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

            bool hasEdge(Vertice one, Vertice two) {
                return getCost(one, two) != -1;
            }
    };

    class PathRelinking {
        private:
            Tree originalTree; 
            Tree targetTree;
        public:
            PathRelinking() {}

            PathRelinking(Tree originalTree, Tree targetTree) {
                this->originalTree = originalTree;
                this->targetTree = targetTree;
            }

            // Get a list of `numberOfRelinks` tree's in the way
            std::list<Tree> relink(int numberOfRelinks) {}

        private:
            // internal funcs
            std::list<Edge> getSimmetryDifferenceEdges() {
                std::list<Edge> accEdges;
                
                // Get all edges that are not in the intersec of both tree's
                for(auto &edge : this->originalTree.edges) {
                    if( !this->targetTree.hasEdge(edge.v1, edge.v2) ) { accEdges.push_back(edge); }
                }

                for(auto &edge : this->targetTree.edges) {
                    if( !this->originalTree.hasEdge(edge.v1, edge.v2) ) { accEdges.push_back(edge); }
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

    Tree pathRelink(Tree original, Tree target) {
         
    };
}

#endif
