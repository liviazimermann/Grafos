// FluxoMaximo.cpp

#include "FluxoMaximo.h"
#include "Busca.h"
#include "GrafoMatriz.h"
#include <chrono>
#include <limits>
#include <vector>

using namespace std;

// ── Ford-Fulkerson ──────────────────────────────────────────────────────────────
//
// Mapeamento do pseudocodigo dos slides ("Os passos de cada iteracao"):
//   - Criar um grafo auxiliar como uma copia do grafo original  -> 'residual'
//   - Criar um inteiro S para solucao iniciado com 0            -> 'S'
//   - Enquanto existir caminho P de capacidade positiva s->t    -> dfsCaminho()
//       - Encontrar o menor arco A do caminho P                 -> 'A'
//       - Somar o valor de A a solucao S                        -> 'S += A'
//       - Para cada arco (u,v) no caminho P:
//           - Subtrair A no arco (u,v)
//           - Se existir arco (v,u): somar A nele
//           - Senao: criar arco (v,u) com valor A
//
// A copia residual e montada como GrafoMatriz (direcionado e ponderado): a matriz
// guarda a capacidade residual de cada arco, atualizar uma celula e direto e a
// criacao do arco reverso e trivial — sem risco de arcos paralelos. O grafo de
// entrada (capacidades reais) permanece intacto.

ResultadoFluxo FluxoMaximo::fordFulkerson(const Grafo &g, int origem, int destino)
{
  auto inicio = chrono::high_resolution_clock::now();

  int n = g.numVertices();

  // Copia auxiliar: rede residual como matriz. Comeca igual ao grafo original.
  auto residual = make_unique<GrafoMatriz>(true, true);
  for (int i = 0; i < n; ++i)
    residual->inserirVertice(g.labelVertice(i));
  for (int u = 0; u < n; ++u)
    for (int v : g.retornarVizinhos(u))
      residual->inserirAresta(u, v, g.pesoAresta(u, v));

  float S = 0.0f; // solucao (fluxo acumulado)
  int caminhos = 0;

  // Caso degenerado: origem/destino invalidos ou iguais -> fluxo 0.
  if (origem >= 0 && destino >= 0 && origem < n && destino < n && origem != destino)
  {
    vector<int> P;
    // Enquanto existir um caminho de capacidade positiva da fonte ao sorvedor
    while (dfsCaminho(*residual, origem, destino, P))
    {
      // Encontrar o menor arco A do caminho P (gargalo)
      float A = numeric_limits<float>::infinity();
      for (size_t k = 0; k + 1 < P.size(); ++k)
        A = min(A, residual->pesoAresta(P[k], P[k + 1]));

      // Somar A a solucao
      S += A;
      ++caminhos;

      // Atualizar a rede residual ao longo do caminho
      for (size_t k = 0; k + 1 < P.size(); ++k)
      {
        int u = P[k], v = P[k + 1];

        // Subtrair A no arco (u,v); se zerar, a celula vira 0 e o arco e ignorado
        residual->inserirAresta(u, v, residual->pesoAresta(u, v) - A);

        // Somar A no arco reverso (v,u). Se nao existir, pesoAresta devolve 0
        // e o arco e criado com valor A.
        residual->inserirAresta(v, u, residual->pesoAresta(v, u) + A);
      }
    }
  }

  double tempoMs = chrono::duration<double, milli>(
                       chrono::high_resolution_clock::now() - inicio)
                       .count();

  return {S, caminhos, tempoMs, std::move(residual)};
}
