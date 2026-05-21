// testes.cpp
// Cobertura: Parte 1 (grafo) + Parte 2 (arquivo, BFS, DFS, Dijkstra) + Parte 3 (coloração) + Parte 5 (Prim) + Parte 6 (Kruskal)
//
// Arquivo de fixture usado: grafo_teste.txt (deve estar na mesma pasta do binário)
//   5 6 0 1
//   0 1 2.0 | 0 2 6.0 | 1 3 5.0 | 2 3 8.0 | 1 4 1.0 | 3 4 3.0
//   Não-direcionado, ponderado.

#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include "AGM.h"
#include "Busca.h"
#include "Coloracao.h"
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

// ── Parte 3: Coloração ────────────────────────────────────────────────────────

// Verifica se nenhum par de vizinhos tem a mesma cor.
static bool coloracaoValida(const Grafo &g, const vector<int> &cores)
{
  for (int v = 0; v < g.numVertices(); ++v)
    for (int viz : g.retornarVizinhos(v))
      if (cores[v] == cores[viz])
        return false;
  return true;
}

static void testarColoracao(bool usarLista)
{
  const string rep = usarLista ? "GrafoLista" : "GrafoMatriz";

  auto novoGrafo = [&]() -> unique_ptr<Grafo>
  {
    if (usarLista)
      return make_unique<GrafoLista>(false, false);
    return make_unique<GrafoMatriz>(false, false);
  };

  // ── Sem arestas (4 vértices) — espera 1 cor ──────────────────────────────
  secao("Coloracao: sem arestas (4 vertices) — " + rep);
  {
    auto g = novoGrafo();
    for (int i = 0; i < 4; ++i)
      g->inserirVertice(to_string(i));

    auto r = Coloracao::semOrdem(*g);
    checar("semOrdem: 1 cor", r.numCores == 1);
    checar("semOrdem: valida", coloracaoValida(*g, r.cores));
    r = Coloracao::welshPowell(*g);
    checar("welshPowell: 1 cor", r.numCores == 1);
    r = Coloracao::dsatur(*g);
    checar("dsatur: 1 cor", r.numCores == 1);
    r = Coloracao::forcaBruta(*g);
    checar("forcaBruta: 1 cor (otimo)", r.numCores == 1);
    checar("forcaBruta: valida", coloracaoValida(*g, r.cores));
  }

  // ── Triângulo K3 — espera 3 cores ────────────────────────────────────────
  secao("Coloracao: triangulo K3 — " + rep);
  {
    auto g = novoGrafo();
    g->inserirVertice("A");
    g->inserirVertice("B");
    g->inserirVertice("C");
    g->inserirAresta(0, 1);
    g->inserirAresta(1, 2);
    g->inserirAresta(0, 2);

    auto r = Coloracao::semOrdem(*g);
    checar("semOrdem: 3 cores", r.numCores == 3);
    checar("semOrdem: valida", coloracaoValida(*g, r.cores));
    r = Coloracao::welshPowell(*g);
    checar("welshPowell: 3 cores", r.numCores == 3);
    checar("welshPowell: valida", coloracaoValida(*g, r.cores));
    r = Coloracao::dsatur(*g);
    checar("dsatur: 3 cores", r.numCores == 3);
    checar("dsatur: valida", coloracaoValida(*g, r.cores));
    r = Coloracao::forcaBruta(*g);
    checar("forcaBruta: 3 cores (otimo)", r.numCores == 3);
    checar("forcaBruta: valida", coloracaoValida(*g, r.cores));
  }

  // ── Bipartido 6 vértices — espera 2 cores ────────────────────────────────
  //
  // Grupo A = {0,1,2}, Grupo B = {3,4,5}
  // Arestas: 0-3, 0-4, 1-3, 1-5, 2-4, 2-5
  secao("Coloracao: bipartido 6 vertices — " + rep);
  {
    auto g = novoGrafo();
    for (int i = 0; i < 6; ++i)
      g->inserirVertice(to_string(i));
    g->inserirAresta(0, 3);
    g->inserirAresta(0, 4);
    g->inserirAresta(1, 3);
    g->inserirAresta(1, 5);
    g->inserirAresta(2, 4);
    g->inserirAresta(2, 5);

    auto r = Coloracao::semOrdem(*g);
    checar("semOrdem: valida", coloracaoValida(*g, r.cores));
    checar("semOrdem: <= 2 cores", r.numCores <= 2);
    r = Coloracao::welshPowell(*g);
    checar("welshPowell: valida", coloracaoValida(*g, r.cores));
    checar("welshPowell: <= 2 cores", r.numCores <= 2);
    r = Coloracao::dsatur(*g);
    checar("dsatur: valida", coloracaoValida(*g, r.cores));
    checar("dsatur: <= 2 cores", r.numCores <= 2);
    r = Coloracao::forcaBruta(*g);
    checar("forcaBruta: 2 cores (otimo)", r.numCores == 2);
    checar("forcaBruta: valida", coloracaoValida(*g, r.cores));
  }

  // ── Grafo completo K4 — espera 4 cores ───────────────────────────────────
  secao("Coloracao: completo K4 — " + rep);
  {
    auto g = novoGrafo();
    for (int i = 0; i < 4; ++i)
      g->inserirVertice(to_string(i));
    g->inserirAresta(0, 1);
    g->inserirAresta(0, 2);
    g->inserirAresta(0, 3);
    g->inserirAresta(1, 2);
    g->inserirAresta(1, 3);
    g->inserirAresta(2, 3);

    auto r = Coloracao::semOrdem(*g);
    checar("semOrdem: 4 cores", r.numCores == 4);
    checar("semOrdem: valida", coloracaoValida(*g, r.cores));
    r = Coloracao::welshPowell(*g);
    checar("welshPowell: 4 cores", r.numCores == 4);
    checar("welshPowell: valida", coloracaoValida(*g, r.cores));
    r = Coloracao::dsatur(*g);
    checar("dsatur: 4 cores", r.numCores == 4);
    checar("dsatur: valida", coloracaoValida(*g, r.cores));
    r = Coloracao::forcaBruta(*g);
    checar("forcaBruta: 4 cores (otimo)", r.numCores == 4);
    checar("forcaBruta: valida", coloracaoValida(*g, r.cores));
  }

  // ── grafo_teste.txt — validade + número cromático = 3 ───────────────────
  //
  // O grafo contém o triângulo 1-3-4, portanto χ = 3.
  secao("Coloracao: grafo_teste.txt — " + rep);
  try
  {
    auto g = carregarDeArquivo(ARQUIVO, usarLista);

    auto r = Coloracao::semOrdem(*g);
    checar("semOrdem: valida", coloracaoValida(*g, r.cores));
    r = Coloracao::welshPowell(*g);
    checar("welshPowell: valida", coloracaoValida(*g, r.cores));
    r = Coloracao::dsatur(*g);
    checar("dsatur: valida", coloracaoValida(*g, r.cores));
    r = Coloracao::forcaBruta(*g);
    checar("forcaBruta: 3 cores (otimo)", r.numCores == 3);
    checar("forcaBruta: valida", coloracaoValida(*g, r.cores));
  }
  catch (const exception &e)
  {
    cout << "  [FALHOU] excecao: " << e.what() << "\n";
    falhou += 5;
  }
}

