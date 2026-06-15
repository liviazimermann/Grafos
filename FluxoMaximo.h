// FluxoMaximo.h
// Algoritmo de fluxo maximo de Ford-Fulkerson sobre a interface Grafo.
// Caminho aumentante encontrado por busca em profundidade (DFS da M1).

#pragma once
#include "Grafo.h"
#include <memory>
#include <vector>

struct ResultadoFluxo
{
  float fluxoMaximo;             // valor do fluxo maximo da origem ao destino
  int caminhos;                  // numero de caminhos aumentantes usados
  double tempoMs;                // tempo de execucao
  std::unique_ptr<Grafo> residual; // rede residual final (capacidades restantes)
};

class FluxoMaximo
{
public:
  // Ford-Fulkerson: devolve o fluxo maximo de 'origem' ate 'destino'.
  // O grafo de entrada NAO e modificado — todo o trabalho e feito sobre uma
  // copia (rede residual). 'destino' padrao = ultimo vertice.
  static ResultadoFluxo fordFulkerson(const Grafo &g, int origem, int destino);
};
