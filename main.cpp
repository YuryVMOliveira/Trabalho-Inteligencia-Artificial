#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <cstdlib>
#include <ctime>
#include "ReguaPuzzle.h"
#include "Solver.h"
#include <iomanip> // Para setw e fixed

using namespace std;

void mostrarMenu() {
    cout << "\n=== JOGO DAS FICHAS (REGUA PUZZLE) ===\n";
    cout << "1. Jogar\n";
    cout << "2. Ver regras\n";
    cout << "3. Solucionador automatico\n";
    cout << "4. Sair\n";
    cout << "5. Comparar algoritmos\n"; // Nova opção
    cout << "Escolha uma opcao: ";
}

void mostrarRegras() {
    cout << "\n=== REGRAS DO JOGO ===\n";
    cout << "O jogo das fichas consiste em uma regua com espacos vazios e fichas de duas cores.\n";
    cout << "O objetivo e mover as fichas para que fiquem organizadas por cor.\n";
    cout << "\nRegras:\n";
    cout << "- Voce pode mover uma ficha para um espaco vazio adjacente\n";
    cout << "- Voce pode pular sobre uma ficha para chegar a um espaco vazio\n";
    cout << "- O objetivo e organizar todas as fichas da mesma cor juntas\n";
    cout << "- Use numeros de 0 ate o tamanho do tabuleiro para selecionar fichas, 'R' para reiniciar, 'Q' para sair\n";
    cout << "\nRepresentacao:\n";
    cout << "A = Ficha Azul\n";
    cout << "B = Ficha Vermelha\n";
    cout << "_ = Espaco vazio\n";
    cout << "\nObjetivo: BBB..._AAA...\n";
}

int pedirNumFichas() {
    int n;
    cout << "\nDigite o numero de fichas de cada cor (minimo 2): ";
    while (true) {
        cin >> n;
        if (cin.fail() || n < 2) {
            cout << "Valor invalido. Digite um numero maior ou igual a 2: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            break;
        }
    }
    return n;
}

void mostrarMenuAlgoritmos() {
    cout << "\nEscolha o algoritmo de busca:\n";
    cout << "1. Busca em Largura\n";
    cout << "2. Backtracking\n";
    cout << "3. Busca em Profundidade (Limitada)\n";
    cout << "4. Busca Ordenada\n";
    cout << "5. Busca Gulosa\n";
    cout << "6. Busca A*\n";
    cout << "7. Busca IDA*\n";
    cout << "Digite a opcao: ";
}

void mostrarMenuHeuristicas() {
    cout << "\nEscolha a heuristica:\n";
    cout << "1. Manhattan\n";
    cout << "2. Fichas Fora do Lugar\n";
    cout << "Digite a opcao: ";
}

string obterNomeAlgoritmo(int algoritmo) {
    switch (algoritmo) {
        case 1: return "Busca em Largura";
        case 2: return "Backtracking";
        case 3: return "Busca em Profundidade (Limitada)";
        case 4: return "Busca Ordenada";
        case 5: return "Busca Gulosa";
        case 6: return "Busca A*";
        case 7: return "Busca IDA*";
        default: return "Desconhecido";
    }
}

