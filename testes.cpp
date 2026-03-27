// testes.cpp
// Cobertura: Parte 1 (grafo) + Parte 2 (arquivo, BFS, DFS, Dijkstra)
//
// Arquivo de fixture usado: grafo_teste.txt (deve estar na mesma pasta do binário)
//   5 6 0 1
//   0 1 2.0 | 0 2 6.0 | 1 3 5.0 | 2 3 8.0 | 1 4 1.0 | 3 4 3.0
//   Não-direcionado, ponderado.

#include <iostream>
#include <string>
#include "Busca.h"
#include "Grafo.h"
#include "GrafoIO.h"
#include "GrafoLista.h"
#include "GrafoMatriz.h"

using namespace std;

// ── Utilidade ────────────────────────────────────────────────────────────────

static int passou = 0, falhou = 0;

static void checar(const string &desc, bool cond)
{
  if (cond)
  {
    cout << "  [OK]     " << desc << "\n";
    passou++;
  }
  else
  {
    cout << "  [FALHOU] " << desc << "\n";
    falhou++;
  }
}

static void secao(const string &titulo)
{
  cout << "\n=== " << titulo << " ===\n";
}

// ── Parte 1: suite genérica ───────────────────────────────────────────────────

static void rodarTestes(Grafo &g, const string &nome)
{
  secao(nome);

  checar("inserirVertice A", g.inserirVertice("A"));
  checar("inserirVertice B", g.inserirVertice("B"));
  checar("inserirVertice C", g.inserirVertice("C"));
  checar("numVertices == 3", g.numVertices() == 3);
  checar("labelVertice(0) == A", g.labelVertice(0) == "A");
  checar("labelVertice(1) == B", g.labelVertice(1) == "B");
  checar("labelVertice(2) == C", g.labelVertice(2) == "C");

  checar("inserirAresta A-B peso 3", g.inserirAresta(0, 1, 3.0f));
  checar("inserirAresta B-C peso 5", g.inserirAresta(1, 2, 5.0f));
  checar("numArestas == 2", g.numArestas() == 2);
  checar("existeAresta A-B", g.existeAresta(0, 1));
  checar("existeAresta B-A (simetrico)", g.existeAresta(1, 0));
  checar("nao existe A-C", !g.existeAresta(0, 2));
  checar("pesoAresta A-B == 3", g.pesoAresta(0, 1) == 3.0f);
  checar("pesoAresta B-C == 5", g.pesoAresta(1, 2) == 5.0f);

  auto viz = g.retornarVizinhos(1);
  checar("B tem 2 vizinhos", viz.size() == 2);
  checar("grauVertice(A) == 1", g.grauVertice(0) == 1);
  checar("grauVertice(B) == 2", g.grauVertice(1) == 2);
  checar("grauVertice(C) == 1", g.grauVertice(2) == 1);

  checar("removerAresta A-B", g.removerAresta(0, 1));
  checar("aresta A-B sumiu", !g.existeAresta(0, 1));
  checar("numArestas == 1", g.numArestas() == 1);

  g.inserirAresta(0, 1, 3.0f);

  checar("removerVertice B", g.removerVertice(1));
  checar("numVertices == 2", g.numVertices() == 2);
  checar("numArestas == 0", g.numArestas() == 0);
  checar("labelVertice(0) ainda A", g.labelVertice(0) == "A");
  checar("labelVertice(1) agora C", g.labelVertice(1) == "C");

  checar("labelVertice(-1) vazio", g.labelVertice(-1).empty());
  checar("grauVertice(99) == -1", g.grauVertice(99) == -1);
  checar("inserirAresta idx invalido", !g.inserirAresta(0, 99));
  checar("removerVertice idx invalido", !g.removerVertice(99));

  cout << "  [VISUAL] imprimeGrafo():\n";
  g.imprimeGrafo();
}

static void rodarTestesDirecionado(Grafo &g, const string &nome)
{
  secao(nome + " (direcionado)");

  g.inserirVertice("X");
  g.inserirVertice("Y");
  g.inserirVertice("Z");
  g.inserirAresta(0, 1);
  g.inserirAresta(0, 2);

  checar("existeAresta X->Y", g.existeAresta(0, 1));
  checar("nao existe Y->X (dirigido)", !g.existeAresta(1, 0));
  checar("numArestas == 2", g.numArestas() == 2);
  checar("grauVertice(X) == 2 (saida)", g.grauVertice(0) == 2);
  checar("grauVertice(Y) == 1 (entrada)", g.grauVertice(1) == 1);
}

// ── Parte 2: leitura de arquivo ───────────────────────────────────────────────
//
// grafo_teste.txt:
//   5 6 0 1
//   0 1 2.0
//   0 2 6.0
//   1 3 5.0
//   2 3 8.0
//   1 4 1.0
//   3 4 3.0
//
// Grafo não-direcionado ponderado com 5 vértices e 6 arestas.

static const string ARQUIVO = "grafo_teste.txt";

