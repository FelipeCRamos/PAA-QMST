#ifndef _FILE_PARSER_H_
#define _FILE_PARSER_H_

#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <unordered_map>
#include "defines.h"

namespace std
{
    template<> struct hash<pair<int,int>>
    {
        typedef pair<int,int> argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& s) const noexcept
        {
            return s.first ^ (s.second << 3);
        }
    };
}

class Parser{
    private:
        int n, m;
        std::vector<std::pair<int,int>> edges;
        std::unordered_map<std::pair<int,int>, int> edgeID;
        int **costs;
        std::string fileName;
        enum ListStates { OPENP, NUM, COMMA, CLOSEP, DONEL, ERRORL };
        enum FileStates { N, M, EDGES, C, Q1, Q2, FINAL, DONEF, ERRORF };
        const std::string NUMBERS = "0123456789";

        void allocateCosts(){
            costs = new int*[m];
            for(int i = 0; i < m; ++i) costs[i] = new int[m];
        }

        bool parseList(std::string s, std::string dels, int numberElements, int *array){
            std::string num;
            int i = 0;
            int el_i = 0;
            bool ok = false;
            ListStates currState = OPENP;

            while(true){
                char c = s[i];

                switch (currState) {
                    case OPENP:
                    // printf("OPENP ");
                    if(c == dels[0]) currState = NUM;
                    else return false;
                    i++;
                    break;

                    case NUM:
                    // printf("NUM ");
                    if(NUMBERS.find(c) != std::string::npos){
                        num.push_back(c);
                        i++;
                    }else{
                        array[el_i++] = (std::stoi(num));
                        num = "";
                        if(--numberElements > 0) currState = COMMA;
                        else currState = CLOSEP;
                    }
                    break;

                    case COMMA:
                    // printf("COMMA ");
                    if(c == ',') currState = NUM;
                    else return false;
                    i++;
                    break;

                    case CLOSEP:
                    // printf("CLOSEP ");
                    if(c == dels[1]) currState = DONEL;
                    else return false;
                    i++;
                    break;

                    case DONEL:
                    // printf("DONENUM ");
                    if(i == s.size()) return true;
                    else return false;
                    break;
                }
            }
        }

    public:

        Parser(std::string _fileName){
            fileName = _fileName;
        }

        ~Parser(){
            for(int i = 0; i < m; ++i) delete[] costs[i];
            delete[] costs;
        }

        bool parse(){
            // LEITURA DO ARQUIVO
            std::ifstream file(fileName);
            try {

                if( !file.is_open() ) { throw "Erro ao abrir arquivo, abortando!"; }

                FileStates currState = N;

                while( file.good() ) {
                    std::string line;
                    std::getline(file, line);

                    std::stringstream buf;
                    buf << line;

                    if(currState == DONEF || currState == ERRORF){
                        break;
                    }

                    std::string s[4];
                    switch (currState) {
                        case N:{
                            buf >> s[0] >> s[1] >> s[2] >> n >> s[3];
                            if(s[0] != "param" || s[1] != "n" || s[2] != ":=" || s[3] != ";")
                                throw "Erro ao ler parametro N, abortando!";
                            else currState = M;
                            break;
                        }

                        case M:{
                            buf >> s[0] >> s[1] >> s[2] >> m >> s[3];
                            if(s[0] != "param" || s[1] != "m" || s[2] != ":=" || s[3] != ";")
                                throw "Erro ao ler parametro M, abortando!";
                            else currState = EDGES;
                            break;
                        }

                        case EDGES:{
                            buf >> s[0] >> s[1] >> s[2];
                            if(s[0] != "set" || s[1] != "Edges" || s[2] != ":=")
                                throw "Erro ao ler parametro Edges, abortando!";

                            int *vals = new int[2];
                            for(int i = 0; i < m; ++i){
                                buf >> s[0];
                                if(!parseList(s[0], "()", 2, vals)){
                                    throw "Erro ao ler parametro Edges, abortando!";
                                }else{
                                    std::pair<int,int> edge = {vals[0], vals[1]};
                                    edgeID[edge] = i;
                                    edges.push_back(edge);
                                }
                            }
                            delete[] vals;

                            buf >> s[0];
                            if(s[0] != ";") throw "Erro ao ler parametro Edges, abortando!";
                            else currState = C;
                            break;
                        }

                        case C:{
                            allocateCosts();
                            buf >> s[0] >> s[1] >> s[2];
                            if(s[0] != "param" || s[1] != "c" || s[2] != ":=")
                                throw "Erro ao ler parametro C, abortando!";


                            int *vals = new int[2];
                            for(int i = 0; i < m; ++i){
                                int cost;
                                buf >> s[0] >> cost;
                                if(!parseList(s[0], "[]", 2, vals))
                                    throw "Erro ao ler parametro C, abortando!";
                                else{
                                    std::pair<int,int> edge = {vals[0], vals[1]};
                                    int id = edgeID[edge];
                                    costs[id][id] = cost;
                                }
                            }
                            delete[] vals;
                            buf >> s[0];
                            if(s[0] != ";") throw "Erro ao ler parametro C, abortando!";
                            else currState = Q1;
                            break;
                        }

                        case Q1:{
                            buf >> s[0] >> s[1] >> s[2];
                            if(s[0] != "param" || s[1] != "q" || s[2] != ":=")
                                throw "Erro ao ler parametro Q[1], abortando!";
                            else currState = Q2;
                            break;
                        }

                        case Q2:{
                            int *vals = new int[4];
                            for(int i = 0; i < m*m - m; ++i){
                                int cost;
                                buf >> s[0] >> cost;
                                if(!parseList(s[0], "[]", 4, vals))
                                    throw "Erro ao ler parametro Q[2], abortando!";
                                else{
                                    std::pair<int,int> edgeA = {vals[0], vals[1]};
                                    std::pair<int,int> edgeB = {vals[2], vals[3]};
                                    int idA = edgeID[edgeA];
                                    int idB = edgeID[edgeB];
                                    costs[idB][idA] = costs[idA][idB] = cost;
                                }
                            }
                            delete[] vals;

                            buf >> s[0];
                            if(s[0] != ";") throw "Erro ao ler parametro Q[2], abortando!";
                            else currState = FINAL;
                            break;
                        }

                        case FINAL:{
                            buf >> s[0];
                            if(s[0] != "end;") throw "Erro ao ler parametro final, abortando!";
                            else currState = DONEF;
                        }

                    }
                }


                file.close();
            } catch(const char *errorMsg) {
                file.close();
                std::cerr << "fileParser: " << errorMsg << std::endl;
                return false;
            }
            return true;
        }

        int getN(){ return n; }
        int getM(){ return m; }
        int** getCosts(){ return costs; }
        std::vector<std::pair<int,int>> getEdges(){ return edges; }
        // std::unordered_map<std::pair<int,int>, int> getEdgesIDs(){ return edgeID; }

};

#endif
