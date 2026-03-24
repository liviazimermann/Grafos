#include <iostream>
#include <limits>
#include <memory>
#include "Grafo.h"
#include "GrafoLista.h"
#include "GrafoMatriz.h"
#include "GrafoIO.h"
#include "Busca.h"

// ── Utilitários ───────────────────────────────────────────────────────────────

static void limparBuffer()
{
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static int lerInt(const std::string &prompt)
{
  int v;
  std::cout << prompt;
  while (!(std::cin >> v))
  {
    std::cout << "Entrada invalida. Tente novamente: ";
    std::cin.clear();
    limparBuffer();
  }
  return v;
}

// ── Criação do grafo ──────────────────────────────────────────────────────────

static std::unique_ptr<Grafo> criarGrafo()
{
  while (true)
  {
    std::cout << "\n=== Novo Grafo ===\n"
              << "  1. Criar manualmente\n"
              << "  2. Carregar de arquivo\n"
              << "Escolha: ";
    int modo = lerInt("");

    if (modo == 2)
    {
      limparBuffer();
      std::cout << "Caminho do arquivo: ";
      std::string path;
      std::getline(std::cin, path);

      std::cout << "Representacao (1=Lista, 2=Matriz): ";
      bool usarLista = (lerInt("") == 1);

      try
      {
        auto g = carregarDeArquivo(path, usarLista);
        std::cout << "Carregado: " << g->numVertices() << " vertices, "
                  << g->numArestas() << " arestas.\n";
        return g;
      }
      catch (const std::exception &e)
      {
        std::cout << "Erro: " << e.what() << "\nTente novamente.\n";
        continue;
      }
    }

    // Criação manual
    std::cout << "Representacao:\n  1. Lista de adjacencia\n  2. Matriz de adjacencia\n";
    int rep = lerInt("Escolha: ");

    std::cout << "Direcionado?  0 = Nao  1 = Sim: ";
    bool dir = lerInt("") == 1;

    std::cout << "Ponderado?   0 = Nao  1 = Sim: ";
    bool pond = lerInt("") == 1;

    if (rep == 2)
    {
      std::cout << "[Usando GrafoMatriz]\n";
      return std::make_unique<GrafoMatriz>(dir, pond);
    }
    std::cout << "[Usando GrafoLista]\n";
    return std::make_unique<GrafoLista>(dir, pond);
  }
}

// ── Menu principal ────────────────────────────────────────────────────────────

static void exibirMenu()
{
  std::cout << "\n--- MENU ---\n"
            << " 1. Inserir vertice\n"
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
      std::cout << "Encerrando.\n";
      exit(0);
    }
    if (op == 0 || op == 13)
      return; // volta para criarGrafo()

    switch (op)
    {
    case 1:
    {
      limparBuffer();
      std::cout << "Label do vertice: ";
      std::string label;
      std::getline(std::cin, label);
      std::cout << (g.inserirVertice(label) ? "Inserido.\n" : "Falhou.\n");
      break;
    }
    case 2:
    {
      int idx = lerInt("Indice do vertice: ");
      std::cout << (g.removerVertice(idx) ? "Removido.\n" : "Indice invalido.\n");
      break;
    }
    case 3:
    {
      int o = lerInt("Origem: ");
      int d = lerInt("Destino: ");
      float p = 1.0f;
      if (g.ehPonderado())
      {
        std::cout << "Peso: ";
        std::cin >> p;
      }
      std::cout << (g.inserirAresta(o, d, p) ? "Aresta inserida.\n" : "Indices invalidos.\n");
      break;
    }
    case 4:
    {
      int o = lerInt("Origem: ");
      int d = lerInt("Destino: ");
      std::cout << (g.removerAresta(o, d) ? "Aresta removida.\n" : "Aresta nao encontrada.\n");
      break;
    }
    case 5:
    {
      int o = lerInt("Origem: ");
      int d = lerInt("Destino: ");
      std::cout << "Existe: " << (g.existeAresta(o, d) ? "Sim\n" : "Nao\n");
      break;
    }
    case 6:
    {
      int o = lerInt("Origem: ");
      int d = lerInt("Destino: ");
      float p = g.pesoAresta(o, d);
      if (p < 0)
        std::cout << "Aresta nao existe.\n";
      else
        std::cout << "Peso: " << p << "\n";
      break;
    }
    case 7:
    {
      int idx = lerInt("Indice do vertice: ");
      auto viz = g.retornarVizinhos(idx);
      if (viz.empty())
      {
        std::cout << "Nenhum vizinho.\n";
        break;
      }
      std::cout << "Vizinhos: ";
      for (int v : viz)
        std::cout << g.labelVertice(v) << "(" << v << ") ";
      std::cout << "\n";
      break;
    }
    case 8:
    {
      int idx = lerInt("Indice: ");
      std::string lbl = g.labelVertice(idx);
      std::cout << (lbl.empty() ? "Indice invalido.\n" : "Label: " + lbl + "\n");
      break;
    }
    case 9:
      std::cout << "Vertices: " << g.numVertices() << "\n";
      break;
    case 10:
      std::cout << "Arestas: " << g.numArestas() << "\n";
      break;
    case 11:
    {
      int idx = lerInt("Indice do vertice: ");
      int grau = g.grauVertice(idx);
      if (grau < 0)
        std::cout << "Indice invalido.\n";
      else
        std::cout << "Grau: " << grau << "\n";
      break;
    }
    case 12:
      std::cout << "\n";
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
        std::cout << "Dijkstra requer grafo ponderado.\n";
        break;
      }
      int orig = lerInt("Vertice de origem: ");
      dijkstra(g, orig);
      break;
    }
    default:
      std::cout << "Opcao invalida.\n";
    }
  }
}

// ── Entry point ───────────────────────────────────────────────────────────────

int main()
{
  std::cout << "=== Trabalho de Grafos - M1 ===\n";
  while (true)
  {
    auto grafo = criarGrafo();
    executarMenu(*grafo);
  }
}
