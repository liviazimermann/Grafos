#include <iostream>
#include <limits>
#include <memory>
#include "Grafo.h"
#include "GrafoLista.h"
#include "GrafoMatriz.h"
#include "GrafoIO.h"
#include "AGM.h"
#include "Busca.h"
#include "Coloracao.h"

using namespace std;

// ── Utilitários ───────────────────────────────────────────────────────────────

static void limparBuffer()
{
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static int lerInt(const string &prompt)
{
  int v;
  cout << prompt;
  while (!(cin >> v))
  {
    cout << "Entrada invalida. Tente novamente: ";
    cin.clear();
    limparBuffer();
  }
  return v;
}

// ── Criação do grafo ──────────────────────────────────────────────────────────

static unique_ptr<Grafo> criarGrafo()
{
  while (true)
  {
    cout << "\n=== Novo Grafo ===\n"
              << "  1. Criar manualmente\n"
              << "  2. Carregar de arquivo\n"
              << "Escolha: ";
    int modo = lerInt("");

    if (modo == 2)
    {
      limparBuffer();
      cout << "Caminho do arquivo: ";
      string path;
      getline(cin, path);

      cout << "Representacao (1=Lista, 2=Matriz): ";
      bool usarLista = (lerInt("") == 1);

      try
      {
        auto g = carregarDeArquivo(path, usarLista);
        cout << "Carregado: " << g->numVertices() << " vertices, "
                  << g->numArestas() << " arestas.\n";
        return g;
      }
      catch (const exception &e)
      {
        cout << "Erro: " << e.what() << "\nTente novamente.\n";
        continue;
      }
    }

    // Criação manual
    cout << "Representacao:\n  1. Lista de adjacencia\n  2. Matriz de adjacencia\n";
    int rep = lerInt("Escolha: ");

    cout << "Direcionado?  0 = Nao  1 = Sim: ";
    bool dir = lerInt("") == 1;

    cout << "Ponderado?   0 = Nao  1 = Sim: ";
    bool pond = lerInt("") == 1;

    if (rep == 2)
    {
      cout << "[Usando GrafoMatriz]\n";
      return make_unique<GrafoMatriz>(dir, pond);
    }
    cout << "[Usando GrafoLista]\n";
    return make_unique<GrafoLista>(dir, pond);
  }
}

// ── AGM ───────────────────────────────────────────────────────────────────────

static void imprimirAGM(const Grafo &g, const string &nome, const ResultadoAGM &r)
{
  cout << "\n[" << nome << "]\n";
  if (!r.conexo)
    cout << "  Aviso: grafo nao conexo — AGM incompleta.\n";
  for (const auto &a : r.arestas)
    cout << "  " << g.labelVertice(a.origem) << " -- "
         << g.labelVertice(a.destino) << "  peso=" << a.peso << "\n";
  cout << "  Peso total : " << r.pesoTotal << "\n"
       << "  Arestas    : " << r.arestas.size() << "\n"
       << "  Tempo      : " << r.tempoMs << " ms\n";
}

// ── Coloração ─────────────────────────────────────────────────────────────────

static void imprimirColoracao(const Grafo &g, const string &nome,
                              const ResultadoColoracao &r)
{
  cout << "\n[" << nome << "]\n"
       << "  Cores usadas : " << r.numCores << "\n"
       << "  Tempo        : " << r.tempoMs << " ms\n";
  if (g.numVertices() < 10)
    for (int v = 0; v < g.numVertices(); ++v)
      cout << "  Vertice " << g.labelVertice(v) << " -> cor " << r.cores[v] << "\n";
}

// ── Menu principal ────────────────────────────────────────────────────────────

static void exibirMenu()
{
  cout << "\n--- MENU ---\n"
            /*<< " 1. Inserir vertice\n"
            << " 2. Remover vertice\n"
            << " 3. Inserir aresta\n"
            << " 4. Remover aresta\n"
            << " 5. Verificar aresta\n"
            << " 6. Peso de aresta\n"
            << " 7. Vizinhos de vertice\n"
            << " 8. Label de vertice\n"
            << " 9. Numero de vertices\n"
            << "10. Numero de arestas\n"
            << "11. Grau de vertice\n"
            << "12. Imprimir grafo\n"
            << "13. Recarregar / Trocar grafo\n"
            << "14. BFS (busca em largura)\n"
            << "15. DFS (busca em profundidade)\n"
            << "16. Dijkstra (menor caminho)\n"
            */
            << "17. Coloracao (Sem Ordem, Welsh-Powell, DSatur)\n"
            << "18. Coloracao - Forca Bruta (so grafos pequenos!)\n"
            << "19. Prim (Arvore Geradora Minima)\n"
            << "20. Kruskal (Arvore Geradora Minima)\n"
            << " 0. Reiniciar (novo grafo)\n"
            << "-1. Sair\n"
            << "Opcao: ";
}

// ── Loop de interação ─────────────────────────────────────────────────────────

static void executarMenu(Grafo &g)
{
  int op;
  while (true)
  {
    exibirMenu();
    op = lerInt("");

    if (op == -1)
    {
      cout << "Encerrando.\n";
      exit(0);
    }
    if (op == 0 || op == 13)
      return; // volta para criarGrafo()

    switch (op)
    {
    case 1:
    {
      limparBuffer();
      cout << "Label do vertice: ";
      string label;
      getline(cin, label);
      cout << (g.inserirVertice(label) ? "Inserido.\n" : "Falhou.\n");
      break;
    }
    case 2:
    {
      int idx = lerInt("Indice do vertice: ");
      cout << (g.removerVertice(idx) ? "Removido.\n" : "Indice invalido.\n");
      break;
    }
    case 3:
    {
      int o = lerInt("Origem: ");
      int d = lerInt("Destino: ");
      float p = 1.0f;
      if (g.ehPonderado())
      {
        cout << "Peso: ";
        cin >> p;
      }
      cout << (g.inserirAresta(o, d, p) ? "Aresta inserida.\n" : "Indices invalidos.\n");
      break;
    }
    case 4:
    {
      int o = lerInt("Origem: ");
      int d = lerInt("Destino: ");
      cout << (g.removerAresta(o, d) ? "Aresta removida.\n" : "Aresta nao encontrada.\n");
      break;
    }
    case 5:
    {
      int o = lerInt("Origem: ");
      int d = lerInt("Destino: ");
      cout << "Existe: " << (g.existeAresta(o, d) ? "Sim\n" : "Nao\n");
      break;
    }
    case 6:
    {
      int o = lerInt("Origem: ");
      int d = lerInt("Destino: ");
      float p = g.pesoAresta(o, d);
      if (p < 0)
        cout << "Aresta nao existe.\n";
      else
        cout << "Peso: " << p << "\n";
      break;
    }
    case 7:
    {
      int idx = lerInt("Indice do vertice: ");
      auto viz = g.retornarVizinhos(idx);
      if (viz.empty())
      {
        cout << "Nenhum vizinho.\n";
        break;
      }
      cout << "Vizinhos: ";
      for (int v : viz)
        cout << g.labelVertice(v) << "(" << v << ") ";
      cout << "\n";
      break;
    }
    case 8:
    {
      int idx = lerInt("Indice: ");
      string lbl = g.labelVertice(idx);
      cout << (lbl.empty() ? "Indice invalido.\n" : "Label: " + lbl + "\n");
      break;
    }
    case 9:
      cout << "Vertices: " << g.numVertices() << "\n";
      break;
    case 10:
      cout << "Arestas: " << g.numArestas() << "\n";
      break;
    case 11:
    {
      int idx = lerInt("Indice do vertice: ");
      int grau = g.grauVertice(idx);
      if (grau < 0)
        cout << "Indice invalido.\n";
      else
        cout << "Grau: " << grau << "\n";
      break;
    }
    case 12:
      cout << "\n";
      g.imprimeGrafo();
      break;
    case 14:
    {
      int orig = lerInt("Vertice de origem: ");
      bfs(g, orig);
      break;
    }
    case 15:
    {
      int orig = lerInt("Vertice de origem: ");
      dfs(g, orig);
      break;
    }
    case 16:
    {
      if (!g.ehPonderado())
      {
        cout << "Dijkstra requer grafo ponderado.\n";
        break;
      }
      int orig = lerInt("Vertice de origem: ");
      dijkstra(g, orig);
      break;
    }
    case 17:
    {
      imprimirColoracao(g, "Sem Ordem",    Coloracao::semOrdem(g));
      imprimirColoracao(g, "Welsh-Powell", Coloracao::welshPowell(g));
      imprimirColoracao(g, "DSatur",       Coloracao::dsatur(g));
      break;
    }
    case 18:
    {
      if (g.numVertices() > 20)
        cout << "Aviso: forca bruta em grafos grandes pode demorar muito.\n";
      imprimirColoracao(g, "Forca Bruta", Coloracao::forcaBruta(g));
      break;
    }
    case 19:
    {
      if (!g.ehPonderado())
      {
        cout << "Prim requer grafo ponderado.\n";
        break;
      }
      int orig = lerInt("Vertice inicial (0 a " + to_string(g.numVertices() - 1) + "): ");
      if (orig < 0 || orig >= g.numVertices())
      {
        cout << "Vertice invalido.\n";
        break;
      }
      imprimirAGM(g, "Prim", AGM::prim(g, orig));
      break;
    }
    case 20:
    {
      if (!g.ehPonderado())
      {
        cout << "Kruskal requer grafo ponderado.\n";
        break;
      }
      imprimirAGM(g, "Kruskal", AGM::kruskal(g));
      break;
    }
    default:
      cout << "Opcao invalida.\n";
    }
  }
}

// ── Entry point ───────────────────────────────────────────────────────────────

int main()
{
  cout << "=== Trabalho de Grafos - M1 ===\n";
  while (true)
  {
    auto grafo = criarGrafo();
    executarMenu(*grafo);
  }
}
