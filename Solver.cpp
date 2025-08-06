#include "Solver.h"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <stack>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <thread>
#include <climits> // Para INT_MAX


// --- Timeout padrão para todos algoritmos (em segundos) ---
const double TIMEOUT_PADRAO = 10.0;

// --- Função auxiliar para checar timeout ---
bool estourouTimeout(const std::chrono::high_resolution_clock::time_point& start, double timeout) {
    auto now = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double>(now - start).count();
    return elapsed > timeout;
}

std::string Solver::tabuleiroParaString(const std::vector<char>& tabuleiro) {
    std::string resultado;
    for (char c : tabuleiro) {
        resultado += c;
    }
    return resultado;
}

std::vector<char> Solver::stringParaTabuleiro(const std::string& str) {
    std::vector<char> resultado;
    for (char c : str) {
        resultado.push_back(c);
    }
    return resultado;
}

int Solver::encontrarEspacoVazio(const std::vector<char>& tabuleiro) {
    for (size_t i = 0; i < tabuleiro.size(); i++) {
        if (tabuleiro[i] == '_') {
            return i;
        }
    }
    return -1;
}

std::vector<int> Solver::encontrarMovimentosPossiveis(const std::vector<char>& tabuleiro) {
    std::vector<int> movimentos;
    int espacoVazio = encontrarEspacoVazio(tabuleiro);
    if (espacoVazio == -1) return movimentos;
    if (espacoVazio > 0) movimentos.push_back(espacoVazio - 1);
    if (espacoVazio < (int)tabuleiro.size() - 1) movimentos.push_back(espacoVazio + 1);
    if (espacoVazio > 1 && tabuleiro[espacoVazio - 1] != '_') movimentos.push_back(espacoVazio - 2);
    if (espacoVazio < (int)tabuleiro.size() - 2 && tabuleiro[espacoVazio + 1] != '_') movimentos.push_back(espacoVazio + 2);
    return movimentos;
}

std::vector<char> Solver::aplicarMovimento(const std::vector<char>& tabuleiro, int posicao) {
    std::vector<char> novoTabuleiro = tabuleiro;
    int espacoVazio = encontrarEspacoVazio(tabuleiro);
    if (espacoVazio == -1) return novoTabuleiro;
    
    // Verificar se o movimento é válido
    if (posicao < 0 || posicao >= (int)tabuleiro.size() || tabuleiro[posicao] == '_') {
        return novoTabuleiro;
    }
    
    // Verificar se pode mover para o espaço vazio
    if (posicao == espacoVazio - 1 || posicao == espacoVazio + 1) {
        // Movimento direto
        std::swap(novoTabuleiro[posicao], novoTabuleiro[espacoVazio]);
    } else if (posicao == espacoVazio - 2 && tabuleiro[espacoVazio - 1] != '_') {
        // Pulo para esquerda
        std::swap(novoTabuleiro[posicao], novoTabuleiro[espacoVazio]);
    } else if (posicao == espacoVazio + 2 && tabuleiro[espacoVazio + 1] != '_') {
        // Pulo para direita
        std::swap(novoTabuleiro[posicao], novoTabuleiro[espacoVazio]);
    }
    
    return novoTabuleiro;
}

// bool Solver::verificarVitoria(const std::vector<char>& tabuleiro) {
//     std::vector<char> v = tabuleiro;
//     v.erase(std::remove(v.begin(), v.end(), '_'), v.end());
//     int numFichas = v.size()/2;
//     if(v[0] == 'A'){
//         for(int i = 0; i < numFichas; i++){
//             if(v[i] != 'A'){
//                 return false;
//             }
//         }
//     }
//     else if(v[0] == 'B'){
//         for(int i = 0; i < numFichas; i++){
//             if(v[i] != 'B'){
//                 return false;
//             }
//         }
//     }
//     return true; // Nenhuma azul depois de vermelha → vitória!
// }

bool Solver::verificarVitoria(const std::vector<char>& tabuleiro) {
   std::vector<char> v = tabuleiro;
   v.erase(std::remove(v.begin(), v.end(), '_'), v.end());
   int numFichas = v.size() / 2;
   // Verifica se a primeira metade é toda 'B' e a segunda metade é toda 'A'
   for (int i = 0; i < numFichas; ++i) {
       if (v[i] != 'B') return false;
   }
   for (int i = numFichas; i < v.size(); ++i) {
       if (v[i] != 'A') return false;
   }
    return true;
}


