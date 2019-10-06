#ifndef _PB_LOWER_BOUND_H_
#define _PB_LOWER_BOUND_H_

#include <vector>
#include <algorithm>

#include "unionFind.h"

#define INF 100000
#define NBITS 100

#define kruskalListType std::vector<std::pair<double,int>>
#define edgeListType std::vector<std::pair<int,int>>
#define maskType std::bitset<NBITS>

bool cmp(std::pair<double,int> &a, std::pair<double,int> &b){
    if(a.first == b.first){
        return a.second < b.second;
    }else return a < b;
}

class PBLowerBound{


    private:
        int n, m;

        int kruskal(kruskalListType &kruskalList, edgeListType &edges, \
                    UnionFind *ufind, int &alreadyChosen, double &cost){

            std::sort(kruskalList.begin(), kruskalList.end(), cmp); // sorting kruskal list

            int chosenKruskal = 0; // edges chosen by kruskal
            for(int k = 0; k < kruskalList.size(); ++k){

                // retrieving edge and vertices and endpoints
                int edgeID = kruskalList[k].second;
                int u = edges[edgeID].first;
                int v = edges[edgeID].second;

                if(ufind->find(u) != ufind->find(v)){ // if not creating cycle
                    cost += kruskalList[k].first; // add cost
                    ufind->join(u, v); // add to the tree
                    // checking if tree is formed
                    if(alreadyChosen + ++chosenKruskal == n - 1) break;
                }
            }

            if(alreadyChosen + chosenKruskal == n - 1) return cost; // computed cost
            else return INF; // tree was not created. dont think that this will
                             // ever be true

        }

        int f_i(edgeListType &edges,  maskType visited, maskType chosen, \
             double *piParameters, int **costs, int i, UnionFind *ufind){

            // i and j were seted as in (11) of the paper

            // setting cost, already initializing with val. b_i(\pi) and using it at
            // union find
            double cost = costs[i][i] - (n - 2) * piParameters[i];
            ufind->join(edges[i].first, edges[i].second);

            // couting the already chosen edges. at the end, it must be that
            // chosen + already = n - 1
            int alreadyChosen = 0;
            // prepare union find, join vertices on already (visited and) chosen edges
            // and computing cost
            for(int j = 0; j < m; ++j){
                if(visited[j] && chosen[j]){ // i was not visited, so i \neq j here
                    ufind->join(edges[j].first, edges[j].second);
                    cost += costs[i][j] + piParameters[j]; // a_{ij}(\pi)x_j
                    alreadyChosen++;
                }
            }

            // create kruskal vector to be sorted
            kruskalListType kruskalList;

            for(int j = 0; j < m; ++j){
                // only add edges that werent yet chosen on the bb algorithm
                if(!visited[j] && i != j){
                    kruskalList.push_back({costs[i][j] + piParameters[j] , j}); // a_{ij}(\pi)x_j
                }
            }

            return kruskal(kruskalList, edges, ufind, alreadyChosen, cost);
        }


    public:

        PBLowerBound(int _n, int _m){
            n = _n, m = _m;
        }

        ~PBLowerBound(){

        }

        std::pair<double,double> pb(edgeListType edges,  maskType &visited, \
             maskType &chosen,  double *piParameters, int **costs, double *fCosts){

            double maxF = 0, minF = INF;

            // computing f_i(\pi) costs
            for(int i = 0; i < m; ++i){
                UnionFind *ufind = new UnionFind(n + 1);
                fCosts[i] = f_i(edges, visited, chosen, piParameters, costs, i, ufind);
                maxF = std::max(maxF, fCosts[i]);
                minF = std::min(minF, fCosts[i]);
                delete ufind;
            }

            UnionFind *ufind = new UnionFind(n + 1);
            double cost = 0; // PB cost

            // couting the already chosen edges. at the end, it must be that
            // chosen + already = n - 1
            int alreadyChosen = 0;
            // prepare union find, join vertices on already (visited and) chosen edges
            // and computing cost
            for(int i = 0; i < m; ++i){
                if(visited[i] && chosen[i]){
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

            double ans = kruskal(kruskalList, edges, ufind, alreadyChosen, cost);
            delete ufind;
            return {ans, maxF - minF};
        }

};

#endif
