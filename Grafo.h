#pragma once

#include <memory>
#include <string>
#include <vector>

using namespace std;

class Grafo
{
protected:
  bool direcionado;
  bool ponderado;
  vector<string> vertices; // índice = identificador do vértice

public:
  Grafo(bool direcionado, bool ponderado)
      : direcionado(direcionado), ponderado(ponderado) {}

  virtual ~Grafo() = default;

  // Copia profunda: devolve um grafo novo e independente (sem referencias
  // ao original). Essencial para o Ford-Fulkerson (rede residual) e para a
  // busca local (geracao de vizinhos), que precisam alterar a copia sem
  // corromper o grafo de entrada.
  virtual unique_ptr<Grafo> clone() const = 0;

  // --- Informações do grafo ---
  bool ehDirecionado() const { return direcionado; }
  bool ehPonderado() const { return ponderado; }
  int numVertices() const { return static_cast<int>(vertices.size()); }

  // --- Interface a ser implementada pelas subclasses ---
  virtual bool inserirVertice(string label) = 0;
  virtual bool removerVertice(int indice) = 0;
  virtual string labelVertice(int indice) const = 0;
  virtual void imprimeGrafo() const = 0;

  virtual bool inserirAresta(int origem, int destino, float peso = 1) = 0;
  virtual bool removerAresta(int origem, int destino) = 0;
  virtual bool existeAresta(int origem, int destino) const = 0;
  virtual float pesoAresta(int origem, int destino) const = 0;
  virtual int numArestas() const = 0;
  virtual int grauVertice(int indice) const = 0;

  virtual vector<int> retornarVizinhos(int vertice) const = 0;
};
