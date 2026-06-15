#include "GrafoMatriz.h"
#include <iostream>
#include <iomanip>

using namespace std;

// ── Auxiliar ────────────────────────────────────────────────────────────────

bool GrafoMatriz::indiceValido(int i) const
{
  return i >= 0 && i < numVertices();
}

// ── Construtor ───────────────────────────────────────────────────────────────

GrafoMatriz::GrafoMatriz(bool direcionado, bool ponderado)
    : Grafo(direcionado, ponderado) {}

// ── Copia profunda ────────────────────────────────────────────────────────────

unique_ptr<Grafo> GrafoMatriz::clone() const
{
  auto copia = make_unique<GrafoMatriz>(direcionado, ponderado);
  copia->vertices = vertices; // copia os labels
  copia->matriz = matriz;     // copia a matriz inteira (deep copy de vector)
  return copia;
}

// ── Vértices ─────────────────────────────────────────────────────────────────

bool GrafoMatriz::inserirVertice(string label)
{
  int n = numVertices();

  // nova coluna em cada linha existente
  for (auto &linha : matriz)
    linha.push_back(0.0f);

  // nova linha inteira com zeros
  matriz.push_back(vector<float>(n + 1, 0.0f));

  vertices.push_back(label);
  return true;
}

bool GrafoMatriz::removerVertice(int idx)
{
  if (!indiceValido(idx))
    return false;

  // remove linha
  matriz.erase(matriz.begin() + idx);

  // remove coluna de cada linha restante
  for (auto &linha : matriz)
    linha.erase(linha.begin() + idx);

  vertices.erase(vertices.begin() + idx);
  return true;
}

string GrafoMatriz::labelVertice(int idx) const
{
  if (!indiceValido(idx))
    return "";
  return vertices[idx];
}

// ── Impressão ────────────────────────────────────────────────────────────────

void GrafoMatriz::imprimeGrafo() const
{
  int n = numVertices();

  // cabeçalho
  cout << "     ";
  for (int j = 0; j < n; ++j)
    cout << setw(6) << vertices[j];
  cout << "\n";

  for (int i = 0; i < n; ++i)
  {
    cout << setw(4) << vertices[i] << " ";
    for (int j = 0; j < n; ++j)
      cout << setw(6) << matriz[i][j];
    cout << "\n";
  }
}

// ── Arestas ──────────────────────────────────────────────────────────────────

bool GrafoMatriz::inserirAresta(int origem, int destino, float peso)
{
  if (!indiceValido(origem) || !indiceValido(destino))
    return false;

  float valor = ponderado ? peso : 1.0f;

  matriz[origem][destino] = valor;

  if (!direcionado)
    matriz[destino][origem] = valor;

  return true;
}

bool GrafoMatriz::removerAresta(int origem, int destino)
{
  if (!indiceValido(origem) || !indiceValido(destino))
    return false;

  matriz[origem][destino] = 0.0f;

  if (!direcionado)
    matriz[destino][origem] = 0.0f;

  return true;
}

bool GrafoMatriz::existeAresta(int origem, int destino) const
{
  if (!indiceValido(origem) || !indiceValido(destino))
    return false;
  return matriz[origem][destino] != 0.0f;
}

float GrafoMatriz::pesoAresta(int origem, int destino) const
{
  if (!indiceValido(origem) || !indiceValido(destino))
    return 0.0f;
  return matriz[origem][destino];
}

int GrafoMatriz::numArestas() const
{
  int total = 0;
  int n = numVertices();
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      if (matriz[i][j] != 0.0f)
        total++;

  // no grafo não direcionado a matriz é simétrica: cada aresta aparece duas vezes
  return direcionado ? total : total / 2;
}

int GrafoMatriz::grauVertice(int idx) const
{
  if (!indiceValido(idx))
    return -1;

  int n = numVertices();
  int grau = 0;

  for (int j = 0; j < n; ++j)
    if (matriz[idx][j] != 0.0f)
      grau++; // out-degree (ou grau simples se não direcionado)

  if (direcionado)
    for (int i = 0; i < n; ++i)
      if (matriz[i][idx] != 0.0f)
        grau++; // in-degree

  return grau;
}

// ── Vizinhos ─────────────────────────────────────────────────────────────────

vector<int> GrafoMatriz::retornarVizinhos(int vertice) const
{
  vector<int> vizinhos;
  if (!indiceValido(vertice))
    return vizinhos;

  for (int j = 0; j < numVertices(); ++j)
    if (matriz[vertice][j] != 0.0f)
      vizinhos.push_back(j);

  return vizinhos;
}
