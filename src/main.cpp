#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <utility>

// - Project custom libs
#include "argParser.h"
#include "helpFn.h"

// - Definições
typedef std::pair<int, int> Aresta;
typedef std::pair<Aresta, int> Custo;
typedef std::pair<Aresta, Aresta> ParAresta;
typedef std::pair<ParAresta, int> CustoQuad;

#define TESTFILE_E "n010d033C010c001Q010q001s-3i1.txt"
#define PTAG "[AGMQ] "

// Utils
#define Aresta(x, y) std::make_pair(x, y)
#define Custo(x, y, z) std::make_pair(std::make_pair(x, y), z)
#define CustosQuad(x, y, w, z, p) std::make_pair(std::make_pair(Aresta(x, y), Aresta(w, z)), p)

// "Maquina de estados"
enum Parameter { n, m, edges, c, q, done };

// -----------------------------------------------------------------------------
// SOBRECARGA DE ARESTA
std::ostream & operator<<(std::ostream &os, Aresta &a) {
    os << "(" << a.first << ", " << a.second << ")";
    return os;
}

// SOBRECARGA DE VETOR DE CUSTOS
std::ostream & operator<<(std::ostream &os, std::vector<Custo> &c) {
    for( auto &it : c )
        os << it.first << " ~ " << it.second << std::endl;
    return os;
}

// SOBRECARGA DE VETOR DE CUSTOQUAD
std::ostream & operator<<(std::ostream &os, std::vector<CustoQuad> &c) {
    for( auto &it : c ) {
        auto edges = it.first;
        auto peso = it.second;
        os << edges.first << " <> " << edges.second << " ~ " << peso << std::endl;
    }
    return os;
}
// -----------------------------------------------------------------------------

int main(int argc, char **argv){
    std::cout << "Bem vindo ao utilitário" << std::endl;
    std::string filename = parseFilename(argc, argv);

    if ( filename.empty() ) {
        std::cerr << error("Número incorreto de argumentos");
        return 1;
    }

    std::cout << PTAG << "Arquivo a ser lido: '" << filename << "'..." << std::endl;

    // ------------------------------------------------------------------------
    // Grafo
    // ------------------------------------------------------------------------
    std::vector<Custo> custos;
    std::vector<CustoQuad> custosQuad;
    //-------------------------------------------------------------------------

    try {
        std::ifstream file(filename);

        if( !file.is_open() ) { throw "Erro ao abrir arquivo, abortando!"; }

        std::cout << PTAG << "Arquivo aberto com sucesso!" << std::endl;

        Parameter currState = n;
        char line[30];

        int infoN, infoM;   // são uteis?
        bool ended = false;

        while( file.good() && !ended ) {
            std::string word;
            file.getline(line, 30);
            if(line[0] == '#') {
                switch(line[2]) {   // "maquina de estados"
                    case 'n': currState = n; break;
                    case 'm': currState = m; break;
                    case 'c': currState = c; break;
                    case 'q': currState = q; break;
                    case 'E': currState = edges; break;
                    case 'e': currState = done; break;
                }
            } else {
                // check state
                std::stringstream buf;
                buf << line;

                switch(currState) {
                    case n:
                        buf >> infoN;
                        // std::cout << "infoN: " << infoN << std::endl;
                        break;

                    case m:
                        buf >> infoM;
                        // std::cout << "infoM: " << infoM << std::endl;
                        break;

                    case edges:

                        int e1, e2;
                        buf >> e1 >> e2;
                        // std::cout << "e1: " << e1 << " | " << "e2: " << e2 << std::endl;
                        // custos.push_back(Custo(e1, e2, 0));
                        break;

                    case c:
                        int c1, c2, c3;
                        buf >> c1 >> c2 >> c3;
                        // std::cout << "c1: " << c1 << " | " << "c2: " << c2 << " | "
                            // << "c3: " << c3 << std::endl;
                        custos.push_back(Custo(c1, c2, c3));
                        break;

                    case q:
                        int p1, p12, p2, p21, peso;
                        buf >> p1 >> p12 >> p2 >> p21 >> peso;
                        /*
                        std::cout 
                            << "p1: " << p1 << " | " 
                            << "p12: " << p12 << " | "
                            << "p2: " << p2 << " | "
                            << "p21: " << p21 << " | "
                            << "peso: " << peso << std::endl;
                        */
                        custosQuad.push_back(CustosQuad(p1, p12, p2, p21, peso));
                        break;

                    case done:
                        ended = true;
                        break;

                    default:
                        break;
                }
            }
        }

        file.close();
    } catch(const char *errorMsg) {
        std::cerr << PTAG << errorMsg << std::endl;
        return 1; 
    }

    // ------------------------------------------------------------------------
    // GRAFO POPULADO!  
    // ------------------------------------------------------------------------
    // usar custos e custosQuad

    // Grafo pronto para uso!
    std::cout << custos << std::endl;
    std::cout << custosQuad << std::endl;

    return 0;
}