// Heuristicas
int Solver::heuristicaManhattan(const std::vector<char>& tabuleiro) {
    int custo = 0;
    int n = tabuleiro.size();
    
    // Contar quantas fichas B e A existem
    int numB = 0, numA = 0;
    for (char c : tabuleiro) {
        if (c == 'B') numB++;
        else if (c == 'A') numA++;
    }
    
    // Para cada ficha B, calcular distância até sua posição ideal
    // As fichas B devem ocupar as primeiras posições (0, 1, 2, ...)
    int posicaoIdealB = 0;
    for (int i = 0; i < n; ++i) {
        if (tabuleiro[i] == 'B') {
            custo += abs(i - posicaoIdealB);
            posicaoIdealB++;
        }
    }
    
    // Para cada ficha A, calcular distância até sua posição ideal
    // As fichas A devem ocupar as últimas posições (n-1, n-2, ...)
    int posicaoIdealA = n - 1;
    for (int i = n - 1; i >= 0; --i) {
        if (tabuleiro[i] == 'A') {
            custo += abs(i - posicaoIdealA);
            posicaoIdealA--;
        }
    }
    
    return custo;
}

int Solver::heuristicaFichasForaDoLugar(const std::vector<char>& tabuleiro) {
    int n = (tabuleiro.size() - 1) / 2;
    int fichasForaDoLugar = 0;
    
    for (int i = 0; i < n; ++i) {
        if (tabuleiro[i] != 'B') fichasForaDoLugar++;
    }
    for (int i = n + 1; i < (int)tabuleiro.size(); ++i) {
        if (tabuleiro[i] != 'A') fichasForaDoLugar++;
    }
    
    return fichasForaDoLugar;
}


