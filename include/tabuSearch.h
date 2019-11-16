/*

K. qntd das melhores configurações que serão salvas
H. qntd das melhores soluções achadas que serão salvas para escolha
L. elevar valores a esse fator
X. quantidade de passos para cada iteração

1. gerar solução usando A, a partir de uma árvore vazia
2. enquanto não tenha completado os X passos
    2.1. para toda aresta permitida a inserseção
        2.1.1. a aresta à árvore
        2.1.2. ache o ciclo formado
        2.1.3. calcule o valor da solução retirando as arestas do ciclo, salvando as top K
        2.1.4. salve as soluções que não são Tabu
    2.2. das soluções salvas, extraia as H primeiras
    2.3. calcule o somatório das H primeiras soluções, e defina uma função de probabilidade em função da contribuição da solução a esse somatório (elevado a um fator L)
    2.4. visite a solução escolhida aleatoriamente, volte para (2)
3. escolha uma das top K arvores, onde a função de probabilidade é definida em função da contribuição da solução ao somatório (elevado a um fator L)
4. faz SO nela, retirando rand() arestas
5. volta pra (1)
*/


class TabuSearch{
    // heuristics
    ConstructiveHeuristic constructiveHeuristic;
    DestructiveHeuristic destructiveHeuristic;

    // pollers
    SolutionsPoller solutionsPoller;
    NeighborsPoller neighborsPoller;
    double skewFactorEdges;
    int numTopSols, numTopNeighbors;

    // tabu tenures logic
    iterationType tabuTenureInMin, tabuTenureInMax, tabuTenureIn;
    iterationType tabuTenureOutMin, tabuTenureOutMax, tabuTenureOut;
    std::vector<iterationType> inLog, outLog;

    // algorithm limits
    iterationType maxItrs, maxItrsNoImprove;

    TabuSearch(iterationType _maxItrs, iterationType _maxItrsNoImprove, double _skewFactorEdges, int _numTopSols, int _numTopNeighbors){
        maxItrs = _maxItrs;
        maxItrsNoImprove = _maxItrsNoImprove;
        skewFactorEdges = _skewFactorEdges;
        numTopSols = _numTopSols;
        numTopNeighbors = _numTopNeighbors;
    }

    void prepSearch(){
        inLog = vector<iterationType>(m, 0); // init tabu in marker
        outLog = vector<iterationType>(m, 0); // init tabu out marker
        // tabu tenure value for getting in
        tabuTenureIn = (tabuTenureInMin + tabuTenureInMax) / 2;
        // tabu tenure value for leaving
        tabuTenureOut = (tabuTenureOutMin + tabuTenureOutMax) / 2;
    }

    Forest run(){

        // tree that is going to be used for searching
        Forest forest;
        constructiveHeuristic.construct(forest);

        prepSearch(); // init tabu search parameters

        Forest bestSolution = forest; // best seen solution

        for(iterationType itr = 0; itr < maxItrs; ++itr){
            itrsNoImprove = 0; // started counting num of iters without improving

            while(itrsNoImprove < maxItrsNoImprove && itr < maxItrs){
                solutionsPoller.addSolution(forest); // add forest to poller
                maybeUpdtBest(forest, bestSolution); // maybe update best seen solution

                // neighbors vector
                vector<neighborType> neighborhood;
                // neighbors costs
                vector<costType> neighborhoodCosts;

                // best seen neighbor and neighbor going to be chosen
                neighborType nextNeighbor
                int bestNeighborIndex;

                // retrive neighbors
                forest.getNeighborhood(neighborhood, neighborhoodCosts, bestNeighborIndex);


                if(neighborhoodCosts[bestNeighborIndex] < forest.cost){ // troca de best neighbor eh melhor q melhor global
                    nextNeighbor = neighborhood[bestNeighborIndex];
                }else{
                    NeighborsPoller neighborsPoller(numTopNeighbors);
                    for(int i = 0; i < neighborhood.size(); ++i){
                        if(!isNeighborTabu(neighborhood[i])){
                            neighborsPoller.add({neighborhood[i], neighborhoodCosts[i]});
                        }
                    }
                    nextNeighboard = neighborsPoller.get();
                }

                costType lastCost = forest.cost; // retriving last cost
                forest.goToNeighbor(nextNeighboard); // change solution
                updateTabu(nextNeighbor, lastCost, forest, iteration); // updt tabu parameters
            }

            // leave if reached max of iters
            if(itr == maxItrs) break;

            // strategic oscillation:
            // get an already visited solution from poller
            forest = solutionsPoller.getSolution();
            // choose (uniformly) how many edges will be removed
            int remEdges = rand() % M;

            // desctruct than construct
            destructiveHeuristic.destruct(forest, remEdges);
            constructiveHeuristic.construct(forest);
        }

        return bestSolution;
    }

    bool isNeighborTabu(neighborType &neighbor, iterationType itr){
        return (itr <= tabuTenureIn + inLog[neighbor.second] || itr <= tabuTenureOut + outLog[neighbor.first]);
    }

    void updateTabu(neighborType neighborChosen, costType lastCost, Forest &forest, iterationType iteration, int &itrsNoImprove){
        if(forest.cost < lastCost && iteration > 0){
            // if we can decrease tabu tenure for in and out
            if(tabuTenureIn > tabuTenureInMin) tabuTenureIn--;
            if(tabuTenureOut > tabuTenureOutMin) tabuTenureOut--;

            itrsNoImprove = 0; // 0 iters without impring, just improved
        }else{
            // if we can increase tabu tenure for in and out
            if(tabuTenureIn < tabuTenureInMax) tabuTenureIn++;
            if(tabuTenureOut < tabuTenureOutMax) tabuTenureOut++;

            itrsNoImprove++; // increasing the num of iters without improving
        }

        // retrieving edges from the chosen neighbor
        edgeId removedEdge = neighborChosen.first;
        edgeId addedEdge = neighborChosen.second;
        inLog[addedEdge] = iteration;
        outLog[removedEdge] = iteration;
    }

    void maybeUpdtBest(Forest &forest, Forest &bestSolution){
        if(forest < bestSolution){
            bestSolution = forest;
        }
    }

}
