#include <vector>
#include <bitset>
#include <algorithm>

#include "PBLowerBound.h"

#define INF 100000
#define NBITS 100

#define edgeListType std::vector<std::pair<int,int>>
#define maskType std::bitset<NBITS>


class LPLowerBound{
    private:
        double *piParameters;
        double *fCosts;
        double delta;
        PBLowerBound* pblb;
        int n, m;

    public:

        LPLowerBound(int _n, int _m, double _delta = 0.1){
            n = _n, m = _m;
            pblb = new PBLowerBound(n, m);
            piParameters = new double[m];
            fCosts = new double[m];
            for(int i = 0; i < m; ++i) piParameters[i] = 0;
            delta = _delta;
        }

        ~LPLowerBound(){
            delete[] piParameters;
            delete pblb;
        }

        double levelingProcedure(edgeListType edges, maskType &visited, maskType &chosen, \
             int **costs){

            double phiGap = INF;
            double cost;

            while(phiGap > delta){
                std::pair<double,double> retPB = pblb->pb(edges, visited, chosen, piParameters, costs, fCosts);
                cost = retPB.first;
                phiGap = retPB.second;

                for(int i = 0; i < m; ++i){
                    piParameters[i] = piParameters[i] + (1.0 / (n - 1)) * fCosts[i];
                }
            }
            return cost;
        }
};
