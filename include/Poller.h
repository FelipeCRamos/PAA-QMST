#ifndef _POOLER_H_
#define _POOLER_H_

template < typename T >
class Poller{
    typedef iterType set<T>::iterator;

    private:
        int maxSize;
        double skewValue;
        set<T> elementsSet;
    public:

        SolutionsPooler(int _maxSize, double _skewValue): maxSize(_maxSize), skewValue(_skewValue){}

        void add(T &e){
            elementsSet.insert(e);

            if(elementsSet.size() > maxSize){
                elementsSet.erase(--elementsSet.end());
            }
        }

        void addFromVector(vector<T> ve){
            sort(ve.begin(), ve.end());
            for(auto &e : ve) add(e);
        }

        T get(){
            vector<double> probs;
            vector<iterType> iters;

            for(iterType it = elementsSet.begin(); it != elementsSet.end(); ++it){
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
