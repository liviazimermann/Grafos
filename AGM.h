// AGM.h
// Algoritmos de Arvore Geradora Minima sobre a interface Grafo.

#pragma once
#include "Grafo.h"
#include <vector>

using namespace std;

struct ArestaAGM
{
  int origem;
  int destino;
  float peso;
};

struct ResultadoAGM
{
  vector<ArestaAGM> arestas;
  float pesoTotal;
  double tempoMs;
  bool conexo; // false se o grafo nao for conexo (AGM incompleta)
};

class AGM
{
public:
  // Prim: min-heap, inicia pelo verticeInicial (padrao 0).
  static ResultadoAGM prim(const Grafo &g, int verticeInicial = 0);

  // Kruskal: ordena arestas + Union-Find.
  static ResultadoAGM kruskal(const Grafo &g);
};
