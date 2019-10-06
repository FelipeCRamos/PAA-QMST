#ifndef _PB_LOWER_BOUND_H_
#define _PB_LOWER_BOUND_H_

#include <vector>
#include <algorithm>

#include "unionFind.h"

#include "defines.h"

#define numType int
#define kruskalListType std::vector<std::pair<numType,int>>
#define edgeListType std::vector<std::pair<int,int>>
#define maskType std::bitset<NBITS>

bool cmp(std::pair<numType,int> &a, std::pair<numType,int> &b){
    if(a.first == b.first){
        return a.second < b.second;
    }else return a < b;
}

class PBLowerBound{


    private:
        int n, m;

        int kruskal(kruskalListType &kruskalList, edgeListType &edges, \
                    UnionFindNRB *ufind, int &alreadyChosen, numType &cost){

            std::sort(kruskalList.begin(), kruskalList.end(), cmp); // sorting kruskal list

            int chosenKruskal = 0; // edges chosen by kruskal
            for(int k = 0; k < kruskalList.size(); ++k){

                // retrieving edge and vertices and endpoints
                int edgeID = kruskalList[k].second;
                int u = edges[edgeID].first;
                int v = edges[edgeID].second;

                // printf("%d %d %d\n", u, v, edgeID);
                if(ufind->find(u) != ufind->find(v)){ // if not creating cycle
                    // printf(">> %d %d %d\n", u, v, edgeID);
                    cost += kruskalList[k].first; // add cost
                    ufind->join(u, v); // add to the tree
                    // checking if tree is formed
                    chosenKruskal++;
                    // if(alreadyChosen + chosenKruskal == n - 1) break;
                }
            }

            // printf("%d %d\n\n\n", alreadyChosen, chosenKruskal);


            if(alreadyChosen + chosenKruskal == n - 1) return cost; // computed cost
            else return INF; // tree was not created. dont think that this will
                             // ever be true

        }

        numType f_i(edgeListType &edges,  maskType visited, maskType chosen, \
             numType *piParameters, int **costs, int i, UnionFindNRB *ufind){

            // i and j were seted as in (11) of the paper

            // setting cost, already initializing with val. b_i(\pi) and using it at
            // union find
            numType cost = costs[i][i] - (n - 2) * piParameters[i];
            ufind->join(edges[i].first, edges[i].second);
            // printf("adicionando a aresta %d (%d, %d) a arvore \n", i, edges[i].first, edges[i].second);

            // couting the already chosen edges. at the end, it must be that
            // chosen + already = n - 1
            int alreadyChosen = 1;
            // prepare union find, join vertices on already (visited and) chosen edges
            // and computing cost

            // create kruskal vector to be sorted
            kruskalListType kruskalList;
            for(int j = 0; j < m; ++j){
                if(i == j) continue;

                if(visited[j]){ // i was not visited, so i \neq j here
                    if(chosen[j]){
                        // printf("%d: (%d, %d) ja foi adicionada na arvore\n", j, edges[j].first, edges[j].second);
                        if(ufind->find(edges[j].first) == ufind->find(edges[j].second)) return INF;
                        ufind->join(edges[j].first, edges[j].second);
                        cost += costs[i][j] + piParameters[j]; // a_{ij}(\pi)x_j
                        alreadyChosen++;
                    }else{
                        // printf("%d: (%d, %d) nunca sera adicionada na arvore\n", j, edges[j].first, edges[j].second);
                    }
                }else{
                    // only add edges that werent yet chosen on the bb algorithm
                    // printf("a aresta %d nao foi processada ainda\n", j);
                    kruskalList.push_back({costs[i][j] + piParameters[j] , j}); // a_{ij}(\pi)x_j
                }
            }
            // printf("\n\n\n\n\n");

            // printf("\n\n\n%d\n", kruskalList.size());

            return kruskal(kruskalList, edges, ufind, alreadyChosen, cost);
        }


    public:

        PBLowerBound(int _n, int _m){
            n = _n, m = _m;
        }

        ~PBLowerBound(){

        }

        numType pb(edgeListType edges,  maskType &visited,  maskType &chosen, \
                    numType *piParameters, int **costs, numType *fCosts){

            // computing f_i(\pi) costs
            UnionFindNRB *ufind = new UnionFindNRB(n + 1);
            for(int i = 0; i < m; ++i){
                ufind->reset();
                if(visited[i] && !chosen[i]){
                    fCosts[i] = INF;
                }else{
                    fCosts[i] = f_i(edges, visited, chosen, piParameters, costs, i, ufind);
                }
                // printf("f(%d): %.3lf ", i, fCosts[i]);
            }
            // printf("\n");

            ufind->reset();
            numType cost = 0; // PB cost

            // couting the already chosen edges. at the end, it must be that
            // chosen + already = n - 1
            int alreadyChosen = 0;
            // prepare union find, join vertices on already (visited and) chosen edges
            // and computing cost
            for(int i = 0; i < m; ++i){
                if(visited[i] && chosen[i]){
                    // se to add uma aresta na arvore com a qual n formo arvore alguma
                    if(fCosts[i] == INF){
                        // printf("Adicionando aresta %d com custo de producao inf\n", i);
                        delete ufind;
                        return INF;
                    }

                    ufind->join(edges[i].first, edges[i].second);
                    cost += fCosts[i];
                    alreadyChosen++;
                }
            }

            // create kruskal vector to be sorted
            kruskalListType kruskalList;

            for(int j = 0; j < m; ++j){
                // only add edges that werent yet chosen on the bb algorithm
                if(!visited[j]){
                    kruskalList.push_back({fCosts[j], j});
                }
            }

            numType ans = kruskal(kruskalList, edges, ufind, alreadyChosen, cost);
            delete ufind;
            return ans;
        }

};

#endif