void mostrarTabelaComparacao(const vector<SolverStats>& statsList, const vector<string>& nomes) {
    cout << "\n+-------------------------------+---------------------+------------------------+-------------------+----------------+--------------+---------------------+" << endl;
    cout << "| Algoritmo                    | Tamanho do Caminho  | Profundidade da Solucao| Custo da Solucao  | Nos Expandidos | Nos Visitados| Tempo de Execucao (s)|" << endl;
    cout << "+-------------------------------+---------------------+------------------------+-------------------+----------------+--------------+---------------------+" << endl;
    for (size_t i = 0; i < statsList.size(); ++i) {
        const SolverStats& s = statsList[i];
        cout << "| " << left << setw(30) << nomes[i].substr(0, 30);
        if (s.profundidade == -1 || s.custo == -1) {
            cout << "| " << right << setw(19) << "N/A"
                 << " | " << setw(22) << "N/A"
                 << " | " << setw(17) << "N/A"
                 << " | " << setw(14) << s.nos_expandidos
                 << " | " << setw(12) << s.nos_visitados
                 << " | " << setw(19) << fixed << setprecision(4) << s.tempo_execucao << " |" << endl;
        } else {
            cout << "| " << right << setw(19) << s.caminho.size()
                 << " | " << setw(22) << s.profundidade
                 << " | " << setw(17) << s.custo
                 << " | " << setw(14) << s.nos_expandidos
                 << " | " << setw(12) << s.nos_visitados
                 << " | " << setw(19) << fixed << setprecision(4) << s.tempo_execucao << " |" << endl;
        }
    }
    cout << "+-------------------------------+---------------------+------------------------+-------------------+----------------+--------------+---------------------+\n";
}

bool desejaTabuleiroAleatorio() {
    char resp;
    cout << "\nDeseja tabuleiro aleatorio? (s/n): ";
    cin >> resp;
    return (resp == 's' || resp == 'S');
}

int main() {
    srand(time(0));
    int num_fichas = 3;
    ReguaPuzzle jogo(num_fichas);
    int opcao;
    
    do {
        mostrarMenu();
        cin >> opcao;
        
        switch(opcao) {
            case 1:
                num_fichas = pedirNumFichas();
                jogo.setNumFichas(num_fichas);
                jogo.iniciarJogo();
                break;
            case 2:
                mostrarRegras();
                break;
            case 3: {
                cout << "\n=== SOLUCIONADOR AUTOMATICO ===\n";
                num_fichas = pedirNumFichas();
                ReguaPuzzle jogo(num_fichas);
                vector<char> tabuleiro = jogo.getTabuleiro();
                cout << "Tabuleiro inicial:\n";
                cout << "+";
                for (int i = 0; i < (int)tabuleiro.size(); ++i) cout << "-+";
                cout << "\n|";
                for (char c : tabuleiro) cout << c << "|";
                cout << "\n+";
                for (int i = 0; i < (int)tabuleiro.size(); ++i) cout << "-+";
                cout << "\n ";
                for (int i = 0; i < (int)tabuleiro.size(); ++i) cout << i << " ";
                cout << "\n\n";
                
                mostrarMenuAlgoritmos();
                int alg;
                cin >> alg;
                
                if (alg >= 1 && alg <= 7) {
                    string nomeAlg = obterNomeAlgoritmo(alg);
                    cout << "\nProcurando solucao (" << nomeAlg << ")...\n";
                    SolverStats stats = Solver::resolver(tabuleiro, alg);
                    Solver::mostrarSolucao(tabuleiro, stats);
                } else {
                    cout << "Opcao invalida!\n";
                }
                cout << "\nPressione Enter para continuar...\n";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
                break;
            }
            case 4:
                cout << "Obrigado por jogar!\n";
                break;
            case 5: { // Nova opção de comparação
                cout << "\n=== COMPARAR ALGORITMOS ===\n";
                num_fichas = pedirNumFichas();
                vector<char> tabuleiro;
                for (int i = 0; i < num_fichas; ++i) tabuleiro.push_back('A');
                tabuleiro.push_back('_');
                for (int i = 0; i < num_fichas; ++i) tabuleiro.push_back('B');
                vector<SolverStats> statsList;
                vector<string> nomes;
                for (int alg = 1; alg <= 7; ++alg) {
                    nomes.push_back(obterNomeAlgoritmo(alg));
                    statsList.push_back(Solver::resolver(tabuleiro, alg));
                }
                mostrarTabelaComparacao(statsList, nomes);
                cout << "\nPressione Enter para continuar...\n";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
                break;
            }
            default:
                cout << "Opcao invalida!\n";
        }
    } while(opcao != 4);
    
    return 0;
}