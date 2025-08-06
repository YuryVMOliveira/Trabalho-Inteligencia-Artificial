# Link para apresentação:
[Apresentação no canva](https://www.canva.com/design/DAGvO-rXBN0/NFuBJT7J03K2p3VHHZ2L_w/view?utm_content=DAGvO-rXBN0&utm_campaign=designshare&utm_medium=link2&utm_source=uniquelinks&utlId=hba2b6ea953)

# Jogo das Fichas (Régua Puzzle)

Este é um projeto em C++ que implementa o clássico jogo das fichas, também conhecido como "Régua Puzzle" ou "Puzzle das Fichas".

## Descrição do Jogo

O jogo consiste em uma régua com 7 posições, onde há:
- 2 fichas vermelhas (R)
- 2 fichas azuis (A) 
- 2 fichas verdes (V)
- 1 espaço vazio (_)

### Objetivo
Organizar as fichas para que fiquem agrupadas por cor, com o espaço vazio no meio:
```
V V _ R R A A
```

### Regras
- Você pode mover uma ficha para um espaço vazio adjacente
- Você pode pular sobre uma ficha para chegar a um espaço vazio
- O objetivo é organizar todas as fichas da mesma cor juntas

## Como Compilar

### Usando Makefile (Recomendado)
```bash
# Compilar o programa
make

# Compilar e executar
make run

# Limpar arquivos gerados
make clean
```

### Compilação Manual
```bash
g++ -std=c++11 -Wall -Wextra -O2 main.cpp ReguaPuzzle.cpp -o regua_puzzle
```

## Como Jogar

1. Execute o programa: `./regua_puzzle`
2. Escolha a opção "1. Jogar" no menu principal
3. O tabuleiro será exibido com as posições numeradas de 0 a 6
4. Digite o número da posição da ficha que deseja mover
5. Use 'R' para reiniciar o jogo
6. Use 'Q' para sair

### Controles
- **0-6**: Selecionar posição da ficha para mover
- **R**: Reiniciar o jogo
- **Q**: Sair do jogo

## Estrutura do Projeto

```
├── main.cpp           # Arquivo principal com menu
├── ReguaPuzzle.h      # Cabeçalho da classe do jogo
├── ReguaPuzzle.cpp    # Implementação da classe do jogo
├── Solver.h           # Cabeçalho do solucionador automático
├── Solver.cpp         # Implementação do solucionador
├── teste.cpp          # Programa de teste do solucionador
├── Makefile           # Script de compilação
└── README.md          # Este arquivo
```

## Funcionalidades

- ✅ Interface de menu interativa
- ✅ Tabuleiro visual com bordas
- ✅ Validação de movimentos
- ✅ Contador de movimentos
- ✅ Verificação de vitória
- ✅ Embaralhamento automático
- ✅ Reinicialização do jogo
- ✅ Solucionador automático (algoritmo de busca em largura)
- ✅ Compatibilidade Windows/Linux

## Requisitos

- Compilador C++ com suporte a C++11 (g++, clang++, etc.)
- Sistema operacional: Windows, Linux ou macOS

## Exemplo de Jogo

```
┌─┬─┬─┬─┬─┬─┬─┐
│R│A│_│V│R│A│V│
└─┴─┴─┴─┴─┴─┴─┘
 0 1 2 3 4 5 6

Movimentos: 0
```

## Desenvolvimento

Este projeto foi desenvolvido como trabalho prático de programação em C++, demonstrando:
- Programação orientada a objetos
- Manipulação de vetores
- Interface de usuário em console
- Lógica de jogos
- Validação de entrada
- Estruturas de controle
- Algoritmos de busca (BFS)
- Resolução automática de problemas

## Autor

Desenvolvido como trabalho prático de programação. 