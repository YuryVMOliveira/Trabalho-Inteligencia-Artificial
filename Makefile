# Makefile para o Jogo das Fichas (Régua Puzzle)

# Compilador
CXX = g++

# Flags de compilação
CXXFLAGS = -std=c++11 -Wall -Wextra -O2

# Nome do executável
TARGET = regua_puzzle

# Arquivos fonte
SOURCES = main.cpp ReguaPuzzle.cpp Solver.cpp

# Arquivos objeto
OBJECTS = $(SOURCES:.cpp=.o)

# Regra padrão
all: $(TARGET)

# Compilar o executável
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Compilar arquivos objeto
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpar arquivos gerados
clean:
	rm -f $(OBJECTS) $(TARGET)

# Executar o programa
run: $(TARGET)
	./$(TARGET)

# Regra para Windows
windows: CXXFLAGS += -D_WIN32
windows: $(TARGET)

# Regra para Linux/Mac
unix: $(TARGET)

# Ajuda
help:
	@echo "Comandos disponíveis:"
	@echo "  make        - Compilar o programa"
	@echo "  make run    - Compilar e executar"
	@echo "  make clean  - Limpar arquivos gerados"
	@echo "  make help   - Mostrar esta ajuda"

.PHONY: all clean run windows unix help 