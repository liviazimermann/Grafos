// AGM.cpp

#include "AGM.h"
#include <algorithm>
#include <chrono>
#include <numeric>
#include <queue>
#include <tuple>
#include <vector>

using namespace std;

// ── Prim ──────────────────────────────────────────────────────────────────────
//
// Mapeamento do pseudocódigo:
//   S  = vetor 'arestas' (conjunto solução)
//   Q  = vértices com naAGM[v] == false (ainda não incorporados)
//   O min-heap mantém candidatas {u, v} onde u já está na AGM e v ainda está em Q.

ResultadoAGM AGM::prim(const Grafo &g, int verticeInicial)
{
  auto inicio = chrono::high_resolution_clock::now();

  int n = g.numVertices();
  vector<bool> naAGM(n, false);
  vector<ArestaAGM> arestas;
  float pesoTotal = 0.0f;

  // min-heap: (peso, origem, destino)
  using Entrada = tuple<float, int, int>;
  priority_queue<Entrada, vector<Entrada>, greater<Entrada>> heap;

  // Remove vértice inicial de Q
  naAGM[verticeInicial] = true;
  for (int viz : g.retornarVizinhos(verticeInicial))
    heap.push({g.pesoAresta(verticeInicial, viz), verticeInicial, viz});

  while (!heap.empty())
  {
    auto [peso, u, v] = heap.top();
    heap.pop();

    if (naAGM[v]) // v ja saiu de Q — aresta obsoleta
      continue;

    // Menor aresta {u,v}: u ja esta na AGM, v ainda esta em Q → adiciona ao S
    naAGM[v] = true;
    arestas.push_back({u, v, peso});
    pesoTotal += peso;

    // Expande a fronteira a partir de v
    for (int viz : g.retornarVizinhos(v))
      if (!naAGM[viz])
        heap.push({g.pesoAresta(v, viz), v, viz});
  }

  double tempoMs = chrono::duration<double, milli>(
                       chrono::high_resolution_clock::now() - inicio)
                       .count();

  return {arestas, pesoTotal, tempoMs, (int)arestas.size() == n - 1};
}

// ── Kruskal ───────────────────────────────────────────────────────────────────
//
// Mapeamento do pseudocódigo:
//   Q  = vetor 'todasArestas' ordenado por peso (menor primeiro)
//   F  = floresta representada pelo Union-Find (cada vértice começa isolado)
//   S  = vetor 'arestas' (conjunto solução)

struct UnionFind
{
  vector<int> pai, rank;

  UnionFind(int n) : pai(n), rank(n, 0)
  {
    iota(pai.begin(), pai.end(), 0);
  }

  int find(int x)
  {
    if (pai[x] != x)
      pai[x] = find(pai[x]); // compressao de caminho
    return pai[x];
  }

  bool unite(int x, int y)
  {
    int px = find(x), py = find(y);
    if (px == py)
      return false; // ja na mesma arvore
    if (rank[px] < rank[py])
      swap(px, py);
    pai[py] = px;
    if (rank[px] == rank[py])
      rank[px]++;
    return true;
  }
};

ResultadoAGM AGM::kruskal(const Grafo &g)
{
  auto inicio = chrono::high_resolution_clock::now();

  int n = g.numVertices();

  // Monta Q: todas as arestas do grafo (cada aresta não-dir aparece uma vez)
  vector<ArestaAGM> todasArestas;
  for (int u = 0; u < n; ++u)
    for (int v : g.retornarVizinhos(u))
      if (v > u) // evita duplicatas em grafos nao-direcionados
        todasArestas.push_back({u, v, g.pesoAresta(u, v)});

  // Ordena Q por peso crescente
  sort(todasArestas.begin(), todasArestas.end(),
       [](const ArestaAGM &a, const ArestaAGM &b) { return a.peso < b.peso; });

  // F: floresta (Union-Find) — cada vertice comeca como arvore isolada
  UnionFind uf(n);
  vector<ArestaAGM> arestas;
  float pesoTotal = 0.0f;

  // Processa Q
  for (const auto &aresta : todasArestas)
  {
    // Se u e v estao em arvores diferentes em F → adiciona ao S e une
    if (uf.unite(aresta.origem, aresta.destino))
    {
      arestas.push_back(aresta);
      pesoTotal += aresta.peso;
      if ((int)arestas.size() == n - 1)
        break; // AGM completa
    }
  }

  double tempoMs = chrono::duration<double, milli>(
                       chrono::high_resolution_clock::now() - inicio)
                       .count();

  return {arestas, pesoTotal, tempoMs, (int)arestas.size() == n - 1};
}
