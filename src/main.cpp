#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <utility>
#include <algorithm>
#include <map>
#include <chrono>

// - Project custom libs
#include "argParser.h"
#include "helpFn.h"
#include "fileParser.h"
#include "backtrack.h"
#include "branchBound.h"
#include "tabuSearch.h"

#define PTAG "[AGMQ] "
#define debug false

int main(int argc, char **argv){

    // ------------------------------------------------------------------------
    // LEITURA DE ARGUMENTOS
    // ------------------------------------------------------------------------

    if(debug) std::cout << "Bem vindo ao utilitário" << std::endl;
    std::string filename = parseFilename(argc, argv);

    if ( filename.empty() ) {
        if(debug) std::cerr << error("Número incorreto de argumentos");
        return 1;
    }

    if(debug) std::cout << PTAG << "Arquivo a ser lido: '" << filename << "'..." << std::endl;

    // ------------------------------------------------------------------------
    // PARSING
    // ------------------------------------------------------------------------

    Parser parser(filename);

    if(!parser.parse()){
        if(debug) std::cout << PTAG << "Falha ao fazer o parsing do arquivo!" << std::endl;
    }

    if(debug) std::cout << PTAG << "Parsing do arquivo feita com sucesso!" << std::endl;

    // recuperando parametros dos arquivos
    int n = parser.getN();
    int m = parser.getM();
    int **costs = parser.getCosts(); // Discutir com felipe
    std::vector<std::pair<int,int>> edges = parser.getEdges();

    std::vector<Edge> allEdges;
    for(int i = 0; i < edges.size(); ++i){
        std::vector<int> quadCosts;
        for(int j = 0; j < edges.size(); ++j){
            if(j == i) quadCosts.push_back(0);
            else quadCosts.push_back(costs[i][j]);
        }
        int lc = costs[i][i];
        int u = edges[i].first;
        int v = edges[i].second;

        allEdges.push_back(Edge(u, v, lc, quadCosts));
    }



    // ------------------------------------------------------------------------
    // PARSING FEITO, GRAFO MONTADO
    // ------------------------------------------------------------------------


    // BBoundAlgorithm bb(n, m, edges, costs);
    //
    // auto bbStartTime = std::chrono::high_resolution_clock::now();
    // auto result = bb.bbound();
    // auto bbEndTime = std::chrono::high_resolution_clock::now();
    // auto bbElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(bbEndTime - bbStartTime);
    //
    // std::cout << "\nBranch and bound run stats: ";
    // std::cout << "n: " << n << "; m: " << m << "; solution: ";
    // std::cout << result << ";";
    // std::cout << "time (ms): " << bbElapsedTime.count() << ";";
    // std::cout << "visited states: " << bb.steps() << std::endl << std::endl;

    // BacktrackAlgorithm b(n, m, edges, costs);
    //
    // auto btStartTime = std::chrono::high_resolution_clock::now();
    // result = b.backtrack();
    // auto btEndTime = std::chrono::high_resolution_clock::now();
    // auto btElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(btEndTime - btStartTime);
    //
    // std::cout << "Backtrack run stats: ";
    // std::cout << "n: " << n << "; m: " << m << "; solution: ";
    // std::cout << result << ";";
    // std::cout << "time (ms): " << btElapsedTime.count() << ";";
    // std::cout << "visited states: " << b.steps() << std::endl << std::endl;
    TabuSearchParameters tsp;
    tsp.maxItrs = 10000;
    tsp.maxItrsNoImprove = n;
    tsp.numTopSols = n;
    tsp.numTopNeighbors = m;
    tsp.tabuTenureMin = 1;
    tsp.tabuTenureMax = (n * 0.4);
    tsp.skewFactor = 1.0;

    TabuSearch ts(n, m, tsp, allEdges);
    ts.run();




    return 0;
}
