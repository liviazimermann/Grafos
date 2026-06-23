// Simulacao.h
// Simulação epidêmica SIR sobre qualquer Grafo.
// Cada vértice tem estado: S (suscetível), I (infectado), R (recuperado).

#pragma once

#include "Grafo.h"
#include <vector>
#include <string>

enum class EstadoSIR { S, I, R };

struct PassoSIR {
    int passo;
    int totalS;
    int totalI;
    int totalR;
};

struct ResultadoSIR {
    std::vector<PassoSIR> historico;  // evolução a cada passo de tempo
    int totalPassos;
    double tempoMs;
};

// Imprime resultado: passos, pico de infectados, tempo total.
// Se grafo tiver < 10 vértices, imprime estado final de cada vértice.
void imprimirResultadoSIR(const ResultadoSIR& r, int numVertices);

class Simulacao {
public:
    // Executa simulação SIR no grafo g.
    // beta  = probabilidade de transmissão por aresta por passo
    // gamma = probabilidade de recuperação por passo
    // semente = vértice inicial infectado (padrão: 0)
    static ResultadoSIR executarSIR(const Grafo& g,
                                     double beta,
                                     double gamma,
                                     int semente = 0);
};
