#ifndef _PARTICLE_SWARM_H
#define _PARTICLE_SWARM_H

#include "forest.h"
#include "edge.h"
#include "pathRelink.h"
#include "tabuSearch.h"


struct ParticleSwarmParameters{
    int maxItrs, numParticles;
    double probPRBestLocalStart, probPRBestGlobalStart, probLocalSearchStart;
    TabuSearchParameters tabuSearchParameters;
    PathRelinkingParameters pathRelinkingParameters;
    double skewFactorConstructiveHeuristic;
};

class ParticleSwarm{
private:
    int N, M;
    std::vector<Edge> availableEdges;

    std::random_device rd;
    std::mt19937 mt;
    std::uniform_real_distribution<double> distUpdate;

    //
    int maxItrs, numParticles;
    double probPRBestLocalStart, probPRBestGlobalStart, probLocalSearchStart;
    PathRelinkingParameters pathRelinkingParameters;
    TabuSearchParameters tabuSearchParameters;
    double skewFactorConstructiveHeuristic;

    void maybeUpdtBest(Forest &forest, Forest &bestSolution){
        if(forest < bestSolution){
            bestSolution = forest;
        }
    }

    void updateProbabilities(double &probPRBestLocal, double &probPRBestGlobal, double &probLocalSearch){
        probLocalSearch *= 0.95;
        probPRBestGlobal *= std::fmin(distUpdate(mt), 1);
        probPRBestLocal = std::fmax(1 - (probPRBestGlobal + probLocalSearch), 0);
    }

    int chooseVel(double probPRBestLocal, double probPRBestGlobal, double probLocalSearch){
        std::uniform_real_distribution<double> distFlip(0.0, probPRBestLocal + probPRBestGlobal + probLocalSearch);

        while(true){
            double flip = distFlip(mt);
            // printf("%lf %lf %lf %lf\n", flip, probPRBestLocal, probPRBestGlobal, probLocalSearch);
            if(flip < probPRBestLocal) return 0;
            else if(flip < probPRBestLocal + probPRBestGlobal) return 1;
            else if(flip < probPRBestLocal + probPRBestGlobal + probLocalSearch) return 2;
        }
    }

public:
    ParticleSwarm(int n, int m, ParticleSwarmParameters particleSwarmParameters, std::vector<Edge> &allEdges){
        N = n;
        M = m;
        tabuSearchParameters = particleSwarmParameters.tabuSearchParameters;
        pathRelinkingParameters = particleSwarmParameters.pathRelinkingParameters;
        skewFactorConstructiveHeuristic = particleSwarmParameters.skewFactorConstructiveHeuristic;
        maxItrs = particleSwarmParameters.maxItrs;
        numParticles = particleSwarmParameters.numParticles;
        probPRBestLocalStart = particleSwarmParameters.probPRBestLocalStart;
        probPRBestGlobalStart = particleSwarmParameters.probPRBestGlobalStart;
        probLocalSearchStart = particleSwarmParameters.probLocalSearchStart;
        mt = std::mt19937(rd());
        distUpdate = std::uniform_real_distribution<double>(1.0, 1.1);
        availableEdges = allEdges;
    }

    Forest run(){
        ConstructiveHeuristic constructiveHeuristic(skewFactorConstructiveHeuristic);
        TabuSearch tabuSearch(N, M, tabuSearchParameters, availableEdges);
        PathRelinking pathRelinking(pathRelinkingParameters);

        std::vector<Forest> particles;
        std::vector<Forest> bestParticles;
        std::vector<double> probsPRBestLocal(numParticles, probPRBestLocalStart);
        std::vector<double> probsPRBestGlobal(numParticles, probPRBestGlobalStart);
        std::vector<double> probsLocalSearch(numParticles, probLocalSearchStart);

        Forest bestGlobal(N, M, availableEdges);
        for(int i = 0; i < numParticles; ++i){
            Forest f(N, M, availableEdges);
            constructiveHeuristic.construct(f, availableEdges);
            particles.push_back(f);
            bestParticles.push_back(f);
            if(i == 0) bestGlobal = f;
            else maybeUpdtBest(f, bestGlobal);
        }


        for(int itr = 0; itr < maxItrs; ++itr){
            printf("itr: %d\n", itr);
            for(int i = 0; i < numParticles; ++i){
                double probPRBestLocal = probsPRBestLocal[i];
                double probPRBestGlobal = probsPRBestGlobal[i];
                double probLocalSearch = probsLocalSearch[i];

                if(particles[i] == bestParticles[i]) probPRBestLocal = 0;
                if(particles[i] == bestGlobal) probPRBestGlobal = 0;

                int vel = chooseVel(probPRBestLocal, probPRBestGlobal, probLocalSearch);


                if(vel == 0){ // path relink com o melhor local
                    particles[i] = pathRelinking.run(particles[i], bestParticles[i], bestParticles[i].cost, availableEdges);
                }else if(vel == 1){ // path relink com o melhor global
                    particles[i] = pathRelinking.run(particles[i], bestGlobal, bestGlobal.cost, availableEdges);
                }else{ // busca local
                    particles[i] = tabuSearch.runLocal(particles[i]);
                }
                printf("particles[i].cost: %d vel: %d\n", particles[i].cost, vel);
                maybeUpdtBest(particles[i], bestGlobal);
                updateProbabilities(probsPRBestLocal[i], probsPRBestGlobal[i], probsLocalSearch[i]);
            }
        }
        std::cout << bestGlobal << std::endl;
        return bestGlobal;
    }
};

#endif
