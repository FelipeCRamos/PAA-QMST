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
    ConstructiveHeuristic constructiveHeuristic;
    DestructiveHeuristic destructiveHeuristic;
    SolutionsPoller solutionsPoller;
    int limit;
    int noUpdtCount;
    int totLoops;
    double skewFactorEdges;

    int tabuTenureInMin, tabuTenureInMax, tabuTenureIn;
    int tabuTenureOutMin, tabuTenureOutMax, tabuTenureOut;

    vector<int> inLog, outLog;

    Forest bestSolution;

    int bestNeighbours;


    TabuSearch(){
        noUpdtCount = 0;
        limit = ?;
        totLoops = ?;

        tabuTenureIn = (tabuTenureInMin + tabuTenureInMax) / 2;
        tabuTenureOut = (tabuTenureOutMin + tabuTenureOutMax) / 2;
    }

    void run(){
        Forest forest;
        constructiveHeuristic.construct(forest);

        for(int loopNum = 0; loopNum < totLoops; ++loopNum){
            noUpdtCount = 0;
            while(noUpdtCount < limit){
                solutionsPoller.addSolution(forest);
                vector<pair<int,int>> neighbourhood;
                vector<int> neighbourhoodCosts;
                pair<int,int> bestNeighboard;
                pair<int, int> nextNeighboard;
                forest.getNeighbourhood(neighbourhood, neighbourhoodCosts, bestNeighboard);

                if(){ // troca de best neighbourd eh melhor q melhor global
                    nextNeighboard = bestNeighboard;
                }else{
                    for(int i = 0; i < neighbourhood.size(); ++i){
                        // selecionar os bestNeighbours melhores vizinhos
                    }

                    RandomPoll rp(neighbourhoodCosts, skewFactorEdges);
                    rp.prepareProbs();
                    int chosenIndex = rp.poll();
                    nextNeighboard = neighbourhood[chosenIndex];
                }

                forest.goToNeighbour(nextNeighboard);
                updateTabu();
            }
            forest = solutionsPoller.getSolution();

            int remEdges = rand() % M;
            destructiveHeuristic.destruct(forest, remEdges);
            constructiveHeuristic.construct(forest);
        }
    }

    void updateTabu(){
        
    }

    void maybeUpdtBest(Forest *forest){
        // todo
    }

}
