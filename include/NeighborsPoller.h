#ifndef _NEIGHBORS_POOLER_H_
#define _NEIGHBORS_POOLER_H_

#include <set>

class NeighborsPoller{
    typedef std::set<std::pair<costType, neighborType>>::iterator iterType;

    private:
        int maxSize;
        double skewFactor;
        std::set<std::pair<costType, neighborType>> elementsSet;
    public:

        NeighborsPoller(int _maxSize, double _skewFactor): maxSize(_maxSize), skewFactor(_skewFactor){}

        void add(neighborType &neighbor, costType cost){
            elementsSet.insert({cost, neighbor});

            if(elementsSet.size() > maxSize){
                elementsSet.erase(--elementsSet.end());
            }
        }

        neighborType get(){
            std::vector<double> probs;
            std::vector<iterType> iters;

            // printf("%d\n", elementsSet.size());

            for(iterType it = elementsSet.begin(); it != elementsSet.end(); ++it){
                // printf("%d\n", it->first);
                probs.push_back(1/it->first);
                iters.push_back(it);

            }

            RandomPoll rpoll(probs, skewFactor);
            rpoll.prepareProbs();
            int chosenIndex = rpoll.poll();

            return iters[chosenIndex]->second;
        }
};

#endif