static void testarCarregarArquivo(bool usarLista)
{
  const string rep = usarLista ? "GrafoLista" : "GrafoMatriz";
  secao("carregarDeArquivo — " + rep);

  try
  {
    auto g = carregarDeArquivo(ARQUIVO, usarLista);

    checar("numVertices == 5", g->numVertices() == 5);
    checar("numArestas == 6", g->numArestas() == 6);
    checar("ehPonderado == true", g->ehPonderado());

    // arestas presentes
    checar("existeAresta 0-1", g->existeAresta(0, 1));
    checar("existeAresta 1-0 (simetrico)", g->existeAresta(1, 0));
    checar("existeAresta 0-2", g->existeAresta(0, 2));
    checar("existeAresta 1-3", g->existeAresta(1, 3));
    checar("existeAresta 2-3", g->existeAresta(2, 3));
    checar("existeAresta 1-4", g->existeAresta(1, 4));
    checar("existeAresta 3-4", g->existeAresta(3, 4));

    // aresta ausente
    checar("nao existe 0-4", !g->existeAresta(0, 4));

    // pesos
    checar("pesoAresta 0-1 == 2", g->pesoAresta(0, 1) == 2.0f);
    checar("pesoAresta 0-2 == 6", g->pesoAresta(0, 2) == 6.0f);
    checar("pesoAresta 1-4 == 1", g->pesoAresta(1, 4) == 1.0f);
    checar("pesoAresta 3-4 == 3", g->pesoAresta(3, 4) == 3.0f);
  }
  catch (const exception &e)
  {
    cout << "  [FALHOU] excecao ao abrir arquivo: " << e.what() << "\n";
    cout << "  >> Certifique-se de que grafo_teste.txt esta na pasta do binario.\n";
    falhou += 14;
  }
}

static void testarArquivoInexistente()
{
  secao("carregarDeArquivo — arquivo inexistente");
  bool lancou = false;
  try
  {
    carregarDeArquivo("nao_existe_xyz.txt", true);
  }
  catch (const exception &)
  {
    lancou = true;
  }
  checar("lanca excecao para arquivo inexistente", lancou);
}

// ── Parte 2: BFS e DFS (saída visual sobre grafo_teste.txt) ──────────────────
//
// Grafo carregado do arquivo. Buscas a partir do vértice 0.
// Resultado esperado (BFS): 0 1 2 3 4   (largura, pesos ignorados)
// Resultado esperado (DFS): 0 1 3 2 4   (pode variar por ordem de vizinhos)

static void testarBuscas(bool usarLista)
{
  const string rep = usarLista ? "GrafoLista" : "GrafoMatriz";
  secao("BFS / DFS — " + rep + " (saida visual, origem=0)");

  try
  {
    auto g = carregarDeArquivo(ARQUIVO, usarLista);

    cout << "  BFS: ";
    bfs(*g, 0);

    cout << "  DFS: ";
    dfs(*g, 0);

    cout << "  BFS origem invalida (sem crash): ";
    bfs(*g, 99);

    checar("buscas executaram sem excecao", true);
  }
  catch (const exception &e)
  {
    cout << "  [FALHOU] excecao: " << e.what() << "\n";
    falhou++;
  }
}

// ── Parte 2: Dijkstra (saída visual sobre grafo_teste.txt) ───────────────────
//
// Dijkstra a partir do vértice 0 no grafo_teste.txt.
// Resultados esperados:
//   0 -> 1 : dist=2   caminho: 0 -> 1
//   0 -> 2 : dist=6   caminho: 0 -> 2
//   0 -> 4 : dist=3   caminho: 0 -> 1 -> 4
//   0 -> 3 : dist=6   caminho: 0 -> 1 -> 4 -> 3

static void testarDijkstra(bool usarLista)
{
  const string rep = usarLista ? "GrafoLista" : "GrafoMatriz";
  secao("Dijkstra — " + rep + " (saida visual, origem=0)");
  cout << "  Esperado: 1=2 | 2=6 | 4=3 | 3=6\n";

  try
  {
    auto g = carregarDeArquivo(ARQUIVO, usarLista);
    dijkstra(*g, 0);
    checar("dijkstra executou sem excecao", true);
  }
  catch (const exception &e)
  {
    cout << "  [FALHOU] excecao: " << e.what() << "\n";
    falhou++;
  }
}

// ── Main ──────────────────────────────────────────────────────────────────────

int main()
{
  // ── Parte 1 ──────────────────────────────────────────────────────────────
  cout << "========================================\n";
  cout << "  PARTE 1 — Estrutura do Grafo\n";
  cout << "========================================\n";

  GrafoLista lista(false, true);
  GrafoMatriz matriz(false, true);
  rodarTestes(lista, "GrafoLista");
  rodarTestes(matriz, "GrafoMatriz");

  GrafoLista listaDir(true, false);
  GrafoMatriz matrizDir(true, false);
  rodarTestesDirecionado(listaDir, "GrafoLista");
  rodarTestesDirecionado(matrizDir, "GrafoMatriz");

  // ── Parte 2 ──────────────────────────────────────────────────────────────
  cout << "\n========================================\n";
  cout << "  PARTE 2 — Arquivo + Algoritmos\n";
  cout << "  fixture: " << ARQUIVO << "\n";
  cout << "========================================\n";

  testarCarregarArquivo(true);
  testarCarregarArquivo(false);
  testarArquivoInexistente();

  testarBuscas(true);
  testarBuscas(false);

  testarDijkstra(true);
  testarDijkstra(false);

  // ── Resultado ─────────────────────────────────────────────────────────────
  cout << "\n========================================\n";
  cout << "  RESULTADO: "
            << passou << " OK  |  "
            << falhou << " FALHOU\n";
  cout << "========================================\n";

  return falhou == 0 ? 0 : 1;
}
