// BuscaLocal.h
// Otimizacao do fluxo maximo por busca local (hill climbing).
//
// Solucao        : uma orientacao do grafo (estrutura base = solucao inicial).
// Vizinho        : o grafo com a direcao de UMA aresta invertida.
// Funcao objetivo: o fluxo maximo (Ford-Fulkerson) da solucao -> MAXIMIZAR.

#pragma once
#include "Grafo.h"
#include <memory>

struct ResultadoBuscaLocal
{
  float fluxoInicial;            // fluxo maximo da solucao base (entrada)
  float fluxoFinal;              // fluxo maximo da melhor solucao encontrada
  int passos;                    // numero de movimentos (melhorias aplicadas)
  std::unique_ptr<Grafo> solucao; // melhor grafo encontrado
  double tempoMs;
};

class BuscaLocal
{
public:
  // Hill climbing por melhor-vizinho: a cada iteracao testa inverter cada
  // aresta, calcula o fluxo maximo de cada vizinho e move para o melhor se ele
  // superar a solucao atual. Para no primeiro otimo local.
  static ResultadoBuscaLocal otimizar(const Grafo &g, int origem, int destino);
};
