// GrafoIO.h
// Responsabilidade: carregar um Grafo a partir de um arquivo de texto.
// Formato esperado:
//   V A D P
//   Ao Ad [Ap]   (A linhas)

#pragma once

#include "Grafo.h"
#include "GrafoLista.h"
#include "GrafoMatriz.h"

#include <memory>
#include <string>

// Lê o arquivo e retorna um grafo pronto.
// usarLista == true  -> GrafoLista
// usarLista == false -> GrafoMatriz
// Lança std::runtime_error em caso de falha.
std::unique_ptr<Grafo> carregarDeArquivo(const std::string &caminho,
                                         bool usarLista);
