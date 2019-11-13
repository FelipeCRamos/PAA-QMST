#ifndef _POOLER_H_
#define _POOLER_H_

class SolutionsPooler{
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

        int choose(vector<pair<double, iterType>> &v){
            double flip = ((double) rand() / (RAND_MAX)) + 1;

            double acc = 0;
            for(int i = 0; i < probs.size(); ++i){
                if(acc + probs[i] >= flip) return i;
                acc += probs[i];
            }

            return probs.size() - 1;
        }

        void prepareProbs(vector<pair<double, iterType>> &probs){
            double totInverseCost = 0;

            for(iterType it = forestSet.begin(); it != forestSet.end(); ++it){
                probs.append(make_pair(1/it->cost, it));
                totInverseCost += 1/it->cost;
            }

            for(int i = 0; i < probs.size(); ++i){
                probs[i].first /= totInverseCost;
            }
        }

        void skewProbs(vector<pair<double, iterType>> &probs){
            double tot = 0;
            for(int i = 0; i < probs.size(); ++i){
                probs[i].first = powf(probs[i].first, skewValue);
                tot += probs[i].first;
            }

            for(int i = 0; i < probs.size(); ++i){
                probs[i].first /= tot;
            }
        }

        Forest getSolution(){
            vector<pair<double, iterType>> probs;
            prepareProbs(probs);
            skewProbs(probs);
            int chosenIndex = choose(probs);

            return *probs[chosenIndex].second;
        }
}




#endif
