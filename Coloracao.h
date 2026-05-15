// Coloracao.h
// Algoritmos de coloração de grafos sobre a interface Grafo.

#pragma once
#include "Grafo.h"
#include <vector>

using namespace std;

struct ResultadoColoracao
{
  int numCores;
  vector<int> cores; // cores[i] = cor do vértice i (1-indexed, 0 = sem cor)
  double tempoMs;
};

class Coloracao
{
public:
  static ResultadoColoracao semOrdem(const Grafo &g);
  static ResultadoColoracao welshPowell(const Grafo &g);
  static ResultadoColoracao dsatur(const Grafo &g);
  static ResultadoColoracao forcaBruta(const Grafo &g);
};
