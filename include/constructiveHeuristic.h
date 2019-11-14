#ifndef _CONSTRUCTIVE_H_
#define _CONSTRUCTIVE_H_

#include "edge.h"
#include "forest.h"
#include "unionFind.h"

class ConstructiveHeuristic{
private:
    void build(Forest &forest, UnionFind &uf){
        // para cada aresta em forest
            // faz a ligacao do union find
    }

public:

    ConstructiveHeuristic(){}

    void construct(Forest &forest, vector<Edge> &edges){
        UnionFind uf;
        // build do union find

        // listar todas as arestas que nao estao na arvore

        while(!forest.isSolution()){
            // nao preciso refazer o vetor, eh so por valor = 0 de quem ja saiu
            // ponderar pelo custo medio
            RandomPoll rp(???????????????????);
            rp.prepareProbs();
            int chosenIndex = rp.poll();
            forest.addEdge(????????[chosenIndex]);
        }
    }


};

#endif
