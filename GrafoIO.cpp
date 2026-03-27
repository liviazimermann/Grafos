// GrafoIO.cpp

#include "GrafoIO.h"
#include <fstream>
#include <stdexcept>
#include <string>

using namespace std;

unique_ptr<Grafo> carregarDeArquivo(const string &caminho,
                                         bool usarLista)
{
  ifstream f(caminho);
  if (!f.is_open())
    throw runtime_error("Nao foi possivel abrir: " + caminho);

  int V, A, D, P;
  if (!(f >> V >> A >> D >> P))
    throw runtime_error("Cabecalho invalido em: " + caminho);

  bool direcionado = (D == 1);
  bool ponderado = (P == 1);

  // Cria a representação escolhida
  unique_ptr<Grafo> g;
  if (usarLista)
    g = make_unique<GrafoLista>(direcionado, ponderado);
  else
    g = make_unique<GrafoMatriz>(direcionado, ponderado);

  // Insere vértices com label = índice em string
  for (int i = 0; i < V; ++i)
    g->inserirVertice(to_string(i));

  // Lê arestas
  for (int i = 0; i < A; ++i)
  {
    int ao, ad;
    if (!(f >> ao >> ad))
      throw runtime_error("Aresta " + to_string(i) + " invalida.");

    float ap = 1.0f;
    if (ponderado && !(f >> ap))
      throw runtime_error("Peso da aresta " + to_string(i) + " ausente.");

    if (!g->inserirAresta(ao, ad, ap))
      throw runtime_error("Indices de aresta invalidos na linha " +
                               to_string(i + 2));
  }

  return g;
}