// BFS (Busca em Largura)
SolverStats Solver::resolverBFS(const std::vector<char>& tabuleiroInicial) {
    SolverStats stats;
    auto start = std::chrono::high_resolution_clock::now();
    std::queue<Estado> fila;
    std::unordered_set<std::string> visitados;
    int nos_expandidos = 0;
    int nos_visitados = 0;
    int soma_ramificacao = 0;
    int total_nos = 0;
    
    fila.push(Estado(tabuleiroInicial, {}, 0));
    visitados.insert(tabuleiroParaString(tabuleiroInicial));
    
    while (!fila.empty()) {
        Estado atual = fila.front();
        fila.pop();
        nos_expandidos++;
        
        if (verificarVitoria(atual.tabuleiro)) {
            stats.caminho = atual.movimentos;
            stats.profundidade = atual.profundidade;
            stats.custo = (int)atual.movimentos.size();
            break;
        }
        
        std::vector<int> movimentosPossiveis = encontrarMovimentosPossiveis(atual.tabuleiro);
        soma_ramificacao += movimentosPossiveis.size();
        total_nos++;
        
        for (int movimento : movimentosPossiveis) {
            std::vector<char> novoTabuleiro = aplicarMovimento(atual.tabuleiro, movimento);
            std::string novoTabuleiroStr = tabuleiroParaString(novoTabuleiro);
            
            if (visitados.find(novoTabuleiroStr) == visitados.end()) {
                visitados.insert(novoTabuleiroStr);
                nos_visitados++;
                std::vector<int> novosMovimentos = atual.movimentos;
                novosMovimentos.push_back(movimento);
                fila.push(Estado(novoTabuleiro, novosMovimentos, atual.profundidade + 1));
            }
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    stats.nos_expandidos = nos_expandidos;
    stats.nos_visitados = nos_visitados;
    stats.fator_ramificacao = total_nos > 0 ? (double)soma_ramificacao / total_nos : 0.0;
    stats.tempo_execucao = std::chrono::duration<double>(end - start).count();
    return stats;
}

// Backtracking
SolverStats Solver::resolverBacktracking(const std::vector<char>& tabuleiroInicial, double timeout) {
    SolverStats stats;
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> caminho;
    std::set<std::string> visitados;
    int nos_expandidos = 0;
    int nos_visitados = 0;
    int soma_ramificacao = 0;
    int total_nos = 0;
    bool encontrou = false;
    const int LIMITE_PROFUNDIDADE = 10000;
    std::function<bool(const std::vector<char>&, int)> backtrack = 
        [&](const std::vector<char>& tabuleiro, int profundidade) -> bool {
            if (estourouTimeout(start, timeout)) return false;
            if (profundidade > LIMITE_PROFUNDIDADE) return false;
            nos_expandidos++;
            if (verificarVitoria(tabuleiro)) {
                stats.caminho = caminho;
                stats.profundidade = profundidade;
                stats.custo = (int)caminho.size();
                encontrou = true;
                return true;
            }
            std::string tabStr = tabuleiroParaString(tabuleiro);
            visitados.insert(tabStr);
            std::vector<int> movimentosPossiveis = encontrarMovimentosPossiveis(tabuleiro);
            soma_ramificacao += movimentosPossiveis.size();
            total_nos++;
            for (int movimento : movimentosPossiveis) {
                std::vector<char> novoTabuleiro = aplicarMovimento(tabuleiro, movimento);
                std::string novoTabuleiroStr = tabuleiroParaString(novoTabuleiro);
                if (visitados.find(novoTabuleiroStr) == visitados.end()) {
                    nos_visitados++;
                    caminho.push_back(movimento);
                    if (backtrack(novoTabuleiro, profundidade + 1)) {
                        return true;
                    }
                    caminho.pop_back();
                }
            }
            visitados.erase(tabStr);
            return false;
        };
    backtrack(tabuleiroInicial, 0);
    auto end = std::chrono::high_resolution_clock::now();
    if (!encontrou) {
        stats.caminho.clear();
        stats.profundidade = -1;
        stats.custo = -1;
    }
    stats.nos_expandidos = nos_expandidos;
    stats.nos_visitados = nos_visitados;
    stats.fator_ramificacao = total_nos > 0 ? (double)soma_ramificacao / total_nos : 0.0;
    stats.tempo_execucao = std::chrono::duration<double>(end - start).count();
    return stats;
}


// UCS (Uniform Cost Search)
SolverStats Solver::resolverOrdenada(const std::vector<char>& tabuleiroInicial) {
    SolverStats stats;
    auto start = std::chrono::high_resolution_clock::now();
    
    // Priority queue ordenada por custo g
    auto comparador = [](const Estado& a, const Estado& b) {
        return a.custo_g > b.custo_g;
    };
    std::priority_queue<Estado, std::vector<Estado>, decltype(comparador)> fila(comparador);
    std::unordered_set<std::string> visitados;
    int nos_expandidos = 0;
    int nos_visitados = 0;
    int soma_ramificacao = 0;
    int total_nos = 0;
    
    fila.push(Estado(tabuleiroInicial, {}, 0, 0, 0));
    visitados.insert(tabuleiroParaString(tabuleiroInicial));
    
    while (!fila.empty()) {
        Estado atual = fila.top();
        fila.pop();
        nos_expandidos++;
        
        if (verificarVitoria(atual.tabuleiro)) {
            stats.caminho = atual.movimentos;
            stats.profundidade = atual.profundidade;
            stats.custo = atual.custo_g;
            break;
        }
        
        std::vector<int> movimentosPossiveis = encontrarMovimentosPossiveis(atual.tabuleiro);
        soma_ramificacao += movimentosPossiveis.size();
        total_nos++;
        
        for (int movimento : movimentosPossiveis) {
            std::vector<char> novoTabuleiro = aplicarMovimento(atual.tabuleiro, movimento);
            std::string novoTabuleiroStr = tabuleiroParaString(novoTabuleiro);
            
            if (visitados.find(novoTabuleiroStr) == visitados.end()) {
                visitados.insert(novoTabuleiroStr);
                nos_visitados++;
                std::vector<int> novosMovimentos = atual.movimentos;
                novosMovimentos.push_back(movimento);
                fila.push(Estado(novoTabuleiro, novosMovimentos, atual.profundidade + 1, 
                               atual.custo_g + 1, 0));
            }
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    stats.nos_expandidos = nos_expandidos;
    stats.nos_visitados = nos_visitados;
    stats.fator_ramificacao = total_nos > 0 ? (double)soma_ramificacao / total_nos : 0.0;
    stats.tempo_execucao = std::chrono::duration<double>(end - start).count();
    return stats;
}

// Busca Gulosa
SolverStats Solver::resolverGulosa(const std::vector<char>& tabuleiroInicial, int heuristica) {
    SolverStats stats;
    auto start = std::chrono::high_resolution_clock::now();
    
    // Priority queue ordenada por heuristica h
    auto comparador = [](const Estado& a, const Estado& b) {
        return a.custo_h > b.custo_h;
    };
    std::priority_queue<Estado, std::vector<Estado>, decltype(comparador)> fila(comparador);
    std::unordered_set<std::string> visitados;
    int nos_expandidos = 0;
    int nos_visitados = 0;
    int soma_ramificacao = 0;
    int total_nos = 0;
    int h_inicial = (heuristica == 2) ? heuristicaInversoes(tabuleiroInicial) : heuristicaManhattan(tabuleiroInicial);
    fila.push(Estado(tabuleiroInicial, {}, 0, 0, h_inicial));
    visitados.insert(tabuleiroParaString(tabuleiroInicial));
    
    while (!fila.empty()) {
        Estado atual = fila.top();
        fila.pop();
        nos_expandidos++;
        
        if (verificarVitoria(atual.tabuleiro)) {
            stats.caminho = atual.movimentos;
            stats.profundidade = atual.profundidade;
            stats.custo = (int)atual.movimentos.size();
            break;
        }
        
        std::vector<int> movimentosPossiveis = encontrarMovimentosPossiveis(atual.tabuleiro);
        
        soma_ramificacao += movimentosPossiveis.size();
        total_nos++;
        
        for (int movimento : movimentosPossiveis) {
            std::vector<char> novoTabuleiro = aplicarMovimento(atual.tabuleiro, movimento);
            std::string novoTabuleiroStr = tabuleiroParaString(novoTabuleiro);
            
            if (visitados.find(novoTabuleiroStr) == visitados.end()) {
                visitados.insert(novoTabuleiroStr);
                nos_visitados++;
                std::vector<int> novosMovimentos = atual.movimentos;
                novosMovimentos.push_back(movimento);
                int h = (heuristica == 2) ? heuristicaInversoes(novoTabuleiro) : heuristicaManhattan(novoTabuleiro);
                fila.push(Estado(novoTabuleiro, novosMovimentos, atual.profundidade + 1, 0, h));
            }
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    if (stats.caminho.empty()) {
        stats.profundidade = -1;
        stats.custo = -1;
    }
    stats.nos_expandidos = nos_expandidos;
    stats.nos_visitados = nos_visitados;
    stats.fator_ramificacao = total_nos > 0 ? (double)soma_ramificacao / total_nos : 0.0;
    stats.tempo_execucao = std::chrono::duration<double>(end - start).count();
    return stats;
}

// A*
SolverStats Solver::resolverAStar(const std::vector<char>& tabuleiroInicial, int heuristica) {
    SolverStats stats;
    auto start = std::chrono::high_resolution_clock::now();

    auto comparador = [](const Estado& a, const Estado& b) {
        return (a.custo_g + a.custo_h) > (b.custo_g + b.custo_h);
    };
    std::priority_queue<Estado, std::vector<Estado>, decltype(comparador)> fila(comparador);
    std::unordered_set<std::string> visitados;
    int nos_expandidos = 0;
    int nos_visitados = 0;
    int soma_ramificacao = 0;
    int total_nos = 0;
    
    // Gerar estado final para usar na heurística
    std::vector<char> estadoFinal = gerarEstadoFinal(tabuleiroInicial);
    int h_inicial = (heuristica == 2) ? heuristicaInversoes(tabuleiroInicial) : heuristicaManhattan(tabuleiroInicial);
    fila.push(Estado(tabuleiroInicial, {}, 0, 0, h_inicial));
    visitados.insert(tabuleiroParaString(tabuleiroInicial));

    while (!fila.empty()) {
        Estado atual = fila.top();
        fila.pop();
        nos_expandidos++;

        if (verificarVitoria(atual.tabuleiro)) {
            stats.caminho = atual.movimentos;
            stats.profundidade = atual.profundidade;
            stats.custo = atual.custo_g;
            break;
        }

        std::vector<int> movimentosPossiveis = encontrarMovimentosPossiveis(atual.tabuleiro);
        
        // REMOVIDO: Ordenação dos movimentos no A* para evitar problemas de eficiência
        
        soma_ramificacao += movimentosPossiveis.size();
        total_nos++;

        for (int movimento : movimentosPossiveis) {
            std::vector<char> novoTabuleiro = aplicarMovimento(atual.tabuleiro, movimento);
            std::string novoTabuleiroStr = tabuleiroParaString(novoTabuleiro);

            if (visitados.find(novoTabuleiroStr) == visitados.end()) {
                visitados.insert(novoTabuleiroStr);
                nos_visitados++;
                std::vector<int> novosMovimentos = atual.movimentos;
                novosMovimentos.push_back(movimento);
                int h = (heuristica == 2) ? heuristicaInversoes(novoTabuleiro) : heuristicaManhattan(novoTabuleiro);
                fila.push(Estado(novoTabuleiro, novosMovimentos, atual.profundidade + 1,
                                 atual.custo_g + 1, h));
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    if (stats.caminho.empty()) {
        stats.profundidade = -1;
        stats.custo = -1;
    }
    stats.nos_expandidos = nos_expandidos;
    stats.nos_visitados = nos_visitados;
    stats.fator_ramificacao = total_nos > 0 ? (double)soma_ramificacao / total_nos : 0.0;
    stats.tempo_execucao = std::chrono::duration<double>(end - start).count();
    return stats;
}

// Funções auxiliares para IDA*
int Solver::calcularCusto(const std::vector<char>& tabuleiro) {
    // Para este problema, o custo é simplesmente o número de movimentos
    // Como não temos acesso ao caminho atual, retornamos 0
    // O custo real será calculado durante a busca
    return 0;
}

int Solver::heuristica(const std::vector<char>& tabuleiro, int tipo) {
    if (tipo == 2) {
        return heuristicaInversoes(tabuleiro);
    } else {
        return heuristicaManhattan(tabuleiro);
    }
}

int Solver::calcularNumFichas(const std::vector<char>& tabuleiro) {
    int numB = 0, numA = 0;
    for (char c : tabuleiro) {
        if (c == 'B') numB++;
        else if (c == 'A') numA++;
    }
    return numB; // ou numA, ambos são iguais
}

std::vector<char> Solver::gerarEstadoFinal(const std::vector<char>& tabuleiro) {
    std::vector<char> estadoFinal = tabuleiro;
    int numB = 0, numA = 0;
    
    // Contar fichas
    for (char c : tabuleiro) {
        if (c == 'B') numB++;
        else if (c == 'A') numA++;
    }
    
    // Gerar estado final: BBB..._...AAA
    int pos = 0;
    for (int i = 0; i < numB; ++i) {
        estadoFinal[pos++] = 'B';
    }
    estadoFinal[pos++] = '_';
    for (int i = 0; i < numA; ++i) {
        estadoFinal[pos++] = 'A';
    }
    
    return estadoFinal;
}

SolverStats Solver::resolverIDAStar(const std::vector<char>& tabuleiroInicial, int heuristica) {
    SolverStats stats;
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> caminho;
    int nos_expandidos = 0;
    int nos_visitados = 0;
    int soma_ramificacao = 0;
    int total_nos = 0;
    bool encontrou = false;
    bool timeout_ocorreu = false;

    int h_inicial = (heuristica == 2) ? heuristicaFichasForaDoLugar(tabuleiroInicial) : heuristicaManhattan(tabuleiroInicial);
    int limite = h_inicial;

    std::function<bool(const std::vector<char>&, int, int, std::set<std::string>&)> idaStar = 
        [&](const std::vector<char>& tabuleiro, int profundidade, int custo_g, std::set<std::string>& visitados) -> bool {
            // Verifica timeout
            auto now = std::chrono::high_resolution_clock::now();
            double tempo_decorrido = std::chrono::duration<double>(now - start).count();
            if (tempo_decorrido > TIMEOUT_PADRAO) {
                timeout_ocorreu = true;
                return false;
            }

            nos_expandidos++;
            
            if (verificarVitoria(tabuleiro)) {
                stats.caminho = caminho;
                stats.profundidade = profundidade;
                stats.custo = custo_g;
                encontrou = true;
                return true;
            }
            
            // Calcular heurística baseada no parâmetro
            int h;
            if (heuristica == 2) {
                h = heuristicaInversoes(tabuleiro);
            } else {
                h = heuristicaManhattan(tabuleiro);
            }
            
            int f = custo_g + h;
            if (f > limite) {
                // Atualizar o próximo limite com o menor valor de f que excedeu
                if (f < proximo_limite) {
                    proximo_limite = f;
                }
                return false;
            }
            
            std::string tabStr = tabuleiroParaString(tabuleiro);
            visitados.insert(tabStr);
            std::vector<int> movimentosPossiveis = encontrarMovimentosPossiveis(tabuleiro);
            
            // Ordenar movimentos por heurística (mais promissores primeiro)
            std::sort(movimentosPossiveis.begin(), movimentosPossiveis.end(), 
                [&](int a, int b) {
                    std::vector<char> tabA = aplicarMovimento(tabuleiro, a);
                    std::vector<char> tabB = aplicarMovimento(tabuleiro, b);
                    int hA = (heuristica == 2) ? heuristicaInversoes(tabA) : heuristicaManhattan(tabA);
                    int hB = (heuristica == 2) ? heuristicaInversoes(tabB) : heuristicaManhattan(tabB);
                    return hA < hB; // Menor heurística primeiro
                });
            
            soma_ramificacao += movimentosPossiveis.size();
            total_nos++;
            
            for (int movimento : movimentosPossiveis) {
                std::vector<char> novoTabuleiro = aplicarMovimento(tabuleiro, movimento);
                std::string novoTabuleiroStr = tabuleiroParaString(novoTabuleiro);
                
                if (visitados.find(novoTabuleiroStr) == visitados.end()) {
                    nos_visitados++;
                    caminho.push_back(movimento);
                    
                    if (idaStar(novoTabuleiro, profundidade + 1, custo_g + 1, visitados, proximo_limite)) {
                        return true;
                    }
                    
                    caminho.pop_back();

                    if (timeout_ocorreu) return false; // Interrompe busca se timeout ocorrer em chamadas recursivas
                }
            }

            visitados.erase(tabStr);
            return false;
        };

    while (true) {
        if (timeout_ocorreu) break;

        std::set<std::string> visitados;
        caminho.clear();

        if (idaStar(tabuleiroInicial, 0, 0, visitados)) {
            break;
        }

        limite++;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    if (!encontrou) {
        stats.caminho.clear();
        stats.profundidade = -1;
        stats.custo = -1;
    }

    stats.nos_expandidos = nos_expandidos;
    stats.nos_visitados = nos_visitados;
    stats.fator_ramificacao = total_nos > 0 ? (double)soma_ramificacao / total_nos : 0.0;
    stats.tempo_execucao = std::chrono::duration<double>(end - start).count();

    return stats;
}
// --- Implementação da DFS ilimitada com timeout ---
SolverStats Solver::resolverDFS(const std::vector<char>& tabuleiroInicial, double timeout) {
    SolverStats stats;
    auto start = std::chrono::high_resolution_clock::now();
    
    std::stack<Estado> pilha;
    std::set<std::string> visitados;
    int nos_expandidos = 0;
    int nos_visitados = 0;
    int soma_ramificacao = 0;
    int total_nos = 0;
    bool encontrou = false;

    pilha.push(Estado(tabuleiroInicial, {}, 0));
    
    while (!pilha.empty()) {
        if (estourouTimeout(start, timeout)) break;

        Estado atual = pilha.top();
        pilha.pop();
        std::string tabStr = tabuleiroParaString(atual.tabuleiro);

        // Verifica se já foi visitado
        if (visitados.find(tabStr) != visitados.end()) continue;
        visitados.insert(tabStr);

        nos_expandidos++;

        if (verificarVitoria(atual.tabuleiro)) {
            stats.caminho = atual.movimentos;
            stats.profundidade = atual.profundidade;
            stats.custo = (int)atual.movimentos.size();
            encontrou = true;
            break;
        }

        std::vector<int> movimentosPossiveis = encontrarMovimentosPossiveis(atual.tabuleiro);
        soma_ramificacao += movimentosPossiveis.size();
        total_nos++;

        // Importante: empilhar em ordem reversa para manter comportamento de DFS
        std::reverse(movimentosPossiveis.begin(), movimentosPossiveis.end());

        for (int movimento : movimentosPossiveis) {
            std::vector<char> novoTabuleiro = aplicarMovimento(atual.tabuleiro, movimento);
            std::string novoTabuleiroStr = tabuleiroParaString(novoTabuleiro);
            
            if (visitados.find(novoTabuleiroStr) == visitados.end()) {
                nos_visitados++;
                std::vector<int> novosMovimentos = atual.movimentos;
                novosMovimentos.push_back(movimento);
                pilha.push(Estado(novoTabuleiro, novosMovimentos, atual.profundidade + 1));
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    if (!encontrou) {
        stats.caminho.clear();
        stats.profundidade = -1;
        stats.custo = -1;
    }

    stats.nos_expandidos = nos_expandidos;
    stats.nos_visitados = nos_visitados;
    stats.fator_ramificacao = total_nos > 0 ? (double)soma_ramificacao / total_nos : 0.0;
    stats.tempo_execucao = std::chrono::duration<double>(end - start).count();
    return stats;
}

// Funcao principal que escolhe o algoritmo
SolverStats Solver::resolver(const std::vector<char>& tabuleiroInicial, int algoritmo, int heuristica) {
    switch (algoritmo) {
        case 1: return resolverBFS(tabuleiroInicial);
        case 2: return resolverBacktracking(tabuleiroInicial, TIMEOUT_PADRAO);
        case 3: return resolverDFS(tabuleiroInicial, TIMEOUT_PADRAO);
        case 4: return resolverOrdenada(tabuleiroInicial);
        case 5: return resolverGulosa(tabuleiroInicial, heuristica);
        case 6: return resolverAStar(tabuleiroInicial, heuristica);
        case 7: return resolverIDAStar(tabuleiroInicial, heuristica);
        default: return resolverBFS(tabuleiroInicial);
    }
}

void Solver::mostrarSolucao(const std::vector<char>& tabuleiroInicial, const SolverStats& stats) {
    if (stats.caminho.empty()) {
        std::cout << "Nenhuma solucao encontrada!\n";
        return;
    }
    std::vector<char> tabuleiroAtual = tabuleiroInicial;
    std::cout << "Estado inicial:\n";
    std::cout << "+";
    for (int i = 0; i < (int)tabuleiroAtual.size(); ++i) std::cout << "-+";
    std::cout << "\n|";
    for (char c : tabuleiroAtual) std::cout << c << "|";
    std::cout << "\n+";
    for (int i = 0; i < (int)tabuleiroAtual.size(); ++i) std::cout << "-+";
    std::cout << "\n ";
    for (int i = 0; i < (int)tabuleiroAtual.size(); ++i) std::cout << i << " ";
    std::cout << "\n\n";
    for (int i = 0; i < (int)stats.caminho.size(); i++) {
        int movimento = stats.caminho[i];
        tabuleiroAtual = aplicarMovimento(tabuleiroAtual, movimento);
        std::cout << "Movimento " << (i + 1) << " (posicao " << movimento << "):\n";
        std::cout << "+";
        for (int j = 0; j < (int)tabuleiroAtual.size(); ++j) std::cout << "-+";
        std::cout << "\n|";
        for (char c : tabuleiroAtual) std::cout << c << "|";
        std::cout << "\n+";
        for (int j = 0; j < (int)tabuleiroAtual.size(); ++j) std::cout << "-+";
        std::cout << "\n ";
        for (int j = 0; j < (int)tabuleiroAtual.size(); ++j) std::cout << j << " ";
        std::cout << "\n\n";
    }
    std::cout << "Solucao completa!\n";
    std::cout << "\n=== ESTATISTICAS ===\n";
    std::cout << "Numero de movimentos: " << stats.caminho.size() << "\n";
    std::cout << "Profundidade da solucao: " << stats.profundidade << "\n";
    std::cout << "Custo da solucao: " << stats.custo << "\n";
    std::cout << "Nos expandidos: " << stats.nos_expandidos << "\n";
    std::cout << "Nos visitados: " << stats.nos_visitados << "\n";
    std::cout << "Fator medio de ramificacao: " << stats.fator_ramificacao << "\n";
    std::cout << "Tempo de execucao: " << stats.tempo_execucao << " segundos\n\n";
}


