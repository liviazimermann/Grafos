#pragma once
#include "Grafo.h"

using namespace std;

struct Aresta
{
  int destino;
  float peso;

  Aresta(int destino, float peso = 1.0f)
      : destino(destino), peso(peso) {}
};

class GrafoLista : public Grafo
{
private:
  vector<vector<Aresta>> lista; // lista[i] = arestas que saem de i

  bool indiceValido(int i) const;

public:
  GrafoLista(bool direcionado, bool ponderado);
  ~GrafoLista() override = default;

  bool inserirVertice(string label) override;
  bool removerVertice(int indice) override;
  string labelVertice(int indice) const override;
  void imprimeGrafo() const override;

  bool inserirAresta(int origem, int destino, float peso = 1) override;
  bool removerAresta(int origem, int destino) override;
  bool existeAresta(int origem, int destino) const override;
  float pesoAresta(int origem, int destino) const override;

  vector<int> retornarVizinhos(int vertice) const override;

  int numArestas() const override;
  int grauVertice(int indice) const override;
};
