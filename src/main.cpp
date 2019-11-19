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
#include <random>

// - Project custom libs
#include "argParser.h"
#include "helpFn.h"
#include "fileParser.h"
#include "backtrack.h"
#include "branchBound.h"
#include "tabuSearch.h"
#include "particleSwarm.h"
#include "tester.h"

#define PTAG "[AGMQ] "
#define debug false

// typedef std::pair<int, int> treeEdge;

int main(int argc, char **argv){

    // ------------------------------------------------------------------------
    // LEITURA DE ARGUMENTOS
    // ------------------------------------------------------------------------

    if(debug) std::cout << "Bem vindo ao utilitário" << std::endl;
    std::string filename = parseFilename(argc, argv);

    if ( filename.empty() ) {
        if(debug) std::cerr << error("Número incorreto de argumentos");
        return 1; }

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

    TabuSearchParameters tsp;
    tsp.maxItrs = m*n;
    tsp.maxItrsNoImprove = n;
    tsp.numTopSols = n;
    tsp.numTopNeighbors = m;
    tsp.tabuTenureMin = 1;
    tsp.tabuTenureMax = (int) pow(n, (0.8));
    tsp.skewFactor = 1.0;

    TabuSearch ts(n, m, tsp, allEdges);

    TabuSearchParameters tspPs;
    tsp.maxItrs = (int) pow(m*n, (0.5));
    tsp.maxItrsNoImprove = n;
    tsp.numTopSols = n;
    tsp.numTopNeighbors = m;
    tsp.tabuTenureMin = 1;
    tsp.tabuTenureMax = (int) pow(n, (0.8));
    tsp.skewFactor = 1.0;

    PathRelinkingParameters prp;
    prp.skewFactor = 1.0;
    prp.numTopSols = n;

    ParticleSwarmParameters psp;

    psp.maxItrs = n;
    psp.numParticles = (int) pow(m*n, (0.5));
    psp.probPRBestLocalStart = 0.05;
    psp.probPRBestGlobalStart = 0.05;
    psp.probLocalSearchStart = 0.9;
    psp.probPRBestLocalDecay = powf(0.1/psp.probLocalSearchStart, (1.0/psp.maxItrs));
    psp.tabuSearchParameters = tspPs;
    psp.pathRelinkingParameters = prp;
    psp.skewFactorConstructiveHeuristic = 1.0;

    ParticleSwarm ps(n, m, psp, allEdges);

    Tester tester(30);
    tester.run(ps, ts, filename);

    return 0;
}
