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



    // ------------------------------------------------------------------------
    // PARSING FEITO, GRAFO MONTADO
    // ------------------------------------------------------------------------

    BacktrackAlgorithm b(n, m, edges, costs);

    auto btStartTime = std::chrono::high_resolution_clock::now();
    auto result = b.backtrack();
    auto btEndTime = std::chrono::high_resolution_clock::now();
    auto btElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(btEndTime - btStartTime);

    std::cout << "Backtrack run stats: ";
    std::cout << n << ";" << m << ";";
    std::cout << result << ";";
    std::cout << "" << btElapsedTime.count() << ";";
    std::cout << "" << b.steps() << std::endl;

    BBoundAlgorithm bb(n, m, edges, costs);

    auto bbStartTime = std::chrono::high_resolution_clock::now();
    result = bb.bbound();
    auto bbEndTime = std::chrono::high_resolution_clock::now();
    auto bbElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(bbEndTime - bbStartTime);

    std::cout << "Branch and bound run stats: ";
    std::cout << n << ";" << m << ";";
    std::cout << result << ";";

    std::cout << "" << bbElapsedTime.count() << ";";
    std::cout << "" << bb.steps() << std::endl;

    return 0;
}
