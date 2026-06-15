// BuscaLocal.cpp

#include "BuscaLocal.h"
#include "FluxoMaximo.h"
#include <chrono>
#include <utility>
#include <vector>

using namespace std;

// Gera o vizinho obtido invertendo a aresta (u,v) -> (v,u) sobre uma copia
// independente de 'base'. Retorna o grafo novo (sem tocar em 'base').
static unique_ptr<Grafo> inverterAresta(const Grafo &base, int u, int v)
{
  auto vizinho = base.clone();
  float peso = vizinho->pesoAresta(u, v);
  vizinho->removerAresta(u, v);
  vizinho->inserirAresta(v, u, peso);
  return vizinho;
}

// ── Busca local (hill climbing) ────────────────────────────────────────────────
//
// Segue o esqueleto dos slides de Busca Local, adaptado para MAXIMIZACAO:
//   Inicie com uma solucao S
//   faca
//     melhorou <- false
//     S' <- selecionaMelhorVizinho   (melhor entre inverter cada aresta)
//     se fluxo(S') > fluxo(S) entao   (maximiza, ao inves de minimizar custo)
//       S <- S'; melhorou <- true
//   enquanto (melhorou)

ResultadoBuscaLocal BuscaLocal::otimizar(const Grafo &g, int origem, int destino)
{
  auto inicio = chrono::high_resolution_clock::now();

  // Partida: solucao inicial = estrutura base do grafo passado.
  unique_ptr<Grafo> atual = g.clone();
  float fluxoInicial = FluxoMaximo::fordFulkerson(*atual, origem, destino).fluxoMaximo;
  float fluxoAtual = fluxoInicial;
  int passos = 0;

  bool melhorou = true;
  while (melhorou)
  {
    melhorou = false;

    // Coleta os arcos da solucao atual antes de gerar vizinhos (nao mexemos
    // em 'atual' durante a varredura — cada vizinho e uma copia).
    int n = atual->numVertices();
    vector<pair<int, int>> arcos;
    for (int u = 0; u < n; ++u)
      for (int v : atual->retornarVizinhos(u))
        arcos.push_back({u, v});

    // Seleciona o melhor vizinho (inverter cada aresta, uma de cada vez).
    unique_ptr<Grafo> melhorVizinho;
    float melhorFluxo = fluxoAtual;

    for (const auto &[u, v] : arcos)
    {
      // So invertemos quando o arco reverso ainda nao existe: assim o vizinho
      // e sempre uma orientacao bem definida (sem arcos paralelos ambiguos).
      if (atual->existeAresta(v, u))
        continue;

      auto vizinho = inverterAresta(*atual, u, v);
      float fluxo = FluxoMaximo::fordFulkerson(*vizinho, origem, destino).fluxoMaximo;

      if (fluxo > melhorFluxo)
      {
        melhorFluxo = fluxo;
        melhorVizinho = std::move(vizinho);
      }
    }

    // Se algum vizinho superou a solucao atual, move para ele.
    if (melhorVizinho)
    {
      atual = std::move(melhorVizinho);
      fluxoAtual = melhorFluxo;
      ++passos;
      melhorou = true;
    }
  }

  double tempoMs = chrono::duration<double, milli>(
                       chrono::high_resolution_clock::now() - inicio)
                       .count();

  return {fluxoInicial, fluxoAtual, passos, std::move(atual), tempoMs};
}
