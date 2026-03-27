# Grafos em C++

Implementação de grafos com lista e matriz de adjacência, com suporte a BFS, DFS e Dijkstra.

---

## Pré-requisitos

- Compilador `g++` com suporte a **C++17**

### Instalação do g++

**Linux (Ubuntu/Debian):**
```bash
sudo apt update && sudo apt install build-essential
```

**macOS:**
```bash
xcode-select --install
```

**Windows:**
Instale o [MinGW-w64](https://www.mingw-w64.org/) e adicione ao PATH.

---

## Estrutura de arquivos

```
.
├── Grafo.h
├── GrafoLista.h / GrafoLista.cpp
├── GrafoMatriz.h / GrafoMatriz.cpp
├── GrafoIO.h / GrafoIO.cpp
├── Busca.h / Busca.cpp
├── main.cpp
└── testes.cpp
```

---

## Como compilar

### Programa principal

```bash
g++ -std=c++17 -Wall -o grafo \
    main.cpp GrafoLista.cpp GrafoMatriz.cpp GrafoIO.cpp Busca.cpp
```

### Testes automatizados

```bash
g++ -std=c++17 -Wall -o testes \
    testes.cpp GrafoLista.cpp GrafoMatriz.cpp GrafoIO.cpp Busca.cpp
```

---

## Como executar

```bash
./grafo     # programa principal (Linux/macOS)
./testes    # testes automatizados
```

```bash
grafo.exe   # Windows
testes.exe
```

---

## Formato de arquivo de grafo

O programa aceita carregar grafos a partir de arquivos `.txt` no seguinte formato:

```
V A D P
Ao Ad [Ap]
Ao Ad [Ap]
...
```

| Campo | Descrição |
|-------|-----------|
| `V`   | Número de vértices |
| `A`   | Número de arestas |
| `D`   | Direcionado: `0` = não, `1` = sim |
| `P`   | Ponderado: `0` = não, `1` = sim |
| `Ao`  | Índice do vértice de origem |
| `Ad`  | Índice do vértice de destino |
| `Ap`  | Peso da aresta (somente se `P = 1`) |

**Exemplo** — grafo não direcionado, ponderado, 3 vértices, 2 arestas:

```
3 2 0 1
0 1 5.0
1 2 3.0
```
