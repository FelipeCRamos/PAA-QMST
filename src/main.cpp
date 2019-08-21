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

// UNION FIND PARA O KRUSKAL

// INICIALIZACAO DO UNION FIND
void init(size_t n, size_t parent[], size_t size[]){
    for(int i = 0; i <= n; ++i){
        parent[i] = i;
        size[i] = 1;
    }
}

// OPERACAO FIND
size_t find(size_t cur, size_t parent[]){ // Find do union find com comp de caminho
    size_t temp, root = cur;

    // Procurando a raiz
    while(parent[root] != root)
        root = parent[root];

    // Vou a caminho da raiz
    while(parent[cur] != cur){
        temp = parent[cur]; // backup do pai do current
        parent[cur] = root; // o novo pai do current eh a raiz
        cur = temp; // subo pro pai no backup
    }

    return root;
}

// OPERACAO JOIN
void join(size_t x, size_t y, size_t parent[], size_t size[]){ // Union do union find
    // acho as respectivas raizes
    x = find(x, parent); y = find(y, parent);

    if(x == y) return; // for a mesma, faco nada

    // compressao de caminho
    // quero sempre botar o x no y
    if(size[x] > size[y]) std::swap(x,y);
    size[y] += size[x]; // arvore y tem seu tamanho aumentado
    parent[x] = y; // pai de x eh o y
}

// -----------------------------------------------------------------------------

// FUNCAO COMPARADORA - HEURISTICA
bool comp(const std::pair<Aresta, double> &a, std::pair<Aresta, double> &b){
    return a.second < b.second;
}

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
    int infoN, infoM;   // são uteis?
    //-------------------------------------------------------------------------

    try {
        std::ifstream file(filename);

        if( !file.is_open() ) { throw "Erro ao abrir arquivo, abortando!"; }

        std::cout << PTAG << "Arquivo aberto com sucesso!" << std::endl;

        Parameter currState = n;
        char line[30];

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

    // // Grafo pronto para uso!
    // std::cout << custos << std::endl;
    // std::cout << custosQuad << std::endl;

    std::map<Aresta, int> custosLinDict;
    std::map<ParAresta, int> custosQuadDict;

    for( auto &c : custos ){
        custosLinDict[c.first] = c.second;
    }
    for( auto &c : custosQuad ){
        custosQuadDict[c.first] = c.second;
    }

    std::map<Aresta, double> custoAcumulado;
    for( auto &c : custosQuad )
        custoAcumulado[c.first.first] += c.second;
    for ( auto &a_c : custoAcumulado ){
        a_c.second /= infoM;
    }

    for( auto &c : custos )
        custoAcumulado[c.first] += c.second;


    std::vector<std::pair<Aresta, double>> novosCustos;
    for ( auto &a_c : custoAcumulado ){
        novosCustos.push_back(std::make_pair(std::make_pair(a_c.first.first, a_c.first.second), a_c.second));
    }

    std::sort(novosCustos.begin(), novosCustos.end(), comp);

    size_t pais_uf[infoN+1], tamanhos_uf[infoN+1];
    init(infoN, pais_uf, tamanhos_uf);

    std::vector<std::pair<Aresta, double>> solucao; // lista de arestas que serao selecionadas
    for( auto &custo : novosCustos ){
        if(find(custo.first.first, pais_uf) == find(custo.first.second, pais_uf))
            continue; // se formar ciclo, nao adiciona

        join(custo.first.first, custo.first.second, pais_uf, tamanhos_uf); // adiciona no union find
        solucao.push_back(custo); // adiciona no vetor resposta
        if(solucao.size() == infoN-1) break; // se chegar a n-1 arestas, eh arvore
    }

    int custoFinalLinear = 0;
    int custoFinalQuad = 0;
    std::cout << "\nArestas presentes na solução:\n";
    for( auto &aresta : solucao ){
        custoFinalLinear += custosLinDict[aresta.first];
        std::cout << aresta.first << std::endl;
        for( auto &aresta_ : solucao ){
            if(aresta_ == aresta) continue;
            custoFinalQuad += custosQuadDict[std::make_pair(aresta.first, aresta_.first)];
        }
    }
    std::cout << "Custo linear calculado: " << custoFinalLinear << std::endl;
    std::cout << "Custo quadratico calculado: " << custoFinalQuad << std::endl;
    std::cout << "Custo total calculado: " << custoFinalLinear + custoFinalQuad << std::endl;
    return 0;
}
