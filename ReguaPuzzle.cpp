#include "ReguaPuzzle.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <cstdlib>
#include <random>
#include <chrono> // Necessário para usar o tempo

#include <random>
#ifdef _WIN32
#include <windows.h>
#else
#include <cstring>
#endif

ReguaPuzzle::ReguaPuzzle(int num_fichas) : num_fichas(num_fichas), movimentos(0), jogoTerminado(false) {
    tamanho = 2 * num_fichas + 1;
    tabuleiro.resize(tamanho);
    inicializarTabuleiro();
}

ReguaPuzzle::~ReguaPuzzle() {
    // Destrutor vazio
}

void ReguaPuzzle::setNumFichas(int n) {
    num_fichas = n;
    tamanho = 2 * num_fichas + 1;
    tabuleiro.resize(tamanho);
    reiniciarJogo();
}

int ReguaPuzzle::getNumFichas() const {
    return num_fichas;
}

std::vector<char> ReguaPuzzle::getTabuleiro() const {
    return tabuleiro;
}



int numeroAleatorio1a2() {
    // Semeia o gerador com o tempo atual.
    // time_since_epoch().count() retorna o número de nanossegundos desde 01/01/1970.
    // É um valor excelente e altamente variável para usar como semente.
    static unsigned int seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    static std::mt19937 gen(seed);

    static std::uniform_int_distribution<> dist(1, 2);

    return dist(gen);
}

void ReguaPuzzle::inicializarTabuleiro() {
    int fichasA = num_fichas;
    int fichasB = num_fichas;

    for (int i = 0; i < tamanho; ++i) {
        if (i == num_fichas) {
            tabuleiro[i] = ESPACO_VAZIO;
            continue; // Pula para a próxima iteração do loop
        }

        bool colocarA = (numeroAleatorio1a2() == 1);

        // Tenta colocar a ficha A, se possível. Se não, tenta colocar a B.
        if (colocarA && fichasA > 0) {
            tabuleiro[i] = FICHA_AZUL;
            fichasA--;
        } else if (fichasB > 0) { // Se não pôde colocar A, tenta colocar B
            tabuleiro[i] = FICHA_VERMELHA;
            fichasB--;
        } else { // Se não pode colocar B, então obrigatoriamente tem que colocar A
            tabuleiro[i] = FICHA_AZUL;
            fichasA--;
        }
    }
    posicaoVazia = num_fichas;
}

void ReguaPuzzle::embaralharTabuleiro() {
    // Embaralha o tabuleiro fazendo movimentos aleatórios válidos
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, tamanho - 1);
    int tentativas = 0;
    do {
        inicializarTabuleiro();
        for (int i = 0; i < 50; i++) {
            int posicao = dis(gen);
            if (podeMover(posicao)) {
                moverFicha(posicao);
            }
        }
        tentativas++;
        // Garante que não está na solução e não está igual ao inicial
    } while (verificarVitoria() && tentativas < 100);
}

bool ReguaPuzzle::movimentoValido(int posicao) const {
    return posicao >= 0 && posicao < tamanho;
}

bool ReguaPuzzle::podeMover(int posicao) const {
    if (!movimentoValido(posicao) || tabuleiro[posicao] == ESPACO_VAZIO) {
        return false;
    }
    // Esquerda
    if (posicao > 0 && tabuleiro[posicao - 1] == ESPACO_VAZIO) return true;
    // Direita
    if (posicao < tamanho - 1 && tabuleiro[posicao + 1] == ESPACO_VAZIO) return true;
    // Pulo esquerda
    if (posicao > 1 && tabuleiro[posicao - 1] != ESPACO_VAZIO && tabuleiro[posicao - 2] == ESPACO_VAZIO) return true;
    // Pulo direita
    if (posicao < tamanho - 2 && tabuleiro[posicao + 1] != ESPACO_VAZIO && tabuleiro[posicao + 2] == ESPACO_VAZIO) return true;
    return false;
}

void ReguaPuzzle::moverFicha(int posicao) {
    if (!podeMover(posicao)) return;
    char ficha = tabuleiro[posicao];
    // Esquerda
    if (posicao > 0 && tabuleiro[posicao - 1] == ESPACO_VAZIO) {
        tabuleiro[posicao - 1] = ficha;
        tabuleiro[posicao] = ESPACO_VAZIO;
        posicaoVazia = posicao - 1;
        movimentos++;
        return;
    }
    // Direita
    if (posicao < tamanho - 1 && tabuleiro[posicao + 1] == ESPACO_VAZIO) {
        tabuleiro[posicao + 1] = ficha;
        tabuleiro[posicao] = ESPACO_VAZIO;
        posicaoVazia = posicao + 1;
        movimentos++;
        return;
    }
    // Pulo esquerda
    if (posicao > 1 && tabuleiro[posicao - 1] != ESPACO_VAZIO && tabuleiro[posicao - 2] == ESPACO_VAZIO) {
        tabuleiro[posicao - 2] = ficha;
        tabuleiro[posicao] = ESPACO_VAZIO;
        posicaoVazia = posicao - 2;
        movimentos++;
        return;
    }
    // Pulo direita
    if (posicao < tamanho - 2 && tabuleiro[posicao + 1] != ESPACO_VAZIO && tabuleiro[posicao + 2] == ESPACO_VAZIO) {
        tabuleiro[posicao + 2] = ficha;
        tabuleiro[posicao] = ESPACO_VAZIO;
        posicaoVazia = posicao + 2;
        movimentos++;
        return;
    }
}

