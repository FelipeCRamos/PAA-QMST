#ifndef _EDGE_H_
#define _EDGE_H_

#include <vector>

class Edge{
    int u, v; // edge endpoints
    int linearCost; // linear cost of the edge
    std::vector<int> quadCosts; // vector with quadratic costs (change to array?)

    Edge(int _u, int _v, int _lc, std::vector<int> &_qc):
        u(_u), 
        v(_v), 
        linearCost(_lc), 
        quadCosts(_qc) {/* default */}

    bool operator<(const Edge& e) const {
        if(u == e.u) return v < e.v;
        return u < e.u;
    }

    bool operator==(const Edge& e) const {
        return u == e.u && v == e.v;
    }
    
};

#endif
