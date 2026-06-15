// Busca.h
// Algoritmos de busca e menor caminho sobre a interface Grafo.
// Todos operam via retornarVizinhos() / pesoAresta() — funcionam em Lista e Matriz.

#pragma once
#include "Grafo.h"
#include <vector>

// Imprime os vértices alcançáveis a partir de 'origem' em largura (BFS).
void bfs(const Grafo &g, int origem);

// Imprime os vértices alcançáveis a partir de 'origem' em profundidade (DFS).
void dfs(const Grafo &g, int origem);

// DFS que procura um caminho de 'origem' ate 'destino' percorrendo apenas
// arcos de capacidade positiva (arestas de custo 0 sao ignoradas). Em caso de
// sucesso devolve true e preenche 'caminho' com a sequencia de vertices
// origem..destino. Usado pelo Ford-Fulkerson para achar caminhos aumentantes.
bool dfsCaminho(const Grafo &g, int origem, int destino, std::vector<int> &caminho);

// Dado um grafo ponderado e um vértice de origem, imprime a menor distância
// e o caminho da origem até cada outro vértice alcançável.
void dijkstra(const Grafo &g, int origem);
