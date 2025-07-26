#include "Solver.h"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <stack>
#include <set>
#include <thread>

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
    for (int i = 0; i < tabuleiro.size(); i++) {
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
    std::swap(novoTabuleiro[posicao], novoTabuleiro[espacoVazio]);
    return novoTabuleiro;
}

bool Solver::verificarVitoria(const std::vector<char>& tabuleiro) {
    int n = (tabuleiro.size() - 1) / 2;
    for (int i = 0; i < n; ++i) {
        if (tabuleiro[i] != 'B') return false;
    }
    if (tabuleiro[n] != '_') return false;
    for (int i = n + 1; i < (int)tabuleiro.size(); ++i) {
        if (tabuleiro[i] != 'A') return false;
    }
    return true;
}

// Heuristicas
int Solver::heuristicaManhattan(const std::vector<char>& tabuleiro) {
    int n = (tabuleiro.size() - 1) / 2;
    int custo = 0;
    
    for (int i = 0; i < tabuleiro.size(); ++i) {
        if (tabuleiro[i] == 'B') {
            // Ficha B deve estar na primeira metade
            if (i >= n) {
                custo += i - (n - 1); // distancia ate a posicao correta
            }
        } else if (tabuleiro[i] == 'A') {
            // Ficha A deve estar na segunda metade
            if (i < n + 1) {
                custo += (n + 1) - i; // distancia ate a posicao correta
            }
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
    if (tabuleiro[n] != '_') fichasForaDoLugar++;
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
SolverStats Solver::resolverGulosa(const std::vector<char>& tabuleiroInicial) {
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
    int h_inicial = heuristicaManhattan(tabuleiroInicial); // Troca para Manhattan
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
                int h = heuristicaManhattan(novoTabuleiro); // Troca para Manhattan
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
SolverStats Solver::resolverAStar(const std::vector<char>& tabuleiroInicial) {
    SolverStats stats;
    auto start = std::chrono::high_resolution_clock::now();
    
    // Priority queue ordenada por f = g + h
    auto comparador = [](const Estado& a, const Estado& b) {
        return (a.custo_g + a.custo_h) > (b.custo_g + b.custo_h);
    };
    std::priority_queue<Estado, std::vector<Estado>, decltype(comparador)> fila(comparador);
    std::unordered_set<std::string> visitados;
    int nos_expandidos = 0;
    int nos_visitados = 0;
    int soma_ramificacao = 0;
    int total_nos = 0;
    int h_inicial = heuristicaManhattan(tabuleiroInicial); // Troca para Manhattan
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
                int h = heuristicaManhattan(novoTabuleiro); // Troca para Manhattan
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

// IDA* (Iterative Deepening A*)
SolverStats Solver::resolverIDAStar(const std::vector<char>& tabuleiroInicial) {
    SolverStats stats;
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> caminho;
    int nos_expandidos = 0;
    int nos_visitados = 0;
    int soma_ramificacao = 0;
    int total_nos = 0;
    bool encontrou = false;
    int h_inicial = heuristicaManhattan(tabuleiroInicial); // Troca para Manhattan
    int limite = h_inicial;
    std::function<bool(const std::vector<char>&, int, int, std::set<std::string>&)> idaStar = 
        [&](const std::vector<char>& tabuleiro, int profundidade, int custo_g, std::set<std::string>& visitados) -> bool {
            nos_expandidos++;
            
            if (verificarVitoria(tabuleiro)) {
                stats.caminho = caminho;
                stats.profundidade = profundidade;
                stats.custo = custo_g;
                encontrou = true;
                return true;
            }
            
            int f = custo_g + heuristicaManhattan(tabuleiro);
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
            visitados.erase(tabStr); // Remove ao voltar
            return false;
        };
    
    // Remover limite superior do while
    while (true) {
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
SolverStats Solver::resolver(const std::vector<char>& tabuleiroInicial, int algoritmo) {
    switch (algoritmo) {
        case 1: return resolverBFS(tabuleiroInicial);
        case 2: return resolverBacktracking(tabuleiroInicial, TIMEOUT_PADRAO);
        case 3: return resolverDFS(tabuleiroInicial, TIMEOUT_PADRAO);
        case 4: return resolverUCS(tabuleiroInicial);
        case 5: return resolverGulosa(tabuleiroInicial);
        case 6: return resolverAStar(tabuleiroInicial);
        case 7: return resolverIDAStar(tabuleiroInicial);
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