bool ReguaPuzzle::verificarVitoria() const {
    bool encontrouVermelha = false;
    for (int i = 0; i < tamanho; ++i) {
        if (tabuleiro[i] == FICHA_VERMELHA) {
            encontrouVermelha = true;
        }
        if (tabuleiro[i] == FICHA_AZUL && encontrouVermelha) {
            // Encontrou uma azul depois de uma vermelha → não venceu
            return false;
        }
    }
    return true; // Nenhuma azul depois de vermelha → vitória!
}

void ReguaPuzzle::exibirTabuleiro() const {
    std::cout << "\nTabuleiro atual (Movimentos: " << movimentos << "):\n";
    std::cout << "+";
    for (int i = 0; i < tamanho; ++i) std::cout << "-+";
    std::cout << "\n|";
    for (int i = 0; i < tamanho; ++i) std::cout << tabuleiro[i] << "|";
    std::cout << "\n+";
    for (int i = 0; i < tamanho; ++i) std::cout << "-+";
    std::cout << "\n ";
    for (int i = 0; i < tamanho; ++i) std::cout << i << " ";
    std::cout << "\n ";
    for (int i = 0; i < tamanho; ++i) {
        if (podeMover(i)) {
            std::cout << "^ ";
        } else {
            std::cout << "  ";
        }
    }
    std::cout << "\n ";
    for (int i = 0; i < tamanho; ++i) {
        if (podeMover(i)) {
            std::cout << i << " ";
        } else {
            std::cout << "  ";
        }
    }
    std::cout << "\n\n";
}

void ReguaPuzzle::exibirInstrucoes() const {
    std::cout << "Instrucoes:\n";
    std::cout << "- Digite o numero da posicao da ficha que deseja mover\n";
    std::cout << "- Use 'R' para reiniciar o jogo\n";
    std::cout << "- Use 'Q' para sair\n";
    std::cout << "- Objetivo: Organizar as fichas por cor (";
    for (int i = 0; i < num_fichas; ++i) std::cout << FICHA_VERMELHA << " ";
    std::cout << ESPACO_VAZIO << " ";
    for (int i = 0; i < num_fichas; ++i) std::cout << FICHA_AZUL << " ";
    std::cout << ")\n\n";
}

char ReguaPuzzle::obterEntrada() const {
    std::string entrada;
    std::cout << "Digite sua escolha: ";
    std::cin >> entrada;
    if (entrada.length() == 1) return entrada[0];
    return '\0';
}

void ReguaPuzzle::limparTela() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void ReguaPuzzle::iniciarJogo() {
    reiniciarJogo();
    while (!jogoTerminado) {
        limparTela();
        exibirTabuleiro();
        exibirInstrucoes();
        char entrada = obterEntrada();
        if (entrada == 'Q' || entrada == 'q') {
            std::cout << "Jogo interrompido!\n";
            break;
        } else if (entrada == 'R' || entrada == 'r') {
            reiniciarJogo();
            continue;
        } else if (entrada >= '0' && entrada <= '9') {
            int posicao = entrada - '0';
            if (podeMover(posicao)) {
                moverFicha(posicao);
                if (verificarVitoria()) {
                    limparTela();
                    exibirTabuleiro();
                    std::cout << "\n🎉 PARABÉNS! Você venceu em " << movimentos << " movimentos! 🎉\n";
                    jogoTerminado = true;
                }
            } else {
                std::cout << "Movimento invalido! Pressione Enter para continuar...\n";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
            }
        } else {
            std::cout << "Entrada invalida! Pressione Enter para continuar...\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        }
    }
    tabuleiroDefinidoManualmente = false; // Permite novo jogo normal depois
    if (jogoTerminado) {
        std::cout << "\nPressione Enter para voltar ao menu...\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
}

void ReguaPuzzle::reiniciarJogo() {
    movimentos = 0;
    jogoTerminado = false;
    if (!tabuleiroDefinidoManualmente) {
        inicializarTabuleiro();
    }
    // Se foi definido manualmente, só zera movimentos e status
}

int ReguaPuzzle::getMovimentos() const {
    return movimentos;
}

bool ReguaPuzzle::getJogoTerminado() const {
    return jogoTerminado;
}

void ReguaPuzzle::setTabuleiro(const std::vector<char>& novoTabuleiro) {
    tabuleiro = novoTabuleiro;
    tamanho = tabuleiro.size();
    movimentos = 0;
    jogoTerminado = false;
    tabuleiroDefinidoManualmente = true; // <-- Adicione esta linha
    for (int i = 0; i < tamanho; ++i) {
        if (tabuleiro[i] == '_') {
            posicaoVazia = i;
            break;
        }
    }
}
