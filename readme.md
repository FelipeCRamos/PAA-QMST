# QMST Problem - NP Hard Project

## Autores
+ Felipe Ramos
+ Paulo Medeiros


## Execução do algoritmo
 Para executar e testar o algoritmo desenvolvido, basta atentar às seguintes dependências:
+ Computador com `G++/clang` instalados (compiladores de `C++`).
+ Arquivo com grafo e suas respectivas informações em `.txt` atentando a formatação suportada pelo algoritmo.
        
A formatação suportada consiste no seguinte padrão:
```
# n
int
# m
int
# c
int int int
... 
int int int
# q
int int int int int
...
int int int int int
# Edges
int int
...
int int
# eof
```
Onde:
+ `n` = número de vértices
+ `m` = número de arestas
+ `c` = 3 inteiros, onde respectivamente são: (vertice 1, vertice 2), peso da aresta.
+ `q` = 5 inteiros, onde respectivamente são: (vertice 1, vertice 2), (vertice 3, vertice 4), peso quadrático
+ `Edges` = 2 inteiros, onde respectivamente são: (vertice 1, vertice 2)
        
E, depois de estar em posse do arquivo correto, basta rodar o algoritmo utilizando o seguinte comando na pasta do projeto:
```
make \&\& ./agmq tests/converted/<arquivo-de-testes-escolhido>.txt
```
A formatação exibida deve ser seguida à risca, incluindo o uso de \#'s e capitalização de letras.
        
