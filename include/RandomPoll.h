class RandomPoll{
    vector<double> probs;
    double skewFactor;

    RandomPoll(vector<double> v, double sf){probs = v; skewFactor = sf;}

    void skew(){
        double totCost = 0;
        for(int i = 0; i < probs.size(); ++i){
            probs[i] = powf(probs[i], skewFactor);
            totCost += probs[i];
        }

        for(int i = 0; i < probs.size(); ++i)
            probs[i] /= totCost;
    }

    void intoProb(){
        // turning into a probability
        double totCost = 0;
        for(auto &x : v) totCost += x;
        for(int i = 0; i < v.size(); ++i) v[i] /= totCost;
    }

    void prepareProbs(){
        intoProb();
        skew();
    }

    int poll(){
        double flip = ((double) rand() / (RAND_MAX)) + 1;

        double acc = 0;
        for(int i = 0; i < probs.size(); ++i){
            if(acc + probs[i] >= flip) return i;
            acc += probs[i];
        }

        return probs.size() - 1;
    }
}
