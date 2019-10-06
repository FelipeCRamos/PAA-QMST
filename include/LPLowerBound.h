

#include "PBLowerBound.h"


class LPLowerBound{
    int *piParameters;
    double _delta;
    PBLowerBound* pblb;

    LPLowerBound(int n, int m, double delta){
        pblb = new PBLowerBound(n, m);
        piParameters = new int[m];
        for(int i = 0; i < m; ++i) piParameters[i] = 0;

        _delta = delta;
    }

    ~LPLowerBound(){
        delete[] piParameters;
        delete pblb;
    }

    levelingProcedure(const std::vector<std::pair<int,int>> edges, \
         const std::bitset<NBITS> &visited, const std::bitset<NBITS> &chosen, \
         int **costs){

        double phiGap = INF;
        while(phiGap > delta){
            std::pair<int,int> retPB = pblb.pb(edges, visited, chosen, piParameters, costs);
            int cost = retPB.first;
            int compPhiGab = retPB.second;

            
        }

    }
}
