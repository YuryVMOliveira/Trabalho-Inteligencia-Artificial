#include "Solver.h"
#include <iostream>
#include <vector>

void testarHeuristicas() {
    std::cout << "=== Teste das Heurísticas ===\n\n";
    
    // Teste 1: Estado inicial típico
    std::vector<char> estado1 = {'A', 'A', '_', 'B', 'B'};
    std::cout << "Estado 1: ";
    for (char c : estado1) std::cout << c << " ";
    std::cout << "\n";
    std::cout << "Manhattan: " << Solver::heuristicaManhattan(estado1) << "\n";
    std::cout << "Fichas Fora: " << Solver::heuristicaFichasForaDoLugar(estado1) << "\n";
    std::cout << "Vitoria: " << (Solver::verificarVitoria(estado1) ? "SIM" : "NAO") << "\n\n";
    
    // Teste 2: Estado objetivo
    std::vector<char> estado2 = {'B', 'B', '_', 'A', 'A'};
    std::cout << "Estado 2: ";
    for (char c : estado2) std::cout << c << " ";
    std::cout << "\n";
    std::cout << "Manhattan: " << Solver::heuristicaManhattan(estado2) << "\n";
    std::cout << "Fichas Fora: " << Solver::heuristicaFichasForaDoLugar(estado2) << "\n";
    std::cout << "Vitoria: " << (Solver::verificarVitoria(estado2) ? "SIM" : "NAO") << "\n\n";
    
    // Teste 3: Outro estado objetivo
    std::vector<char> estado3 = {'B', 'B', 'A', 'A', '_'};
    std::cout << "Estado 3: ";
    for (char c : estado3) std::cout << c << " ";
    std::cout << "\n";
    std::cout << "Manhattan: " << Solver::heuristicaManhattan(estado3) << "\n";
    std::cout << "Fichas Fora: " << Solver::heuristicaFichasForaDoLugar(estado3) << "\n";
    std::cout << "Vitoria: " << (Solver::verificarVitoria(estado3) ? "SIM" : "NAO") << "\n\n";
    
    // Teste 4: Estado intermediário
    std::vector<char> estado4 = {'A', 'B', '_', 'A', 'B'};
    std::cout << "Estado 4: ";
    for (char c : estado4) std::cout << c << " ";
    std::cout << "\n";
    std::cout << "Manhattan: " << Solver::heuristicaManhattan(estado4) << "\n";
    std::cout << "Fichas Fora: " << Solver::heuristicaFichasForaDoLugar(estado4) << "\n";
    std::cout << "Vitoria: " << (Solver::verificarVitoria(estado4) ? "SIM" : "NAO") << "\n\n";
}

int main() {
    testarHeuristicas();
    return 0;
} 