#ifndef _BBOUND_H_
#define _BBOUND_H_

#include <vector>
#include <algorithm>
#include <bitset>
#include <queue>

#include "unionFind.h"

#define NBITS 100


// TODO
// SABER COMO CALCULAR O LOWER_BOUND DO STATE
// PRA ENTAO USAR ISSO COMO UM ATRIBUTO/METODO DA STRUCT
// E ENTAO PODER USAR ISSO NA PRIORITY QUEUE
// CHECAR SE ESTADO VAI GERAR CICLO PRECISA DO UNION FIND PRA CADA ESTADO
    // FAZER DFS OU ALGO DO TIPO (UFIND) PRA VER SE GERA CICLO

struct State{
    int nextEdge;
    int cost;
    std::bitset<NBITS> mask;

    State(int ne, int c, std::bitset<NBITS> bmask){
        nextEdge = ne;
        cost = c;
        mask = bmask;
    }
};

bool operator <(const State & a, const State & b){
    if(a.cost == b.cost){
        if(a.mask.count() == b.mask.count()){
            return a.nextEdge > b.nextEdge;
        }
        return a.mask.count() > b.mask.count();
    }
    return a.cost < b.cost;
}

class BBoundAlgorithm{
private:
    const int INF = 1000000; // valor infinito para
    int **_costs; // matriz de custos
    std::vector<std::pair<int,int>> _edges; // lista com todas as arestas
    int _m, _n; // qntd de arestas e de nos
    UnionFind *ufind; // estrutura union find


    int _bbound(){
        int upper_bound = INF;
        std::priority_queue<std::pair<int, State > > pq;

        State initalState = State(0, 0, std::bitmask<NBITS>());

        pq.push({0, initalState});

        while(!pq.empty()){
            State currentState = pq.top(); // recuperando estado atual
            pq.pop(); // removendo estado recuperado da pq

            // nao preciso continuar nesse no se ja uma solucao melhor
            if(currentState.cost > upper_bound) continue;

            // se formei uma arvore, atualizo o upper bound se obtive melhor result
            if(currentState.mask.count() == _n - 1)
                upper_bound = min(upper_bound, currentState.cost);

            // se nao tenho mais quem botar e nao completei arvore, cheguei num dead end
            if(currentState.nextEdge == m) continue;

            // _m - currentState.nextEdge: quantos arestas consigo por no max
            // _n - 1 - currentState.mask.count(): quantos arestas ainda preciso
            if(_m - currentState.nextEdge < _n - 1 - currentState.mask.count()) return INF;


            int v = _edges[currentState.nextEdge].first; // vertice numa extremidade
            int u = _edges[currentState.nextEdge].second; // vertice noutra extremidade

            pq.push({0, State(0,0, currentState.mask)}); // TODO

            if(ufind->find(u) != ufind->find(v)){ // posso adicionar sem criar ciclos?
                int lin_cost = _costs[currentState.nextEdge][currentState.nextEdge]; // custo linear

                int quad_costs = 0; // custo quadratico
                for( auto p : placed ) quad_costs += _costs[p][currentState.nextEdge] * 2;

                currentState.mask.set(currentState.nextEdge); // ativando aresta atual na bitmask

                ufind->join(v, u); // ERRADO

                // ponha o estado gerado na fila
                pq.push({0, State(0, 0, currentState.mask)}); // TODO
            }

            ans = std::min(ans, _backtrack(i + 1, cost));


        }

        return upper_bound;
    }

public:
    BBoundAlgorithm(int n, int m, std::vector<std::pair<int,int>> &edges, int **costs){
        _edges = edges;
        _n = n;
        _m = m;

        _costs = new int*[_m];
        for(int i = 0; i < _m; ++i) _costs[i] = new int[_m];

        for(int i = 0; i < _m; ++i)
            for(int j = 0; j < _m; ++j)
                _costs[i][j] = costs[i][j];

        ufind = new UnionFind(n+1);
    }

    ~BBoundAlgorithm(){
        for(int i = 0; i < _m; ++i) delete[] _costs[i];
        delete[] _costs;
        delete ufind;
    }

    int bbound(){
        return _bbound();
    }
};

#endif
