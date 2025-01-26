# Makefile for Minesweeper project

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

# Detect OS
ifeq ($(OS),Windows_NT)
    EXE = .exe
    RM = del /Q
else
    EXE =
    RM = rm -f
endif

# Source files and output
SRC = main.c loader.c display.c array.c mines.c scores.c
OBJ = $(SRC:.c=.o)
OUTPUT = minesweeper$(EXE)

# Test files
TEST_SRC = test_functions.c array.c mines.c
TEST_OBJ = $(TEST_SRC:.c=.o)
TEST_OUTPUT = tests$(EXE)

# Default rule to build the application
all: $(OUTPUT)

$(OUTPUT): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to run the program
run: $(OUTPUT)
	./$(OUTPUT)

# Rule to build and run tests
test: $(TEST_OUTPUT)
	./$(TEST_OUTPUT)

$(TEST_OUTPUT): $(TEST_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Clean up object files and executables
clean:
	$(RM) *.o $(OUTPUT) $(TEST_OUTPUT)

.PHONY: all run test clean