// ── Parte 4: fixtures do professor ───────────────────────────────────────────
//
// slides.txt          5v  7e  não-dir  ponderado  χ = 3
// slides_modificado.txt  6v  6e  não-dir  ponderado  χ = 3  (vértice 5 isolado)
// espacoaereo.txt   333v 2126e  não-dir  ponderado  só heurísticas (força bruta inviável)

static void testarColoracaoArquivos()
{
  // ── slides.txt ────────────────────────────────────────────────────────────
  secao("Coloracao: slides.txt (5v 7e) — GrafoLista");
  try
  {
    auto g = carregarDeArquivo("slides.txt", true);
    checar("numVertices == 5", g->numVertices() == 5);
    checar("numArestas == 7",  g->numArestas()  == 7);
    checar("ehPonderado",      g->ehPonderado());

    auto r = Coloracao::semOrdem(*g);
    checar("semOrdem: 3 cores", r.numCores == 3);
    checar("semOrdem: valida",  coloracaoValida(*g, r.cores));

    r = Coloracao::welshPowell(*g);
    checar("welshPowell: valida",    coloracaoValida(*g, r.cores));
    checar("welshPowell: <= 3 cores", r.numCores <= 3);

    r = Coloracao::dsatur(*g);
    checar("dsatur: valida",    coloracaoValida(*g, r.cores));
    checar("dsatur: <= 3 cores", r.numCores <= 3);

    r = Coloracao::forcaBruta(*g);
    checar("forcaBruta: 3 cores (otimo)", r.numCores == 3);
    checar("forcaBruta: valida",          coloracaoValida(*g, r.cores));
  }
  catch (const exception &e)
  {
    cout << "  [FALHOU] excecao: " << e.what() << "\n";
    falhou += 10;
  }

  // ── slides_modificado.txt ─────────────────────────────────────────────────
  secao("Coloracao: slides_modificado.txt (6v 6e) — GrafoLista");
  try
  {
    auto g = carregarDeArquivo("slides_modificado.txt", true);
    checar("numVertices == 6", g->numVertices() == 6);
    checar("numArestas == 6",  g->numArestas()  == 6);
    checar("ehPonderado",      g->ehPonderado());

    auto r = Coloracao::semOrdem(*g);
    checar("semOrdem: valida",    coloracaoValida(*g, r.cores));
    checar("semOrdem: <= 3 cores", r.numCores <= 3);

    r = Coloracao::welshPowell(*g);
    checar("welshPowell: valida",    coloracaoValida(*g, r.cores));
    checar("welshPowell: <= 3 cores", r.numCores <= 3);

    r = Coloracao::dsatur(*g);
    checar("dsatur: valida",    coloracaoValida(*g, r.cores));
    checar("dsatur: <= 3 cores", r.numCores <= 3);

    r = Coloracao::forcaBruta(*g);
    checar("forcaBruta: 3 cores (otimo)", r.numCores == 3);
    checar("forcaBruta: valida",          coloracaoValida(*g, r.cores));
  }
  catch (const exception &e)
  {
    cout << "  [FALHOU] excecao: " << e.what() << "\n";
    falhou += 10;
  }

  // ── espacoaereo.txt ───────────────────────────────────────────────────────
  // 333 vértices, 2126 arestas — força bruta inviável; só heurísticas.
  secao("Coloracao: espacoaereo.txt (333v 2126e) — GrafoLista");
  try
  {
    auto g = carregarDeArquivo("espacoaereo.txt", true);
    checar("numVertices == 333", g->numVertices() == 333);
    checar("numArestas == 2126", g->numArestas()  == 2126);
    checar("ehPonderado",        g->ehPonderado());

    auto r = Coloracao::semOrdem(*g);
    checar("semOrdem: valida", coloracaoValida(*g, r.cores));
    cout << "  [INFO]   semOrdem:    " << r.numCores << " cores  " << r.tempoMs << " ms\n";

    r = Coloracao::welshPowell(*g);
    checar("welshPowell: valida", coloracaoValida(*g, r.cores));
    cout << "  [INFO]   welshPowell: " << r.numCores << " cores  " << r.tempoMs << " ms\n";

    r = Coloracao::dsatur(*g);
    checar("dsatur: valida", coloracaoValida(*g, r.cores));
    cout << "  [INFO]   dsatur:      " << r.numCores << " cores  " << r.tempoMs << " ms\n";
  }
  catch (const exception &e)
  {
    cout << "  [FALHOU] excecao: " << e.what() << "\n";
    falhou += 6;
  }

  // ── GrafoMatriz: slides.txt ───────────────────────────────────────────────
  secao("Coloracao: slides.txt — GrafoMatriz");
  try
  {
    auto g = carregarDeArquivo("slides.txt", false);
    auto r = Coloracao::semOrdem(*g);
    checar("semOrdem: 3 cores", r.numCores == 3);
    checar("semOrdem: valida",  coloracaoValida(*g, r.cores));
    r = Coloracao::welshPowell(*g);
    checar("welshPowell: valida",     coloracaoValida(*g, r.cores));
    checar("welshPowell: <= 3 cores", r.numCores <= 3);
    r = Coloracao::dsatur(*g);
    checar("dsatur: valida",     coloracaoValida(*g, r.cores));
    checar("dsatur: <= 3 cores", r.numCores <= 3);
    r = Coloracao::forcaBruta(*g);
    checar("forcaBruta: 3 cores (otimo)", r.numCores == 3);
    checar("forcaBruta: valida",          coloracaoValida(*g, r.cores));
  }
  catch (const exception &e)
  {
    cout << "  [FALHOU] excecao: " << e.what() << "\n";
    falhou += 8;
  }

  // ── GrafoMatriz: slides_modificado.txt ───────────────────────────────────
  secao("Coloracao: slides_modificado.txt — GrafoMatriz");
  try
  {
    auto g = carregarDeArquivo("slides_modificado.txt", false);
    auto r = Coloracao::semOrdem(*g);
    checar("semOrdem: valida",    coloracaoValida(*g, r.cores));
    checar("semOrdem: <= 3 cores", r.numCores <= 3);
    r = Coloracao::welshPowell(*g);
    checar("welshPowell: valida",     coloracaoValida(*g, r.cores));
    checar("welshPowell: <= 3 cores", r.numCores <= 3);
    r = Coloracao::dsatur(*g);
    checar("dsatur: valida",     coloracaoValida(*g, r.cores));
    checar("dsatur: <= 3 cores", r.numCores <= 3);
    r = Coloracao::forcaBruta(*g);
    checar("forcaBruta: 3 cores (otimo)", r.numCores == 3);
    checar("forcaBruta: valida",          coloracaoValida(*g, r.cores));
  }
  catch (const exception &e)
  {
    cout << "  [FALHOU] excecao: " << e.what() << "\n";
    falhou += 8;
  }

  // ── GrafoMatriz: espacoaereo.txt ─────────────────────────────────────────
  secao("Coloracao: espacoaereo.txt — GrafoMatriz");
  try
  {
    auto g = carregarDeArquivo("espacoaereo.txt", false);
    auto r = Coloracao::semOrdem(*g);
    checar("semOrdem: valida", coloracaoValida(*g, r.cores));
    r = Coloracao::welshPowell(*g);
    checar("welshPowell: valida", coloracaoValida(*g, r.cores));
    r = Coloracao::dsatur(*g);
    checar("dsatur: valida", coloracaoValida(*g, r.cores));
  }
  catch (const exception &e)
  {
    cout << "  [FALHOU] excecao: " << e.what() << "\n";
    falhou += 3;
  }
}

