# Makefile para o Jogo das Fichas (Régua Puzzle)

# Compilador
CXX = g++

# Flags de compilação
CXXFLAGS = -std=c++11 -Wall -Wextra -O2

# Nome do executável
TARGET = regua_puzzle
TEST_TARGET = test_heuristicas

# Arquivos fonte
SOURCES = main.cpp ReguaPuzzle.cpp Solver.cpp
TEST_SOURCES = test_heuristicas.cpp Solver.cpp

# Arquivos objeto
OBJECTS = $(SOURCES:.cpp=.o)
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o)

# Detectar sistema operacional
ifeq ($(OS),Windows_NT)
    # Windows
    RM = del /Q
    RMDIR = rmdir /S /Q
    EXE_EXT = .exe
else
    # Unix/Linux/Mac
    RM = rm -f
    RMDIR = rm -rf
    EXE_EXT = 
endif

# Regra padrão
all: $(TARGET)

# Compilar o executável
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)$(EXE_EXT)

# Compilar o teste
$(TEST_TARGET): $(TEST_OBJECTS)
	$(CXX) $(TEST_OBJECTS) -o $(TEST_TARGET)$(EXE_EXT)

# Compilar arquivos objeto
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpar arquivos gerados
clean:
ifeq ($(OS),Windows_NT)
	$(RM) $(OBJECTS) $(TEST_OBJECTS) $(TARGET)$(EXE_EXT) $(TEST_TARGET)$(EXE_EXT) 2>nul || exit 0
else
	$(RM) $(OBJECTS) $(TEST_OBJECTS) $(TARGET)$(EXE_EXT) $(TEST_TARGET)$(EXE_EXT)
endif

# Executar o programa
run: $(TARGET)
ifeq ($(OS),Windows_NT)
	./$(TARGET)$(EXE_EXT)
else
	./$(TARGET)
endif

# Executar o teste
test: $(TEST_TARGET)
ifeq ($(OS),Windows_NT)
	./$(TEST_TARGET)$(EXE_EXT)
else
	./$(TEST_TARGET)
endif

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
	@echo "  make test   - Compilar e executar teste das heurísticas"
	@echo "  make clean  - Limpar arquivos gerados"
	@echo "  make help   - Mostrar esta ajuda"

.PHONY: all clean run test windows unix help 