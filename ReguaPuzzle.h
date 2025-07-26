#ifndef REGUAPUZZLE_H
#define REGUAPUZZLE_H

#include <vector>
#include <string>

class ReguaPuzzle {
private:
    std::vector<char> tabuleiro;
    int num_fichas;
    int tamanho;
    int posicaoVazia;
    int movimentos;
    bool jogoTerminado;
    
    // Constantes para cores das fichas
    static const char FICHA_AZUL = 'A';
    static const char FICHA_VERMELHA = 'B';
    static const char ESPACO_VAZIO = '_';
    
    // Métodos privados
    void inicializarTabuleiro();
    void embaralharTabuleiro();
    bool movimentoValido(int posicao) const;
    bool podeMover(int posicao) const;
    void moverFicha(int posicao);
    bool verificarVitoria() const;
    void exibirTabuleiro() const;
    void exibirInstrucoes() const;
    char obterEntrada() const;
    void limparTela() const;
    
public:
    ReguaPuzzle(int num_fichas = 3);
    ~ReguaPuzzle();
    
    void iniciarJogo();
    void reiniciarJogo();
    int getMovimentos() const;
    bool getJogoTerminado() const;
    void setNumFichas(int n);
    int getNumFichas() const;
    std::vector<char> getTabuleiro() const;
};

#endif 