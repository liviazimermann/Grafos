#pragma once

#include <string>
#include <vector>

class Grafo
{
protected:
  bool direcionado;
  bool ponderado;
  std::vector<std::string> vertices; // índice = identificador do vértice

public:
  Grafo(bool direcionado, bool ponderado)
      : direcionado(direcionado), ponderado(ponderado) {}

  virtual ~Grafo() = default;

  // --- Informações do grafo ---
  bool ehDirecionado() const { return direcionado; }
  bool ehPonderado() const { return ponderado; }
  int numVertices() const { return static_cast<int>(vertices.size()); }

  // --- Interface a ser implementada pelas subclasses ---
  virtual bool inserirVertice(std::string label) = 0;
  virtual bool removerVertice(int indice) = 0;
  virtual std::string labelVertice(int indice) const = 0;
  virtual void imprimeGrafo() const = 0;

  virtual bool inserirAresta(int origem, int destino, float peso = 1) = 0;
  virtual bool removerAresta(int origem, int destino) = 0;
  virtual bool existeAresta(int origem, int destino) const = 0;
  virtual float pesoAresta(int origem, int destino) const = 0;
  virtual int numArestas() const = 0;
  virtual int grauVertice(int indice) const = 0;

  virtual std::vector<int> retornarVizinhos(int vertice) const = 0;
};
