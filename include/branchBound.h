#ifndef _BBOUND_H_
#define _BBOUND_H_

#include <vector>
#include <algorithm>
#include <bitset>
#include <queue>

#include "unionFind.h"
#include "LPLowerBound.h"
#include "PBLowerBound.h"

#include "defines.h"

#define numType int
#define edgeListType std::vector<std::pair<int,int>>
#define maskType std::bitset<NBITS>

struct State{
    int nextEdge, cost, alreadyChosen;
    numType lowerBound;
    maskType visited, chosen;

    State(int ne, int c, maskType v, maskType chos, int ac){
        alreadyChosen = ac;
        nextEdge = ne;
        cost = c;
        chosen = chos;
        visited = v;
    }

    void computeLB(int n, int m, edgeListType &edges, int **costs, numType *piParameters, \
                        numType *fCosts, PBLowerBound *pblb){

        LPLowerBound lb(n, m, piParameters, fCosts, pblb);

        lowerBound = lb.levelingProcedure(edges, visited, chosen, costs);
    }
};

bool operator <(const State & a, const State & b){
    // melhor o no aquele que tem menor lower bound
    // em caso de empate, pego o no com mais vertices visitados
    // em caso de empate, pego o que tem a proxima aresta a ser proc com maior id
    if(a.lowerBound == b.lowerBound){
        if(a.visited.count() == b.visited.count()) return a.nextEdge > b.nextEdge;
        return a.visited.count() > b.visited.count();
    }
    return a.lowerBound < b.lowerBound;
}

class BBoundAlgorithm{
    private:
        int **_costs; // matriz de custos
        edgeListType _edges; // lista com todas as arestas
        int _m, _n; // qntd de arestas e de nos
        long int counter = 0;
        numType *piParameters;
        numType *fCosts;
        PBLowerBound* pblb;

        void prepareUFind(State & state, UnionFindNRB *ufind){
            for(int i = 0; i < _m; ++i){
                if(state.visited[i] && state.chosen[i]){
                    int v = _edges[i].first; // vertice numa extremidade
                    int u = _edges[i].second; // vertice noutra extremidade
                    ufind->join(u, v);
                }
            }
        }


        int _bbound(){
            int upper_bound = INF;
            std::priority_queue<State> pq;

            State initalState = State(0, 0, maskType(), maskType(), 0);
            initalState.computeLB(_n, _m, _edges, _costs, piParameters, fCosts, pblb);
            pq.push(initalState);


            UnionFindNRB *ufind = new UnionFindNRB(_n+1);
            while(!pq.empty()){
                counter++;
                State currentState = pq.top(); // recuperando estado atual
                pq.pop(); // removendo estado recuperado da pq

                // printf("currEdge: %d currCost: %d\n", currentState.nextEdge, currentState.cost);

                // nao preciso continuar nesse no se ja uma solucao melhor
                if(currentState.cost > upper_bound) continue;

                // se formei uma arvore, atualizo o upper bound se obtive melhor result
                if(currentState.alreadyChosen == _n - 1)
                    upper_bound = std::min(upper_bound, currentState.cost);

                // se nao tenho mais quem botar e nao completei arvore, cheguei num dead end
                if(currentState.nextEdge == _m) continue;

                // _m - currentState.nextEdge: quantos arestas consigo por no max
                // _n - 1 - currentState.mask.count(): quantos arestas ainda preciso
                if(_m - currentState.nextEdge < _n - 1 - currentState.alreadyChosen) return INF;

                // setting union find for checking for cycles
                ufind->reset();
                prepareUFind(currentState, ufind);

                // retrieving vertices of edge
                int v = _edges[currentState.nextEdge].first; // vertice numa extremidade
                int u = _edges[currentState.nextEdge].second; // vertice noutra extremidade

                // setting state of not adding the current edge
                State stateNotAddingEdge = State(currentState.nextEdge + 1, currentState.cost, currentState.visited, \
                        currentState.chosen, currentState.alreadyChosen);
                stateNotAddingEdge.visited[currentState.nextEdge] = 1; // visited the current edge
                stateNotAddingEdge.chosen[currentState.nextEdge] = 0; // did not add it to the tree
                stateNotAddingEdge.computeLB(_n, _m, _edges, _costs, piParameters, fCosts, pblb); // computing lower bound for the state

                if(stateNotAddingEdge.lowerBound != INF && stateNotAddingEdge.lowerBound < upper_bound)
                    pq.push(stateNotAddingEdge); // adding it to the queues

                if(ufind->find(u) != ufind->find(v)){ // posso adicionar sem criar ciclos?

                    // computing costs for state adding current edge
                    int lin_cost = _costs[currentState.nextEdge][currentState.nextEdge]; // custo linear
                    int quad_costs = 0; // custo quadratico
                    for(int i = 0; i < _m; ++i){
                        if(currentState.visited[i] && currentState.chosen[i])
                            quad_costs += _costs[i][currentState.nextEdge] * 2;
                    }

                    // setting state of adding the current edge
                    State stateAddingEdge = State(currentState.nextEdge + 1, currentState.cost + lin_cost + quad_costs, \
                            currentState.visited, currentState.chosen, \
                            currentState.alreadyChosen);

                    stateAddingEdge.visited[currentState.nextEdge] = 1; // visited the current edge
                    stateAddingEdge.alreadyChosen++; // visited the current edge
                    stateAddingEdge.chosen[currentState.nextEdge] = 1; // added it to the tree
                    stateAddingEdge.computeLB(_n, _m, _edges, _costs, piParameters, fCosts, pblb); // computing lower bound for the state


                    // printf("%d %d %d\n", currentState.nextEdge, stateAddingEdge.lowerBound, upper_bound );
                    if(stateAddingEdge.lowerBound != INF && stateAddingEdge.lowerBound < upper_bound)
                        pq.push(stateAddingEdge); // adding it to the queues
                }

            }

            delete ufind; // del ufind pointer
            return upper_bound;
        }

    public:
        BBoundAlgorithm(int n, int m, edgeListType &edges, int **costs){
            _edges = edges;
            _n = n;
            _m = m;

            pblb = new PBLowerBound(n, m);
            piParameters = new numType[m];
            fCosts = new numType[m];

            _costs = costs;
        }

        ~BBoundAlgorithm(){
            delete pblb;
            delete[] fCosts;
            delete[] piParameters;
        }

        int bbound(){
            return _bbound();
        }

        int steps(){
            return counter;
        }
};

#endif