// ── Parte 5: Prim ─────────────────────────────────────────────────────────────

static void testarPrim()
{
  auto arestasValidas = [](const Grafo &g, const ResultadoAGM &r) {
    for (const auto &a : r.arestas)
      if (!g.existeAresta(a.origem, a.destino))
        return false;
    return true;
  };

  // ── grafo_teste.txt — peso MST = 12 ─────────────────────────────────────
  //
  // MST: 0-1(2) + 1-4(1) + 4-3(3) + 0-2(6)  =  12
  secao("Prim: grafo_teste.txt (5v 6e, peso MST=12)");
  try
  {
    auto g = carregarDeArquivo(ARQUIVO, true);
    auto r = AGM::prim(*g, 0);
    checar("conexo",             r.conexo);
    checar("n-1 arestas",        (int)r.arestas.size() == g->numVertices() - 1);
    checar("peso total == 12",   fabsf(r.pesoTotal - 12.0f) < 1e-3f);
    checar("arestas existem",    arestasValidas(*g, r));
  }
  catch (const exception &e)
  {
    cout << "  [FALHOU] excecao: " << e.what() << "\n";
    falhou += 4;
  }

  // ── agm_teste.txt — peso MST = 14 ───────────────────────────────────────
  //
  // MST (Kruskal confirma): 0-3(2) + 1-5(2) + 1-4(3) + 2-5(3) + 3-5(4) = 14
  // Nota: o prompt do professor cita 18, mas esse valor corresponde a um
  // exemplo diferente dos slides. O MST correto para este grafo e 14.
  secao("Prim: agm_teste.txt (6v 10e, peso MST=14)");
  try
  {
    auto g = carregarDeArquivo("agm_teste.txt", true);
    auto r = AGM::prim(*g, 0);
    checar("conexo",             r.conexo);
    checar("n-1 arestas",        (int)r.arestas.size() == g->numVertices() - 1);
    checar("peso total == 14",   fabsf(r.pesoTotal - 14.0f) < 1e-3f);
    checar("arestas existem",    arestasValidas(*g, r));
  }
  catch (const exception &e)
  {
    cout << "  [FALHOU] excecao: " << e.what() << "\n";
    falhou += 4;
  }

  // ── agm_teste.txt — GrafoMatriz: mesmo resultado ─────────────────────────
  secao("Prim: agm_teste.txt — GrafoMatriz");
  try
  {
    auto g = carregarDeArquivo("agm_teste.txt", false);
    auto r = AGM::prim(*g, 0);
    checar("conexo",           r.conexo);
    checar("n-1 arestas",      (int)r.arestas.size() == g->numVertices() - 1);
    checar("peso total == 14", fabsf(r.pesoTotal - 14.0f) < 1e-3f);
  }
  catch (const exception &e)
  {
    cout << "  [FALHOU] excecao: " << e.what() << "\n";
    falhou += 3;
  }

  // ── agm_teste.txt — resultado independe do vertice inicial ───────────────
  secao("Prim: agm_teste.txt — vertices iniciais diferentes");
  try
  {
    auto g = carregarDeArquivo("agm_teste.txt", true);
    for (int v = 0; v < g->numVertices(); ++v)
    {
      auto r = AGM::prim(*g, v);
      checar("v=" + to_string(v) + " peso == 14", fabsf(r.pesoTotal - 14.0f) < 1e-3f);
    }
  }
  catch (const exception &e)
  {
    cout << "  [FALHOU] excecao: " << e.what() << "\n";
    falhou += 6;
  }

  // ── slides.txt — peso MST = 12 ───────────────────────────────────────────
  //
  // MST (Kruskal): 1-3(2) + 2-4(2) + 0-1(3) + 0-2(5) = 12
  secao("Prim: slides.txt (5v 7e, peso MST=12)");
  try
  {
    auto g = carregarDeArquivo("slides.txt", true);
    auto r = AGM::prim(*g, 0);
    checar("conexo",           r.conexo);
    checar("n-1 arestas",      (int)r.arestas.size() == g->numVertices() - 1);
    checar("peso total == 12", fabsf(r.pesoTotal - 12.0f) < 1e-3f);
    checar("arestas existem",  arestasValidas(*g, r));
  }
  catch (const exception &e)
  {
    cout << "  [FALHOU] excecao: " << e.what() << "\n";
    falhou += 4;
  }

  // ── slides_modificado.txt — grafo desconexo (vertice 5 isolado) ──────────
  //
  // Componente principal (0-4): peso MST = 15. Vertice 5 nao e alcancavel.
  secao("Prim: slides_modificado.txt (6v, vertice 5 isolado, MST componente=15)");
  try
  {
    auto g = carregarDeArquivo("slides_modificado.txt", true);
    auto r = AGM::prim(*g, 0);
    checar("nao conexo (v5 isolado)", !r.conexo);
    checar("4 arestas encontradas",   (int)r.arestas.size() == 4);
    checar("peso componente == 15",   fabsf(r.pesoTotal - 15.0f) < 1e-3f);
    checar("arestas existem",         arestasValidas(*g, r));
  }
  catch (const exception &e)
  {
    cout << "  [FALHOU] excecao: " << e.what() << "\n";
    falhou += 4;
  }

  // ── espacoaereo.txt — verifica validade e imprime tempo ──────────────────
  //
  // Vertice 0 e isolado (nao aparece em nenhuma aresta do arquivo).
  // Prim parte do vertice 1 para cobrir o componente conexo principal.
  secao("Prim: espacoaereo.txt (333v, vertice 0 isolado) — validade e tempo");
  try
  {
    auto g = carregarDeArquivo("espacoaereo.txt", true);
    auto r = AGM::prim(*g, 1);
    checar("encontrou arestas",   !r.arestas.empty());
    checar("arestas existem",     arestasValidas(*g, r));
    cout << "  [INFO]   Arestas: " << r.arestas.size()
         << "  Peso total: " << r.pesoTotal
         << "  Tempo: " << r.tempoMs << " ms\n";
  }
  catch (const exception &e)
  {
    cout << "  [FALHOU] excecao: " << e.what() << "\n";
    falhou += 2;
  }
}

