#ifndef _POOLER_H_
#define _POOLER_H_

class SolutionsPoller{
    typedef iterType set<Forest>::iterator;

    private:
        int maxSize;
        double skewValue;
    public:
        set<Forest> forestSet;

        SolutionsPooler(int _maxSize, double _skewValue): maxSize(_maxSize), skewValue(_skewValue){}

        void addSolution(Forest &solution){
            forestSet.insert(solution);

            if(forestSet.size() > maxSize){
                forestSet.erase(--forestSet.end());
            }
        }

        Forest getSolution(){
            vector<double> probs;
            vector<iterType> iters;

            for(iterType it = forestSet.begin(); it != forestSet.end(); ++it){
                probs.append(1/it->cost);
                iters.append(it);
            }

            RandomPoll rpoll(probs, skewValue);
            rpoll.prepareProbs();
            int chosenIndex = rpoll.poll();

            return *iters[chosenIndex];
        }
}




#endif
