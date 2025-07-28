# Instruções Detalhadas - Jogo das Fichas

## Compilação e Execução

### 1. Compilar o projeto
```bash
make
```

### 2. Executar o jogo principal
```bash
make run
```

## Como Jogar

### Menu Principal
O jogo oferece 4 opções:
1. **Jogar** - Inicia uma nova partida
2. **Ver regras** - Mostra as regras do jogo
3. **Solucionador automático** - Demonstra como o computador resolve o puzzle
4. **Sair** - Encerra o programa

### Durante o Jogo

#### Controles:
- **0-6**: Seleciona a posição da ficha que deseja mover
- **R**: Reinicia o jogo com nova configuração
- **Q**: Sai do jogo atual

#### Objetivo:
Organizar as fichas para que fiquem agrupadas por cor:
```
V V _ R R A A
```
Onde:
- V = Ficha Verde
- R = Ficha Vermelha  
- A = Ficha Azul
- _ = Espaço vazio

#### Regras de Movimento:
1. **Movimento simples**: Uma ficha pode se mover para um espaço vazio adjacente
2. **Pulo**: Uma ficha pode pular sobre outra ficha para chegar a um espaço vazio

#### Exemplo de Movimento:
```
Estado inicial: R R _ A A V V
Movimento: Selecionar posição 3 (ficha A)
Resultado: R R A _ A V V
```

## Solucionador Automático

O solucionador usa um algoritmo de **Busca em Largura (BFS)** para encontrar a solução ótima:

### Como funciona:
1. Analisa todas as configurações possíveis do tabuleiro
2. Explora os movimentos em ordem de profundidade
3. Encontra o caminho mais curto para a solução
4. Mostra cada passo da solução

### Exemplo de saída:
```
=== SOLUÇÃO ENCONTRADA ===
Número de movimentos: 14

Estado inicial:
┌─┬─┬─┬─┬─┬─┬─┐
│R│R│_│A│A│V│V│
└─┴─┴─┴─┴─┴─┴─┘

Movimento 1 (posição 3):
┌─┬─┬─┬─┬─┬─┬─┐
│R│R│A│_│A│V│V│
└─┴─┴─┴─┴─┴─┴─┘
...
```

## Estratégias para Resolver

### 1. Análise do Estado Atual
- Identifique onde estão as fichas de cada cor
- Localize o espaço vazio
- Visualize os movimentos possíveis

### 2. Planejamento
- Decida qual grupo de fichas mover primeiro
- Calcule quantos movimentos são necessários
- Identifique possíveis bloqueios

### 3. Execução
- Faça os movimentos em sequência lógica
- Evite criar configurações que bloqueiem futuros movimentos
- Use pulos estratégicos para economizar movimentos

### 4. Verificação
- Após cada movimento, verifique se está mais próximo do objetivo
- Se necessário, desfaça movimentos que não ajudaram

## Dicas para Jogadores

1. **Comece pelos extremos**: Mova as fichas das extremidades primeiro
2. **Use pulos estrategicamente**: Pulos podem economizar movimentos
3. **Mantenha o espaço vazio central**: Facilita a organização
4. **Pense à frente**: Visualize 2-3 movimentos à frente
5. **Não tenha medo de desfazer**: Às vezes é necessário voltar atrás

## Solução Ótima

A solução ótima para o puzzle clássico (R R _ A A V V → V V _ R R A A) requer **14 movimentos**.

### Sequência de movimentos:
1. Mover A da posição 3 para 2
2. Mover V da posição 5 para 3
3. Mover V da posição 6 para 5
4. Mover R da posição 1 para 6
5. Mover A da posição 4 para 1
6. Mover R da posição 0 para 4
7. Mover V da posição 3 para 0
8. Mover V da posição 5 para 3
9. Mover R da posição 6 para 5
10. Mover R da posição 4 para 6
11. Mover A da posição 1 para 4
12. Mover A da posição 2 para 1
13. Mover V da posição 0 para 2
14. Mover V da posição 3 para 0

Resultado final: V V _ R R A A ✅

## Troubleshooting

### Problemas de Compilação:
- Certifique-se de ter um compilador C++11 instalado
- No Windows, use `make windows` para compilação específica
- Verifique se todos os arquivos estão no mesmo diretório

### Problemas de Execução:
- Se o programa não iniciar, verifique se o executável foi criado
- No Windows, execute `regua_puzzle.exe`
- No Linux/Mac, execute `./regua_puzzle`

### Problemas de Interface:
- Se os caracteres especiais não aparecerem corretamente, o terminal pode não suportar UTF-8
- Use um terminal moderno (Windows Terminal, iTerm2, etc.) 