// ── Parte 6: Kruskal ──────────────────────────────────────────────────────────

static void testarKruskal()
{
  auto arestasValidas = [](const Grafo &g, const ResultadoAGM &r) {
    for (const auto &a : r.arestas)
      if (!g.existeAresta(a.origem, a.destino))
        return false;
    return true;
  };

  // ── grafo_teste.txt — peso MST = 12 ─────────────────────────────────────
  secao("Kruskal: grafo_teste.txt (5v 6e, peso MST=12)");
  try
  {
    auto g = carregarDeArquivo(ARQUIVO, true);
    auto r = AGM::kruskal(*g);
    checar("conexo",           r.conexo);
    checar("n-1 arestas",      (int)r.arestas.size() == g->numVertices() - 1);
    checar("peso total == 12", fabsf(r.pesoTotal - 12.0f) < 1e-3f);
    checar("arestas existem",  arestasValidas(*g, r));
  }
  catch (const exception &e)
  {
    cout << "  [FALHOU] excecao: " << e.what() << "\n";
    falhou += 4;
  }

  // ── agm_teste.txt — peso MST = 14 ───────────────────────────────────────
  secao("Kruskal: agm_teste.txt (6v 10e, peso MST=14)");
  try
  {
    auto g = carregarDeArquivo("agm_teste.txt", true);
    auto r = AGM::kruskal(*g);
    checar("conexo",           r.conexo);
    checar("n-1 arestas",      (int)r.arestas.size() == g->numVertices() - 1);
    checar("peso total == 14", fabsf(r.pesoTotal - 14.0f) < 1e-3f);
    checar("arestas existem",  arestasValidas(*g, r));
  }
  catch (const exception &e)
  {
    cout << "  [FALHOU] excecao: " << e.what() << "\n";
    falhou += 4;
  }

  // ── agm_teste.txt — GrafoMatriz: mesmo resultado ─────────────────────────
  secao("Kruskal: agm_teste.txt — GrafoMatriz");
  try
  {
    auto g = carregarDeArquivo("agm_teste.txt", false);
    auto r = AGM::kruskal(*g);
    checar("conexo",           r.conexo);
    checar("n-1 arestas",      (int)r.arestas.size() == g->numVertices() - 1);
    checar("peso total == 14", fabsf(r.pesoTotal - 14.0f) < 1e-3f);
  }
  catch (const exception &e)
  {
    cout << "  [FALHOU] excecao: " << e.what() << "\n";
    falhou += 3;
  }

  // ── slides.txt — peso MST = 12 ───────────────────────────────────────────
  secao("Kruskal: slides.txt (5v 7e, peso MST=12)");
  try
  {
    auto g = carregarDeArquivo("slides.txt", true);
    auto r = AGM::kruskal(*g);
    checar("conexo",           r.conexo);
    checar("n-1 arestas",      (int)r.arestas.size() == g->numVertices() - 1);
    checar("peso total == 12", fabsf(r.pesoTotal - 12.0f) < 1e-3f);
    checar("arestas existem",  arestasValidas(*g, r));
  }
  catch (const exception &e)
  {
    cout << "  [FALHOU] excecao: " << e.what() << "\n";
    falhou += 4;
  }

  // ── slides_modificado.txt — grafo desconexo (vertice 5 isolado) ──────────
  secao("Kruskal: slides_modificado.txt (6v, vertice 5 isolado, MST componente=15)");
  try
  {
    auto g = carregarDeArquivo("slides_modificado.txt", true);
    auto r = AGM::kruskal(*g);
    checar("nao conexo (v5 isolado)", !r.conexo);
    checar("4 arestas encontradas",   (int)r.arestas.size() == 4);
    checar("peso componente == 15",   fabsf(r.pesoTotal - 15.0f) < 1e-3f);
    checar("arestas existem",         arestasValidas(*g, r));
  }
  catch (const exception &e)
  {
    cout << "  [FALHOU] excecao: " << e.what() << "\n";
    falhou += 4;
  }

  // ── Prim e Kruskal convergem no mesmo peso ────────────────────────────────
  secao("Prim == Kruskal: agm_teste.txt e grafo_teste.txt");
  try
  {
    for (const string &arq : {string("agm_teste.txt"), ARQUIVO})
    {
      auto g = carregarDeArquivo(arq, true);
      auto rPrim    = AGM::prim(*g, 0);
      auto rKruskal = AGM::kruskal(*g);
      checar(arq + ": mesmo peso total",
             fabsf(rPrim.pesoTotal - rKruskal.pesoTotal) < 1e-3f);
    }
  }
  catch (const exception &e)
  {
    cout << "  [FALHOU] excecao: " << e.what() << "\n";
    falhou += 2;
  }

  // ── espacoaereo.txt — verifica validade e imprime tempo ──────────────────
  secao("Kruskal: espacoaereo.txt (333v) — validade e tempo");
  try
  {
    auto g = carregarDeArquivo("espacoaereo.txt", true);
    auto r = AGM::kruskal(*g);
    checar("encontrou arestas", !r.arestas.empty());
    checar("arestas existem",   arestasValidas(*g, r));

    // Kruskal e Prim devem concordar no peso do componente principal
    auto rPrim = AGM::prim(*g, 1);
    checar("mesmo peso que Prim", fabsf(r.pesoTotal - rPrim.pesoTotal) < 1e-3f);

    cout << "  [INFO]   Arestas: " << r.arestas.size()
         << "  Peso total: " << r.pesoTotal
         << "  Tempo: " << r.tempoMs << " ms\n";
  }
  catch (const exception &e)
  {
    cout << "  [FALHOU] excecao: " << e.what() << "\n";
    falhou += 3;
  }
}

