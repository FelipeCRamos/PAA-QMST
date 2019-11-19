#ifndef _defines_h_
#define _defines_h_

#define NBITS 120
#define INF 1000000000
typedef std::pair<int, int> treeEdge;

typedef int edgeId;
typedef int vertexId;
typedef int costType;
typedef int neighborCost;
typedef std::pair<edgeId, edgeId> neighborType;
typedef std::pair<vertexId, edgeId> rootedTreeNode;
typedef int iterationType;
typedef std::chrono::duration<long int, std::ratio<1, 1000> > durationType;

#endif
