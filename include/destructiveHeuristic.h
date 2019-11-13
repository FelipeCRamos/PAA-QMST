#ifndef _DESTRUCTIVE_H_
#define _DESTRUCTIVE_H_

#include "edge.h"
#include "forest.h"

class DestructiveHeuristic{

    DestructiveHeuristic(){}

    void desctruct(Forest &forest, int numRemovals){
        // recuperar todas as arestas que estao na arvore
        while(numRemovals--){
            // por arestas recuperadas numa lista, ponderadas pelo custo medio
            // nao preciso refazer o vetor, eh so por valor = 0 de quem ja saiu
            RandomPoll rp(???????????????????);
            rp.prepareProbs();
            int chosenIndex = rp.poll();
            forest.removeEdge(????????[chosenIndex]);
        }
    }

};

#endif
