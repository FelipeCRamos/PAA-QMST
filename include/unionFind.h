#ifndef _UNION_FIND_H_
#define _UNION_FIND_H_

#include <stack>
#include <algorithm>

class UnionFind{

    private:
        int _n;
        int *sizes;
        int *parents;
        std::stack<std::pair<int,int>> rb_parent, rb_size;


    public:
        UnionFind(int n){
            _n = n;
            sizes = new int[_n];
            parents = new int[_n];
            for(int i = 0; i < _n; ++i){
                sizes[i] = 1; // tamanho inicial
                parents[i] = i; // valor padrao: cada um eh seu pai
            }
        }

        ~UnionFind(){
            delete[] sizes, parents;
        }

        int find(int current){
            if(current < _n){
                // ROLLBACK DOESNT HAVE PATH COMPRESSION
                int new_root = current; // variavel para guardar nova raiz
                while(new_root != parents[new_root]) // enquanto nao encontro no raiz
                new_root = parents[new_root]; // subo na arvore

                return new_root; // retornamo a raiz da arvore
            }else{
                throw "Não é possível fazer find num UnionFind vazio!";
            }
        }

        void join(int a, int b){
            if(a < _n && b < _n){
                a = find(a); b = find(b); // unimos uma raiz a outra

                if(a == b) return; // se for a mesma raiz, nao tenho o que unir

                // uniao por tamanho
                if(sizes[a] < sizes[b]) std::swap(a,b); // quero unir "b" a "a"

                rb_size.push({a, sizes[a]});
                rb_parent.push({b, parents[b]});

                sizes[a] += sizes[b]; // atualizando tamanho de "a"
                parents[b] = a; // pai de "b" eh "a"
            }else{
                throw "Não é possível fazer union num UnionFind vazio!";
            }
        }

        void rollback(){
            if(!rb_size.empty() && !rb_parent.empty()){
                sizes[rb_size.top().first] = rb_size.top().second; rb_size.pop();
                parents[rb_parent.top().first] = rb_parent.top().second; rb_parent.pop();
            }else{
                throw "Não é possível fazer rollback com a pilha vazia!";
            }
        }
};

#endif
