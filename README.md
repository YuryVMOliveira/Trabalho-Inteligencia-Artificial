# Jogo das Fichas (Régua Puzzle)

Este é um projeto em C++ que implementa o clássico jogo das fichas, também conhecido como "Régua Puzzle".

## Descrição do Jogo

O jogo consiste em uma régua com posições, onde há:
- N fichas vermelhas (B)
- N fichas azuis (A)
- 1 espaço vazio (_)

### Objetivo
Organizar as fichas para que fiquem separadas por cor, por exemplo:
```
B B B ... _ A A A ...
```

### Regras
- Você pode mover uma ficha para um espaço vazio adjacente.
- Você pode pular sobre uma ficha para chegar a um espaço vazio.
- O objetivo é organizar todas as fichas da mesma cor juntas, conforme o objetivo acima.

## Como Compilar

### Usando Makefile (Recomendado)
```bash
make           # Compilar o programa
make run       # Compilar e executar
make clean     # Limpar arquivos gerados
```

### Compilação Manual
```bash
g++ -std=c++11 -Wall -Wextra -O2 main.cpp ReguaPuzzle.cpp Solver.cpp -o regua_puzzle
```

## Como Jogar

1. Execute o programa: `./regua_puzzle` (Linux/Mac) ou `regua_puzzle.exe` (Windows)
2. Escolha a opção desejada no menu principal:
    - **1. Jogar**: Inicia uma nova partida interativa
    - **2. Ver regras**: Mostra as regras do jogo
    - **3. Solucionador automático**: O computador resolve o puzzle usando algoritmos de busca
    - **4. Sair**: Encerra o programa
    - **5. Comparar algoritmos**: Compara o desempenho dos algoritmos de busca

### Durante o Jogo

- Digite o número da posição da ficha que deseja mover.
- Use 'R' para reiniciar o jogo.
- Use 'Q' para sair do jogo atual.

### Controles
- **0-N**: Selecionar posição da ficha para mover
- **R**: Reiniciar o jogo
- **Q**: Sair do jogo

## Estrutura do Projeto

```
├── main.cpp           # Arquivo principal com menu
├── ReguaPuzzle.h      # Cabeçalho da classe do jogo
├── ReguaPuzzle.cpp    # Implementação da classe do jogo
├── Solver.h           # Cabeçalho do solucionador automático
├── Solver.cpp         # Implementação do solucionador
├── Makefile           # Script de compilação
└── README.md          # Este arquivo
```

## Funcionalidades

- Interface de menu interativa
- Tabuleiro visual com bordas
- Validação de movimentos
- Contador de movimentos
- Verificação de vitória
- Embaralhamento automático
- Reinicialização do jogo
- Solucionador automático (vários algoritmos de busca)
- Comparação de desempenho entre algoritmos
- Compatibilidade Windows/Linux

## Requisitos

- Compilador C++ com suporte a C++11 (g++, clang++, etc.)
- Sistema operacional: Windows, Linux ou macOS

## Exemplo de Jogo

```
+---+---+---+---+---+---+---+
|B  |B  |_  |A  |A  |   |   |
+---+---+---+---+---+---+---+
 0   1   2   3   4   5   6
```
Movimentos: 0

## Solucionador Automático

O solucionador pode usar diferentes algoritmos de busca:
- Busca em Largura (BFS)
- Backtracking
- Busca em Profundidade (DFS)
- Busca Ordenada (UCS)
- Busca Gulosa
- Busca A*
- Busca IDA*

Para algoritmos heurísticos, é possível escolher entre:
- Heurística de Manhattan
- Heurística de Fichas Fora do Lugar

O programa mostra estatísticas como número de movimentos, profundidade, custo, nós expandidos, visitados, fator de ramificação e tempo de execução.

## Desenvolvimento

Este projeto foi desenvolvido como trabalho prático de programação em C++, demonstrando:
- Programação orientada a objetos
- Manipulação de vetores
- Interface de usuário em console
- Lógica de jogos
- Validação de entrada
- Estruturas de controle
- Algoritmos de busca (BFS, DFS, A*, etc.)
- Resolução automática de problemas

## Autor

Desenvolvido como trabalho prático de programação.