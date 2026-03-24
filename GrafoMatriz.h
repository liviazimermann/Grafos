#pragma once

#include "Grafo.h"

class GrafoMatriz : public Grafo
{
private:
  std::vector<std::vector<float>> matriz; // matriz[i][j] = peso da aresta i→j (0 = sem aresta)

  bool indiceValido(int i) const;

public:
  GrafoMatriz(bool direcionado, bool ponderado);
  ~GrafoMatriz() override = default;

  bool inserirVertice(std::string label) override;
  bool removerVertice(int indice) override;
  std::string labelVertice(int indice) const override;
  void imprimeGrafo() const override;

  bool inserirAresta(int origem, int destino, float peso = 1) override;
  bool removerAresta(int origem, int destino) override;
  bool existeAresta(int origem, int destino) const override;
  float pesoAresta(int origem, int destino) const override;

  std::vector<int> retornarVizinhos(int vertice) const override;

  int numArestas() const override;
  int grauVertice(int indice) const override;
};
