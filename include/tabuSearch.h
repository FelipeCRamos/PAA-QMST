/*
- vizinhança
    - troca de 1 aresta
        - adiciona uma aresta que está fora
        - acho o ciclo que é formado
        - removo arestas do ciclo a fim de trocar
            - faço qual troca? analisar sempre custos e qual aresta sai e entra
                - talvez um pair pra isso
        - critério de aspiração




A. função heurística de construção
B. função heurística de destrução
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
3. escolha uma das top K arestas, onde a função de probabilidade é definida em função da contribuição da solução ao somatório (elevado a um fator L)
4. faz SO nela, retirando rand() arestas
5. volta pra (1)
*/
