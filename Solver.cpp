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

bool Solver::verificarVitoria(const std::vector<char>& tabuleiro) {
    bool encontrouVermelha = false;
    for (char c : tabuleiro) {
        if (c == 'B') {
            encontrouVermelha = true;
        }
        if (c == 'A' && encontrouVermelha) {
            // Encontrou uma azul depois de uma vermelha → não venceu
            return false;
        }
    }
    return true; // Nenhuma azul depois de vermelha → vitória!
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

// Nova heurística mais eficiente para IDA*
int Solver::heuristicaInversoes(const std::vector<char>& tabuleiro) {
    int inversoes = 0;
    int n = tabuleiro.size();
    
    // Contar inversões: quantas vezes uma ficha A aparece antes de uma ficha B
    for (int i = 0; i < n; ++i) {
        if (tabuleiro[i] == 'A') {
            for (int j = i + 1; j < n; ++j) {
                if (tabuleiro[j] == 'B') {
                    inversoes++;
                }
            }
        }
    }
    
    // Cada inversão requer pelo menos um movimento para ser corrigida
    return inversoes;
}

int Solver::heuristicaFichasForaDoLugar(const std::vector<char>& tabuleiro) {
    int fichasForaDoLugar = 0;
    int n = tabuleiro.size();
    
    // Contar quantas fichas B e A existem
    int numB = 0, numA = 0;
    for (char c : tabuleiro) {
        if (c == 'B') numB++;
        else if (c == 'A') numA++;
    }
    
    // Verificar se as fichas estão na ordem correta: BBB..._...AAA
    // As primeiras numB posições devem ter fichas B
    for (int i = 0; i < numB; ++i) {
        if (tabuleiro[i] != 'B') fichasForaDoLugar++;
    }
    
    // A posição numB deve estar vazia (espaço entre B e A)
    if (tabuleiro[numB] != '_') fichasForaDoLugar++;
    
    // As últimas numA posições devem ter fichas A
    for (int i = numB + 1; i < n; ++i) {
        if (tabuleiro[i] != 'A') fichasForaDoLugar++;
    }
    
    // Manter o valor original para garantir admissibilidade
    return fichasForaDoLugar;
}
// Nova heurística mais eficiente: combinação de Manhattan e Fichas Fora do Lugar
int Solver::heuristicaCombinada(const std::vector<char>& tabuleiro) {
    int manhattan = heuristicaManhattan(tabuleiro);
    int fichasFora = heuristicaFichasForaDoLugar(tabuleiro);
    return (manhattan + fichasFora) / 2; // Média das duas heurísticas
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

// DFS Limitado removido (não faz sentido com DFS ilimitada)

// UCS (Uniform Cost Search)
SolverStats Solver::resolverUCS(const std::vector<char>& tabuleiroInicial) {
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
    int h_inicial = (heuristica == 2) ? heuristicaFichasForaDoLugar(tabuleiroInicial) : heuristicaManhattan(tabuleiroInicial);
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
                int h = (heuristica == 2) ? heuristicaFichasForaDoLugar(novoTabuleiro) : heuristicaManhattan(novoTabuleiro);
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
    int h_inicial = (heuristica == 2) ? heuristicaFichasForaDoLugar(tabuleiroInicial) : heuristicaManhattan(tabuleiroInicial);
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
                int h = (heuristica == 2) ? heuristicaFichasForaDoLugar(novoTabuleiro) : heuristicaManhattan(novoTabuleiro);
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

SolverStats Solver::resolverIDAStar(const std::vector<char>& tabuleiroInicial, int heuristica) {
    SolverStats stats;
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> caminho;
    int nos_expandidos = 0;
    int nos_visitados = 0;
    int soma_ramificacao = 0;
    int total_nos = 0;
    bool encontrou = false;
    
    // Escolher heurística baseada no parâmetro
    int h_inicial;
    if (heuristica == 2) {
        h_inicial = heuristicaInversoes(tabuleiroInicial); // Usar heurística de inversões
    } else {
        h_inicial = heuristicaManhattan(tabuleiroInicial);
    }
    
    // Usar limite inicial mais conservador
    int limite = h_inicial;
    const int LIMITE_MAXIMO = 100; // Aumentar limite para garantir otimalidade
    const double TIMEOUT = 15.0; // Aumentar timeout
    
    std::function<bool(const std::vector<char>&, int, int, std::unordered_set<std::string>&)> idaStar = 
        [&](const std::vector<char>& tabuleiro, int profundidade, int custo_g, std::unordered_set<std::string>& visitados) -> bool {
            // Verificar timeout
            if (estourouTimeout(start, TIMEOUT)) return false;
            
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
                h = heuristicaInversoes(tabuleiro); // Usar heurística de inversões
            } else {
                h = heuristicaManhattan(tabuleiro);
            }
            
            int f = custo_g + h;
            if (f > limite) return false;
            
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
                    
                    if (idaStar(novoTabuleiro, profundidade + 1, custo_g + 1, visitados)) {
                        return true;
                    }
                    
                    caminho.pop_back();
                }
            }
            
            return false;
        };
    
    // Loop principal do IDA*
    while (limite <= LIMITE_MAXIMO) {
        std::unordered_set<std::string> visitados;
        caminho.clear();
        
        if (idaStar(tabuleiroInicial, 0, 0, visitados)) {
            break; // Solução encontrada
        }
        
        // Verificar timeout antes de incrementar o limite
        if (estourouTimeout(start, TIMEOUT)) {
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
    std::vector<int> caminho;
    std::set<std::string> visitados;
    int nos_expandidos = 0;
    int nos_visitados = 0;
    int soma_ramificacao = 0;
    int total_nos = 0;
    bool encontrou = false;
    const int LIMITE_PROFUNDIDADE = 10000;
    std::function<bool(const std::vector<char>&, int)> dfs =
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
                    if (dfs(novoTabuleiro, profundidade + 1)) {
                        return true;
                    }
                    caminho.pop_back();
                }
            }
            visitados.erase(tabStr);
            return false;
        };
    dfs(tabuleiroInicial, 0);
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
        case 4: return resolverUCS(tabuleiroInicial);
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


