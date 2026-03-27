// Busca.cpp

#include "Busca.h"
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

// ── BFS ───────────────────────────────────────────────────────────────────────

void bfs(const Grafo &g, int origem)
{
  int n = g.numVertices();
  if (origem < 0 || origem >= n)
  {
    cout << "Vertice de origem invalido.\n";
    return;
  }

  vector<bool> visitado(n, false);
  queue<int> fila;

  visitado[origem] = true;
  fila.push(origem);

  cout << "BFS a partir de " << g.labelVertice(origem) << ": ";

  while (!fila.empty())
  {
    int atual = fila.front();
    fila.pop();

    cout << g.labelVertice(atual) << " ";

    for (int viz : g.retornarVizinhos(atual))
    {
      if (!visitado[viz])
      {
        visitado[viz] = true;
        fila.push(viz);
      }
    }
  }

  cout << "\n";
}

// ── DFS ───────────────────────────────────────────────────────────────────────

static void dfsRecursivo(const Grafo &g, int atual, vector<bool> &visitado)
{
  visitado[atual] = true;
  cout << g.labelVertice(atual) << " ";

  for (int viz : g.retornarVizinhos(atual))
    if (!visitado[viz])
      dfsRecursivo(g, viz, visitado);
}

void dfs(const Grafo &g, int origem)
{
  int n = g.numVertices();
  if (origem < 0 || origem >= n)
  {
    cout << "Vertice de origem invalido.\n";
    return;
  }

  vector<bool> visitado(n, false);
  cout << "DFS a partir de " << g.labelVertice(origem) << ": ";
  dfsRecursivo(g, origem, visitado);
  cout << "\n";
}

// ── Dijkstra ──────────────────────────────────────────────────────────────────

void dijkstra(const Grafo &g, int origem)
{
  int n = g.numVertices();
  if (origem < 0 || origem >= n)
  {
    cout << "Vertice de origem invalido.\n";
    return;
  }

  const float INF = numeric_limits<float>::infinity();

  vector<float> dist(n, INF);     // distância da origem a cada vértice
  vector<bool> fechado(n, false); // vértice já processado?
  vector<int> anterior(n, -1);    // vértice anterior no caminho

  dist[origem] = 0.0f;

  // Enquanto existir vértice aberto com distância não-infinita
  while (true)
  {
    // Encontra o vértice aberto com menor distância
    int atual = -1;
    for (int i = 0; i < n; ++i)
      if (!fechado[i] && dist[i] < INF)
        if (atual == -1 || dist[i] < dist[atual])
          atual = i;

    if (atual == -1)
      break; // nenhum vértice aberto alcançável restante

    // Relaxa os vizinhos
    for (int viz : g.retornarVizinhos(atual))
    {
      if (fechado[viz])
        continue;

      float peso = g.pesoAresta(atual, viz);
      if (dist[atual] + peso < dist[viz])
      {
        dist[viz] = dist[atual] + peso;
        anterior[viz] = atual;
      }
    }

    fechado[atual] = true;
  }

  // Imprime resultados
  cout << "Dijkstra a partir de " << g.labelVertice(origem) << ":\n";

  for (int i = 0; i < n; ++i)
  {
    if (i == origem)
      continue;

    cout << "  " << g.labelVertice(origem) << " -> " << g.labelVertice(i) << ": ";

    if (dist[i] == INF)
    {
      cout << "inacessivel\n";
      continue;
    }

    cout << "dist=" << dist[i] << "  caminho: ";

    // Reconstrói o caminho de trás para frente
    vector<int> caminho;
    for (int v = i; v != -1; v = anterior[v])
      caminho.push_back(v);

    // Imprime na ordem correta (origem -> destino)
    for (int k = static_cast<int>(caminho.size()) - 1; k >= 0; --k)
    {
      cout << g.labelVertice(caminho[k]);
      if (k > 0)
        cout << " -> ";
    }

    cout << "\n";
  }
}
