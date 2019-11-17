#ifndef _RANDOMPOLL_H_
#define _RANDOMPOLL_H_

#include <math.h>
#include <random>
#include <iostream>

class RandomPoll{
private:
    std::vector<double> probs;
    double skewFactor;
    bool null;
    std::random_device rd;
    std::mt19937 mt;
    std::uniform_real_distribution<double> dist;

    void skew(){
        double totCost = 0;
        for(int i = 0; i < probs.size(); ++i){
            probs[i] = powf(probs[i], skewFactor);
            totCost += probs[i];
        }

        for(int i = 0; i < probs.size(); ++i)
            probs[i] /= totCost;
    }

    double intoProb(){
        // turning into a probability
        double totCost = 0;
        for(auto &x : probs) totCost += x;

        if(totCost != 0){
            for(int i = 0; i < probs.size(); ++i)
                probs[i] /= totCost;
        }
        return totCost;
    }

public:
    RandomPoll(std::vector<double> v, double sf){
        probs = v; skewFactor = sf;
        mt = std::mt19937(rd());
        dist = std::uniform_real_distribution<double>(0.0, 1.0);
    }

    RandomPoll(std::vector<int> v, double sf){
        for(auto &x : v) probs.push_back((double) x);
        skewFactor = sf;
    }

    void prepareProbs(){
        double totCost = intoProb();
        if(totCost != 0) skew();
        null = (totCost == 0);
    }

    int poll(){
        double flip = dist(mt);

        // printf(">> %lf\n", flip);
        //
        // for(int i = 0; i < probs.size(); ++i){
        //     printf("%lf ", probs[i]);
        // }
        // printf("\n");

        double acc = 0;
        for(int i = 0; i < probs.size(); ++i){
            if(acc + probs[i] >= flip) return i;
            acc += probs[i];
        }

        return probs.size() - 1;
    }
};

#endif
