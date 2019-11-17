#ifndef _SOLS_POOLER_H_
#define _SOLS_POOLER_H_

#include <set>

class SolutionsPoller{
    typedef std::set<Forest>::iterator iterType;

    private:
        int maxSize;
        double skewFactor;
    public:
        std::set<Forest> forestSet;

        SolutionsPoller(int _maxSize, double _skewFactor): maxSize(_maxSize), skewFactor(_skewFactor){}

        void addSolution(Forest &solution){
            forestSet.insert(solution);

            if(forestSet.size() > maxSize){
                forestSet.erase(--forestSet.end());
            }
        }

        Forest getSolution(){
            std::vector<double> probs;
            std::vector<iterType> iters;

            for(iterType it = forestSet.begin(); it != forestSet.end(); ++it){
                probs.push_back(1/it->cost);
                iters.push_back(it);
            }

            RandomPoll rpoll(probs, skewFactor);
            rpoll.prepareProbs();
            int chosenIndex = rpoll.poll();

            return *iters[chosenIndex];
        }
};




#endif
