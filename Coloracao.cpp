// Coloracao.cpp

#include "Coloracao.h"
#include <algorithm>
#include <chrono>
#include <numeric>
#include <set>
#include <vector>

using namespace std;

// ── Helper ────────────────────────────────────────────────────────────────────

// Retorna a menor cor >= 1 não usada por nenhum vizinho já colorido de v.
static int menorCorDisponivel(const Grafo &g, int v, const vector<int> &cores)
{
  set<int> usadas;
  for (int viz : g.retornarVizinhos(v))
    if (cores[viz] > 0)
      usadas.insert(cores[viz]);
  int cor = 1;
  while (usadas.count(cor))
    ++cor;
  return cor;
}

// ── Sem Ordem ─────────────────────────────────────────────────────────────────

ResultadoColoracao Coloracao::semOrdem(const Grafo &g)
{
  auto inicio = chrono::high_resolution_clock::now();

  int n = g.numVertices();
  vector<int> cores(n, 0);
  int numCores = 0;

  for (int v = 0; v < n; ++v)
  {
    cores[v] = menorCorDisponivel(g, v, cores);
    numCores = max(numCores, cores[v]);
  }

  double tempoMs = chrono::duration<double, milli>(
                       chrono::high_resolution_clock::now() - inicio)
                       .count();
  return {numCores, cores, tempoMs};
}

// ── Welsh-Powell ──────────────────────────────────────────────────────────────

ResultadoColoracao Coloracao::welshPowell(const Grafo &g)
{
  auto inicio = chrono::high_resolution_clock::now();

  int n = g.numVertices();
  vector<int> ordem(n);
  iota(ordem.begin(), ordem.end(), 0);
  sort(ordem.begin(), ordem.end(), [&](int a, int b)
       { return g.grauVertice(a) > g.grauVertice(b); });

  vector<int> cores(n, 0);
  int numCores = 0;

  for (int v : ordem)
  {
    cores[v] = menorCorDisponivel(g, v, cores);
    numCores = max(numCores, cores[v]);
  }

  double tempoMs = chrono::duration<double, milli>(
                       chrono::high_resolution_clock::now() - inicio)
                       .count();
  return {numCores, cores, tempoMs};
}

// ── DSatur ────────────────────────────────────────────────────────────────────

ResultadoColoracao Coloracao::dsatur(const Grafo &g)
{
  auto inicio = chrono::high_resolution_clock::now();

  int n = g.numVertices();
  vector<int> cores(n, 0);
  vector<set<int>> coresViz(n); // cores distintas entre vizinhos já coloridos
  int numCores = 0;

  for (int passo = 0; passo < n; ++passo)
  {
    // Escolhe vértice sem cor com maior saturação; desempata pelo maior grau
    int escolhido = -1, maxSat = -1, maxGrau = -1;
    for (int v = 0; v < n; ++v)
    {
      if (cores[v] != 0)
        continue;
      int sat = (int)coresViz[v].size();
      int grau = g.grauVertice(v);
      if (sat > maxSat || (sat == maxSat && grau > maxGrau))
      {
        maxSat = sat;
        maxGrau = grau;
        escolhido = v;
      }
    }

    // Atribui menor cor disponível
    int cor = 1;
    while (coresViz[escolhido].count(cor))
      ++cor;
    cores[escolhido] = cor;
    numCores = max(numCores, cor);

    // Atualiza saturação dos vizinhos não coloridos
    for (int viz : g.retornarVizinhos(escolhido))
      if (cores[viz] == 0)
        coresViz[viz].insert(cor);
  }

  double tempoMs = chrono::duration<double, milli>(
                       chrono::high_resolution_clock::now() - inicio)
                       .count();
  return {numCores, cores, tempoMs};
}

// ── Força Bruta (backtracking) ────────────────────────────────────────────────

static bool backtrack(const Grafo &g, int v, int k, vector<int> &cores)
{
  if (v == g.numVertices())
    return true;

  for (int cor = 1; cor <= k; ++cor)
  {
    bool valido = true;
    for (int viz : g.retornarVizinhos(v))
      if (cores[viz] == cor)
      {
        valido = false;
        break;
      }
    if (!valido)
      continue;

    cores[v] = cor;
    if (backtrack(g, v + 1, k, cores))
      return true;
    cores[v] = 0;
  }
  return false;
}

ResultadoColoracao Coloracao::forcaBruta(const Grafo &g)
{
  auto inicio = chrono::high_resolution_clock::now();

  int n = g.numVertices();
  vector<int> cores(n, 0);

  for (int k = 1; k <= n; ++k)
  {
    fill(cores.begin(), cores.end(), 0);
    if (backtrack(g, 0, k, cores))
    {
      double tempoMs = chrono::duration<double, milli>(
                           chrono::high_resolution_clock::now() - inicio)
                           .count();
      return {k, cores, tempoMs};
    }
  }

  // Nunca alcançado: n cores sempre bastam
  double tempoMs = chrono::duration<double, milli>(
                       chrono::high_resolution_clock::now() - inicio)
                       .count();
  return {n, cores, tempoMs};
}
