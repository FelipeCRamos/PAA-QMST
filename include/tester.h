#include "tabuSearch.h"
#include "particleSwarm.h"
#include <string>

struct Results{
    int bestResult, countBestResult;
    double meanResult, meanElapsedTime;
};

class Tester{
private:
    int numItrs;

    void printResults(Results &resultParticleSwarm, Results &resultTabuSearch, std::string &fileName){
        std::cout << fileName << ";";
        std::cout << resultParticleSwarm.bestResult << ";";
        std::cout << resultParticleSwarm.countBestResult << ";";
        std::cout << resultParticleSwarm.meanResult << ";";
        std::cout << resultParticleSwarm.meanElapsedTime << ";";
        std::cout << resultTabuSearch.bestResult << ";";
        std::cout << resultTabuSearch.countBestResult << ";";
        std::cout << resultTabuSearch.meanResult << ";";
        std::cout << resultTabuSearch.meanElapsedTime << ";\n";
    }

    double getMeanInt(std::vector<int> &v){
        int sum = 0;
        for(int x : v) sum += x;
        return ((double) sum) / numItrs;
    }

    double getMeanDuration(std::vector<durationType> &v){
        durationType sum = durationType();
        for(auto x : v) sum += x;
        return ((double) sum.count()) / numItrs;
    }

public:
    Tester(int numIterations){
        numItrs = numIterations;
    }

    void run(ParticleSwarm &ps, TabuSearch &ts, std::string &fileName){
        // testing particle swarm

        int bestParticleSwarm = INF, bestTabuSearch = INF;
        std::vector<int> particleSwarmResults, tabuSearchResults;
        std::vector<durationType> particleSwarmElapsedTimes, tabuSearchElapsedTimes;

        for(int _ = 0; _ < numItrs; ++_){
            auto startTime = std::chrono::high_resolution_clock::now();
            auto result = ps.run().cost;
            auto endTime = std::chrono::high_resolution_clock::now();
            durationType elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

            particleSwarmElapsedTimes.push_back(elapsedTime);
            particleSwarmResults.push_back(result);
            bestParticleSwarm = std::min(result, bestParticleSwarm);
        }

        for(int _ = 0; _ < numItrs; ++_){
            auto startTime = std::chrono::high_resolution_clock::now();
            auto result = ts.run().cost;
            auto endTime = std::chrono::high_resolution_clock::now();
            durationType elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

            tabuSearchElapsedTimes.push_back(elapsedTime);
            tabuSearchResults.push_back(result);
            bestTabuSearch = std::min(result, bestTabuSearch);
        }

        int countBestParticleSwarm = 0;
        int countBestTabuSearch = 0;

        for(int i = 0; i < numItrs; ++i){
            if(particleSwarmResults[i] == bestParticleSwarm) countBestParticleSwarm++;
            if(tabuSearchResults[i] == bestTabuSearch) countBestTabuSearch++;
        }

        double meanParticleSwarmResults = getMeanInt(particleSwarmResults);
        double meanTabuSearchResults = getMeanInt(tabuSearchResults);

        double meanParticleSwarmElapsedTimes = getMeanDuration(particleSwarmElapsedTimes);
        double meanTabuSearchElapsedTimes = getMeanDuration(tabuSearchElapsedTimes);

        Results resultParticleSwarm;
        resultParticleSwarm.bestResult = bestParticleSwarm;
        resultParticleSwarm.countBestResult = countBestParticleSwarm;
        resultParticleSwarm.meanResult = meanParticleSwarmResults;
        resultParticleSwarm.meanElapsedTime = meanParticleSwarmElapsedTimes;

        Results resultTabuSearch;
        resultTabuSearch.bestResult = bestTabuSearch;
        resultTabuSearch.countBestResult = countBestTabuSearch;
        resultTabuSearch.meanResult = meanTabuSearchResults;
        resultTabuSearch.meanElapsedTime = meanTabuSearchElapsedTimes;


        resultTabuSearch.meanElapsedTime /= 1000.0;
        resultParticleSwarm.meanElapsedTime /= 1000.0;

        printResults(resultParticleSwarm, resultTabuSearch, fileName);


    }
};
