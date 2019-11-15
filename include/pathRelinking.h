#ifndef _PATH_RELINLING_H
#define _PATH_RELINLING_H

// maybe i need those
#include <vector>
#include <list>
#include <algorithm>

#include "forest.h"

typedef struct Tree_s {
    int cost; // stub
    Edge edges;

    bool hasEdge(Edge edge);
} Tree;

typedef int Vertice;

/*
    Fluxo pensado:
        -> Obtem arvore original e destino
        -> Obtem a diferença simétrica das arvores
        -> Escolhe uma aresta da diferenca simetrica para adicionar na original
        -> Remove uma aresta do caminho dos dois vertices envolvidos na aresta escolhida (para não criar ciclo)
        -> retorna arvore com o path relinking

        // TODO: Pensar se só precisa retornar uma arvore ou mais de uma
        // Pensamento até o momento: Retornar melhor path relink encontrado entre as arvores
*/

class PathRelinking {
    private:
        Tree originalTree; 
        Tree targetTree;
    public:
        PathRelinking(Tree originalTree, Tree targetTree);

        // Get a list of `numberOfRelinks` tree's in the way
        std::list<Tree> relink(int numberOfRelinks);

    private:
        // internal funcs
        std::list<Edge> getSimmetryDifferenceEdges(Tree origin, Tree destiny) {
            std::list<Edge> accEdges;
            
            // Get all edges that are not in the intersec of both tree's
            for(auto &edge : origin.edges) {
                if( !destiny.hasEdge(edge) ) { accEdges.push_back(edge); }
            }

            for(auto &edge : destiny.edges) {
                if( !origin.hasEdge(edge) ) { accEdges.push_back(edge); }
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

#endif
