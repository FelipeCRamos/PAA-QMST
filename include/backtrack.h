#ifndef _BACKTRACK_H_
#define _BACKTRACK_H_

#include <vector>
#include <algorithm>

#include "unionFind.h"


class BacktrackAlgorithm{
private:
    const int INF = 1000000; // valor infinito para
    int **_costs; // matriz de custos
    std::vector<int> placed; // lista de arestas sendo utilizadas no backtrack
    std::vector<std::pair<int,int>> _edges; // lista com todas as arestas
    int _m, _n; // qntd de arestas e de nos
    UnionFind *ufind; // estrutura union find

    int _backtrack(int i, int cost){

        // em backtrack(i, cost) decido se ponho a aresta i na arvore
        // eventualmente, pode ser que i = m se eu pegar a m-1 esima aresta

        // se fechei a arvore, retorno o custo acumulado
        if(placed.size() == _n - 1) return cost;

        // se cheguei ao final sem fechar arvore, estou num pos invalida
        if(i == _m) return INF;

        // _m - placed.size(): qnts arestas ainda me restam para por
        //  _n - 1 - placed.size(): qnts arestas eu preciso por para formar arvore
        // se eu preciso por mais do que eu tenho, estou num pos invalida
        if(_m - placed.size() < _n - 1 - placed.size()) return INF;


        int ans = INF; // resposta para esse no do backtrack

        int v = _edges[i].first; // vertice numa extremidade
        int u = _edges[i].second; // vertice noutra extremidade



        if(ufind->find(u) != ufind->find(v)){ // posso adicionar sem criar ciclos?

            int lin_cost = _costs[i][i]; // custo linear

            int quad_costs = 0; // custo quadratico
            for( auto p : placed ) quad_costs += _costs[p][i] * 2;

            placed.push_back(i); // adiciono a aresta na lista de adicionadas
            ufind->join(v, u); // junto os vertices

            // visito o prox vertice adicionando essa aresta e checo se a resp melhorou
            ans = std::min(ans, _backtrack(i + 1, cost + lin_cost + quad_costs));

            placed.pop_back(); // retiro da lista de arestas adicionadas
            ufind->rollback(); // desfaco a conexao do ufind
        }

        // visisto o prox vertice sem adicionar essa aresta e checo se a resposta melhorou
        return std::min(ans, _backtrack(i + 1, cost));
    }

public:
    BacktrackAlgorithm(int n, int m, std::vector<std::pair<int,int>> &edges, int **costs){
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

    ~BacktrackAlgorithm(){
        for(int i = 0; i < _m; ++i) delete[] _costs[i];
        delete[] _costs;
        delete ufind;
    }

    int backtrack(){
        return _backtrack(0, 0);
    }
};

#endif
