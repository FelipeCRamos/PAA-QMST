#include <vector>
#include <bitset>
#include <algorithm>

#include "PBLowerBound.h"

#define INF 100000
#define NBITS 100

#define numType int
#define edgeListType std::vector<std::pair<int,int>>
#define maskType std::bitset<NBITS>


class LPLowerBound{
    private:
        numType *piParameters;
        numType *fCosts;
        numType delta;
        PBLowerBound* pblb;
        int n, m;

    public:

        LPLowerBound(int _n, int _m, numType _delta = 30){
            n = _n, m = _m;
            pblb = new PBLowerBound(n, m);
            piParameters = new numType[m];
            fCosts = new numType[m];
            for(int i = 0; i < m; ++i) piParameters[i] = 0;
            delta = _delta;
        }

        ~LPLowerBound(){
            delete[] piParameters;
            delete pblb;
        }

        numType levelingProcedure(edgeListType edges, maskType &visited, maskType &chosen, \
             int **costs){

            numType cost;
            numType maxF = 0, minF = INF;
            numType phiGap = INF, lastPhiGap = INF;

            // printf("STARTING LEVELING PROCEDURE\n");
            while(phiGap > delta){
                cost = pblb->pb(edges, visited, chosen, piParameters, costs, fCosts);
                maxF = 0, minF = INF;

                for(int i = 0; i < m; ++i){
                    if(fCosts[i] != INF){
                        maxF = std::max(maxF, fCosts[i]);
                        minF = std::min(minF, fCosts[i]);
                    }
                    // printf("\tfCost[%d]: %d ", i, fCosts[i]);
                    piParameters[i] = piParameters[i] + (fCosts[i] / (n - 1));
                }
                phiGap = maxF - minF;
                // printf("\tcost: %lf phiGap: %lf\n", cost, phiGap);

                if(cost == INF) return INF;
                if(lastPhiGap = phiGap) break;

                // printf("\n");
                lastPhiGap = phiGap;
            }
            // printf("FINISHING LEVELING PROCEDURE\n");
            return cost;
        }
};
