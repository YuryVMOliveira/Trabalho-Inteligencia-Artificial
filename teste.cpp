#include <iostream>
#include <vector>
#include "Solver.h"

using namespace std;

int main() {
    cout << "=== TESTE DO SOLUCIONADOR ===\n\n";
    
    // Teste com configuração inicial
    vector<char> tabuleiro = {'R', 'R', '_', 'A', 'A', 'V', 'V'};
    
    cout << "Tabuleiro inicial:\n";
    cout << "┌─┬─┬─┬─┬─┬─┬─┐\n│";
    for (char c : tabuleiro) {
        cout << c << "│";
    }
    cout << "\n└─┴─┴─┴─┴─┴─┴─┘\n";
    cout << " 0 1 2 3 4 5 6\n\n";
    
    cout << "Procurando solução...\n";
    vector<int> solucao = Solver::resolver(tabuleiro);
    
    if (!solucao.empty()) {
        cout << "Solucao encontrada com " << solucao.size() << " movimentos!\n";
        Solver::mostrarSolucao(tabuleiro, solucao);
    } else {
        cout << "Nenhuma solução encontrada!\n";
    }
    
    return 0;
} 