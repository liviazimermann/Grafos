// Busca.h
// Algoritmos de busca e menor caminho sobre a interface Grafo.
// Todos operam via retornarVizinhos() / pesoAresta() — funcionam em Lista e Matriz.

#pragma once

#include "Grafo.h"

// Imprime os vértices alcançáveis a partir de 'origem' em largura (BFS).
void bfs(const Grafo &g, int origem);

// Imprime os vértices alcançáveis a partir de 'origem' em profundidade (DFS).
void dfs(const Grafo &g, int origem);

// Dado um grafo ponderado e um vértice de origem, imprime a menor distância
// e o caminho da origem até cada outro vértice alcançável.
void dijkstra(const Grafo &g, int origem);
