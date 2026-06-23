// Simulacao.cpp

#include "Simulacao.h"
#include <iostream>
#include <iomanip>
#include <random>
#include <chrono>

void imprimirResultadoSIR(const ResultadoSIR& r, int numVertices) {
    std::cout << "\n=== Resultado SIR ===\n";
    std::cout << "Passos simulados : " << r.totalPassos << "\n";
    std::cout << "Tempo de execucao: " << std::fixed << std::setprecision(3)
              << r.tempoMs << " ms\n";

    // Encontrar pico de infectados
    int picoI = 0;
    int passoPico = 0;
    for (const auto& p : r.historico) {
        if (p.totalI > picoI) {
            picoI = p.totalI;
            passoPico = p.passo;
        }
    }
    std::cout << "Pico de infectados: " << picoI
              << " (passo " << passoPico << ")\n";

    // Estado final
    if (!r.historico.empty()) {
        const auto& fim = r.historico.back();
        std::cout << "Estado final -> S: " << fim.totalS
                  << " | I: " << fim.totalI
                  << " | R: " << fim.totalR << "\n";
    }

    // Mostrar evolução passo a passo apenas para grafos pequenos
    if (numVertices < 10) {
        std::cout << "\nEvolucao por passo:\n";
        std::cout << std::setw(6) << "Passo"
                  << std::setw(6) << "S"
                  << std::setw(6) << "I"
                  << std::setw(6) << "R" << "\n";
        for (const auto& p : r.historico) {
            std::cout << std::setw(6) << p.passo
                      << std::setw(6) << p.totalS
                      << std::setw(6) << p.totalI
                      << std::setw(6) << p.totalR << "\n";
        }
    }
}

ResultadoSIR Simulacao::executarSIR(const Grafo& g,
                                      double beta,
                                      double gamma,
                                      int semente) {
    auto inicio = std::chrono::high_resolution_clock::now();

    int n = g.numVertices();
    std::vector<EstadoSIR> estado(n, EstadoSIR::S);

    // Validar semente
    if (semente < 0 || semente >= n) semente = 0;
    estado[semente] = EstadoSIR::I;

    // Gerador de números aleatórios
    std::mt19937 rng(42);  // semente fixa para reprodutibilidade
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    ResultadoSIR resultado;
    int maxPassos = n * 10;  // limite de segurança

    for (int t = 0; t < maxPassos; ++t) {
        // Contar estados atuais
        int cS = 0, cI = 0, cR = 0;
        for (int v = 0; v < n; ++v) {
            if (estado[v] == EstadoSIR::S) ++cS;
            else if (estado[v] == EstadoSIR::I) ++cI;
            else ++cR;
        }
        resultado.historico.push_back({t, cS, cI, cR});

        // Parar se não há infectados
        if (cI == 0) break;

        // Próximo estado (atualização síncrona)
        std::vector<EstadoSIR> proximo = estado;

        for (int v = 0; v < n; ++v) {
            if (estado[v] != EstadoSIR::I) continue;

            // Tentar infectar vizinhos suscetíveis
            std::vector<int> vizinhos = g.retornarVizinhos(v);
            for (int u : vizinhos) {
                if (estado[u] == EstadoSIR::S && dist(rng) < beta) {
                    proximo[u] = EstadoSIR::I;
                }
            }

            // Tentar se recuperar
            if (dist(rng) < gamma) {
                proximo[v] = EstadoSIR::R;
            }
        }

        estado = proximo;
    }

    resultado.totalPassos = static_cast<int>(resultado.historico.size());

    auto fim = std::chrono::high_resolution_clock::now();
    resultado.tempoMs = std::chrono::duration<double, std::milli>(fim - inicio).count();

    return resultado;
}
