// GrafoIO.cpp

#include "GrafoIO.h"

#include <fstream>
#include <stdexcept>
#include <string>

std::unique_ptr<Grafo> carregarDeArquivo(const std::string &caminho,
                                         bool usarLista)
{
  std::ifstream f(caminho);
  if (!f.is_open())
    throw std::runtime_error("Nao foi possivel abrir: " + caminho);

  int V, A, D, P;
  if (!(f >> V >> A >> D >> P))
    throw std::runtime_error("Cabecalho invalido em: " + caminho);

  bool direcionado = (D == 1);
  bool ponderado = (P == 1);

  // Cria a representação escolhida
  std::unique_ptr<Grafo> g;
  if (usarLista)
    g = std::make_unique<GrafoLista>(direcionado, ponderado);
  else
    g = std::make_unique<GrafoMatriz>(direcionado, ponderado);

  // Insere vértices com label = índice em string
  for (int i = 0; i < V; ++i)
    g->inserirVertice(std::to_string(i));

  // Lê arestas
  for (int i = 0; i < A; ++i)
  {
    int ao, ad;
    if (!(f >> ao >> ad))
      throw std::runtime_error("Aresta " + std::to_string(i) + " invalida.");

    float ap = 1.0f;
    if (ponderado && !(f >> ap))
      throw std::runtime_error("Peso da aresta " + std::to_string(i) + " ausente.");

    if (!g->inserirAresta(ao, ad, ap))
      throw std::runtime_error("Indices de aresta invalidos na linha " +
                               std::to_string(i + 2));
  }

  return g;
}
