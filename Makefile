# Makefile dla projektu Minesweeper

# Kompilator i flagi
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

# Wykrycie SO
ifeq ($(OS),Windows_NT)
    EXE = .exe
    RM = del /Q
else
    EXE =
    RM = rm -f
endif

# Pliki zrodlowe i output
SRC = main.c instruction.c loader.c display.c array.c mines.c scores.c
OBJ = $(SRC:.c=.o)
OUTPUT = minesweeper$(EXE)

# Pliki testowe
TEST_SRC = test_functions.c array.c mines.c
TEST_OBJ = $(TEST_SRC:.c=.o)
TEST_OUTPUT = tests$(EXE)

# Domyslna regula do budowania programu
all: $(OUTPUT)

$(OUTPUT): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Budowa pliku wykonywalnego
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Uruchomienie programu
run: $(OUTPUT)
	./$(OUTPUT)

# Budowa i uruchomienie plikow tekstowych
test: $(TEST_OUTPUT)
	./$(TEST_OUTPUT)

$(TEST_OUTPUT): $(TEST_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Usuwanie wszystkich plikow obiektowych
clean:
	$(RM) *.o $(OUTPUT) $(TEST_OUTPUT)

.PHONY: all run test clean
