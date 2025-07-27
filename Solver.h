#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <queue>
#include <stack>
#include <unordered_set>
#include <string>
#include <chrono>
#include <functional>

struct SolverStats {
    std::vector<int> caminho;
    int profundidade = 0;
    int custo = 0;
    int nos_expandidos = 0;
    int nos_visitados = 0;
    double fator_ramificacao = 0.0;
    double tempo_execucao = 0.0; // em segundos
};

class Solver {
public:
    // Nenhum método público
private:
    struct Estado {
        std::vector<char> tabuleiro;
        std::vector<int> movimentos;
        int profundidade;
        int custo_g; // custo acumulado (para UCS e A*)
        int custo_h; // heuristica (para A* e Gulosa)
        
        Estado(const std::vector<char>& tab, const std::vector<int>& movs, int prof)
            : tabuleiro(tab), movimentos(movs), profundidade(prof), custo_g(0), custo_h(0) {}
        
        Estado(const std::vector<char>& tab, const std::vector<int>& movs, int prof, int g, int h)
            : tabuleiro(tab), movimentos(movs), profundidade(prof), custo_g(g), custo_h(h) {}
    };
    
    static std::string tabuleiroParaString(const std::vector<char>& tabuleiro);
    static std::vector<char> stringParaTabuleiro(const std::string& str);
    static std::vector<int> encontrarMovimentosPossiveis(const std::vector<char>& tabuleiro);
    static std::vector<char> aplicarMovimento(const std::vector<char>& tabuleiro, int posicao);
    static bool verificarVitoria(const std::vector<char>& tabuleiro);
    static int encontrarEspacoVazio(const std::vector<char>& tabuleiro);
    
    // Heuristicas
    static int heuristicaManhattan(const std::vector<char>& tabuleiro);
    static int heuristicaFichasForaDoLugar(const std::vector<char>& tabuleiro);
    
    // Algoritmos de busca
    static SolverStats resolverBFS(const std::vector<char>& tabuleiroInicial);
    static SolverStats resolverBacktracking(const std::vector<char>& tabuleiroInicial, double timeout = 10.0);
    static SolverStats resolverDFS(const std::vector<char>& tabuleiroInicial, double timeout = 10.0);
    static SolverStats resolverUCS(const std::vector<char>& tabuleiroInicial);
    static SolverStats resolverGulosa(const std::vector<char>& tabuleiroInicial, int heuristica);
    static SolverStats resolverAStar(const std::vector<char>& tabuleiroInicial, int heuristica); // alterado
    static SolverStats resolverIDAStar(const std::vector<char>& tabuleiroInicial, int heuristica); // alterado
    
    // Funcoes auxiliares
    static bool dfsLimitadoRecursivo(const std::vector<char>& tabuleiro, std::vector<int>& caminho, 
                                   std::unordered_set<std::string>& visitados, int limite, int profundidade,
                                   int& nos_expandidos, int& nos_visitados);
    static bool idaStarRecursivo(const std::vector<char>& tabuleiro, std::vector<int>& caminho,
                               std::unordered_set<std::string>& visitados, int limite, int profundidade,
                               int& nos_expandidos, int& nos_visitados, int custo_g);

public:
    static SolverStats resolver(const std::vector<char>& tabuleiroInicial, int algoritmo = 1, int heuristica = 1);
    static void mostrarSolucao(const std::vector<char>& tabuleiroInicial, const SolverStats& stats);
};

#endif