// ── Main ──────────────────────────────────────────────────────────────────────

int main()
{
  /*
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
  */
  // ── Parte 3 ──────────────────────────────────────────────────────────────
  cout << "\n========================================\n";
  cout << "  PARTE 3 — Coloracao\n";
  cout << "========================================\n";

  testarColoracao(true);
  testarColoracao(false);

  // ── Parte 4 ──────────────────────────────────────────────────────────────
  cout << "\n========================================\n";
  cout << "  PARTE 4 — Coloracao: fixtures do professor\n";
  cout << "========================================\n";

  testarColoracaoArquivos();

  // ── Parte 5 ──────────────────────────────────────────────────────────────
  cout << "\n========================================\n";
  cout << "  PARTE 5 — Prim (AGM)\n";
  cout << "========================================\n";

  testarPrim();

  // ── Parte 6 ──────────────────────────────────────────────────────────────
  cout << "\n========================================\n";
  cout << "  PARTE 6 — Kruskal (AGM)\n";
  cout << "========================================\n";

  testarKruskal();

  // ── Resultado ─────────────────────────────────────────────────────────────
  cout << "\n========================================\n";
  cout << "  RESULTADO: "
            << passou << " OK  |  "
            << falhou << " FALHOU\n";
  cout << "========================================\n";

  return falhou == 0 ? 0 : 1;
}
