#include "GrafoLista.h"
#include <iostream>
#include <algorithm>

// ── Auxiliar ─────────────────────────────────────────────────────────────────

bool GrafoLista::indiceValido(int i) const
{
  return i >= 0 && i < numVertices();
}

// ── Construtor ────────────────────────────────────────────────────────────────

GrafoLista::GrafoLista(bool direcionado, bool ponderado)
    : Grafo(direcionado, ponderado) {}

// ── Vértices ──────────────────────────────────────────────────────────────────

bool GrafoLista::inserirVertice(std::string label)
{
  vertices.push_back(label);
  lista.push_back({});
  return true;
}

bool GrafoLista::removerVertice(int idx)
{
  if (!indiceValido(idx))
    return false;

  // remove a lista de arestas do vértice
  lista.erase(lista.begin() + idx);
  vertices.erase(vertices.begin() + idx);

  // remove arestas que apontam para idx e corrige índices > idx
  for (auto &vizinhos : lista)
  {
    // remove arestas cujo destino é idx
    vizinhos.erase(
        std::remove_if(vizinhos.begin(), vizinhos.end(),
                       [idx](const Aresta &a)
                       { return a.destino == idx; }),
        vizinhos.end());
    // decrementa destinos acima do índice removido
    for (auto &a : vizinhos)
      if (a.destino > idx)
        a.destino--;
  }

  return true;
}

std::string GrafoLista::labelVertice(int idx) const
{
  if (!indiceValido(idx))
    return "";
  return vertices[idx];
}

// ── Impressão ─────────────────────────────────────────────────────────────────

void GrafoLista::imprimeGrafo() const
{
  for (int i = 0; i < numVertices(); ++i)
  {
    std::cout << vertices[i] << ": ";
    for (const auto &a : lista[i])
    {
      std::cout << vertices[a.destino];
      if (ponderado)
        std::cout << "(" << a.peso << ")";
      std::cout << " ";
    }
    std::cout << "\n";
  }
}

// ── Arestas ───────────────────────────────────────────────────────────────────

bool GrafoLista::inserirAresta(int origem, int destino, float peso)
{
  if (!indiceValido(origem) || !indiceValido(destino))
    return false;

  float valor = ponderado ? peso : 1.0f;

  lista[origem].emplace_back(destino, valor);

  if (!direcionado)
    lista[destino].emplace_back(origem, valor);

  return true;
}

bool GrafoLista::removerAresta(int origem, int destino)
{
  if (!indiceValido(origem) || !indiceValido(destino))
    return false;

  auto remover = [](std::vector<Aresta> &v, int dest)
  {
    v.erase(
        std::remove_if(v.begin(), v.end(),
                       [dest](const Aresta &a)
                       { return a.destino == dest; }),
        v.end());
  };

  remover(lista[origem], destino);

  if (!direcionado)
    remover(lista[destino], origem);

  return true;
}

bool GrafoLista::existeAresta(int origem, int destino) const
{
  if (!indiceValido(origem) || !indiceValido(destino))
    return false;

  for (const auto &a : lista[origem])
    if (a.destino == destino)
      return true;

  return false;
}

float GrafoLista::pesoAresta(int origem, int destino) const
{
  if (!indiceValido(origem) || !indiceValido(destino))
    return 0.0f;

  for (const auto &a : lista[origem])
    if (a.destino == destino)
      return a.peso;

  return 0.0f;
}

int GrafoLista::numArestas() const
{
  int total = 0;
  for (const auto &vizinhos : lista)
    total += static_cast<int>(vizinhos.size());

  // no grafo não direcionado cada aresta foi inserida nos dois sentidos
  return direcionado ? total : total / 2;
}

int GrafoLista::grauVertice(int idx) const
{
  if (!indiceValido(idx))
    return -1;

  int grau = static_cast<int>(lista[idx].size()); // out-degree

  if (direcionado)
  {
    // soma in-degree: contar quantas listas apontam para idx
    for (const auto &vizinhos : lista)
      for (const auto &a : vizinhos)
        if (a.destino == idx)
          grau++;
  }

  return grau;
}

// ── Vizinhos ──────────────────────────────────────────────────────────────────

std::vector<int> GrafoLista::retornarVizinhos(int vertice) const
{
  std::vector<int> vizinhos;
  if (!indiceValido(vertice))
    return vizinhos;

  for (const auto &a : lista[vertice])
    vizinhos.push_back(a.destino);

  return vizinhos;
}
