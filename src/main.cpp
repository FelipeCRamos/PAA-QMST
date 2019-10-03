#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <utility>
#include <algorithm>
#include <map>

// - Project custom libs
#include "argParser.h"
#include "helpFn.h"
#include "fileParser.h"
#include "backtrack.h"
#include "branchBound.h"


#define PTAG "[AGMQ] "

int main(int argc, char **argv){

    // ------------------------------------------------------------------------
    // LEITURA DE ARGUMENTOS
    // ------------------------------------------------------------------------

    std::cout << "Bem vindo ao utilitário" << std::endl;
    std::string filename = parseFilename(argc, argv);

    if ( filename.empty() ) {
        std::cerr << error("Número incorreto de argumentos");
        return 1;
    }

    std::cout << PTAG << "Arquivo a ser lido: '" << filename << "'..." << std::endl;

    // ------------------------------------------------------------------------
    // PARSING
    // ------------------------------------------------------------------------

    Parser parser(filename);

    if(!parser.parse()){
        std::cout << PTAG << "Falha ao fazer o parsing do arquivo!" << std::endl;
    }

    std::cout << PTAG << "Parsing do arquivo feita com sucesso!" << std::endl;


    // recuperando parametros dos arquivos
    int n = parser.getN();
    int m = parser.getM();
    int **costs = parser.getCosts(); // Discutir com felipe
    std::vector<std::pair<int,int>> edges = parser.getEdges();


    // ------------------------------------------------------------------------
    // PARSING FEITO, GRAFO MONTADO
    // ------------------------------------------------------------------------

    BacktrackAlgorithm b(n, m, edges, costs);
    std::cout << b.backtrack() << std::endl;

    return 0;
}
