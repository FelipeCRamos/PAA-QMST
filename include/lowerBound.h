#ifndef _KRUSKAL_H_
#define _KRUSKAL_H_

#include <vector>
#include <algorithm>

#include "unionFind.h"

#define INF 100000

class LowerBound{
    private:
        int n, m;

        int kruskal(std::vector<std::pair<int,int>> &kruskalList, \
                    const std::vector<std::pair<int,int>> &edges, UnionFind *ufind,
                    int alreadyChosen){

            sort(kruskalList.begin(), kruskalList.end()); // sorting kruskal list

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

        int f_i(const std::vector<std::pair<int,int>> &edges, \
             const std::bitset<NBITS> visited, const std::bitset<NBITS> chosen, \
             int *piParameters, int **costs, int i, UnionFind *ufind){

            // i and j were seted as in (11) of the paper

            // setting cost, already initializing with val. b_i(\pi) and using it at
            // union find
            int cost = costs[i][i] - (n - 2) * piParameters[i];
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
            std::vector<std::pair<int,int>> kruskalList;

            for(int j = 0; j < m; ++j){
                // only add edges that werent yet chosen on the bb algorithm
                if(!visited[j] && i != j){
                    kruskalList.push_back({costs[i][j] + piParameters[j] , j}); // a_{ij}(\pi)x_j
                }
            }

            return kruskal(kruskalList, edges, ufind, alreadyChosen);
        }


    public:

        LowerBound(int _n, int _m){
            n = _n, m = _m;
        }

        ~LowerBound(){

        }

        int pb(const std::vector<std::pair<int,int>> edges, \
             const std::bitset<NBITS> &visited, const std::bitset<NBITS> &chosen, \
             int *piParameters, int **costs){

            // computing f_i(\pi) costs
            int fCosts = new int[m];
            for(int i = 0; i < m; ++i){
                UnionFind ufind = new UnionFind(n + 1);
                fCosts[i] = f_i(edges, visited, chosen, piParameters, costs, i, ufind);
                delete ufind;
            }

            UnionFind ufind = new UnionFind(n + 1);
            int cost = 0; // PB cost

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
            std::vector<std::pair<int,int>> kruskalList;

            for(int i = 0; i < m; ++i){
                // only add edges that werent yet chosen on the bb algorithm
                if(!visited[j]){
                    kruskalList.push_back({fCosts[j], j});
                }
            }

            int ans = kruskal(kruskalList, edges, ufind, alreadyChosen);
            delete ufind;
            return ans;
        }

};

#endif
