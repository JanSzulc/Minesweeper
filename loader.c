#include "loader.h"
#include "array.h"
#include "display.h"
#include "mines.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void loadGameFromFile(const char* filePath) {
    FILE* file = fopen(filePath, "r");
    if (!file) {
        perror("Nie udało się otworzyć pliku");
        exit(EXIT_FAILURE);
    }

    int rows = 0, columns = 0, minesCount = 0;
    fscanf(file, "wiersze: %d\n", &rows);
    fscanf(file, "kolumny: %d\n", &columns);
    fscanf(file, "miny: %d\n", &minesCount);

    char*** display = generateArray(columns, rows, "███");
    char*** mines = generateArray(columns, rows, "   ");

    fscanf(file, "pozycje_min:\n");
    for (int i = 0; i < minesCount; i++) {
        int x, y;
        fscanf(file, "%d %d\n", &x, &y);
        strcpy(mines[x - 1][y - 1], " o ");
    }

    fscanf(file, "ruchy:\n");
    int points = 0, correctSteps = 0;
    char command;
    int col, row;
    int gameState = 0; // 0 p, 1 w, 2 t

    while (fscanf(file, " %c %d %d\n", &command, &col, &row) == 3) {
        col--;
        row--;

        if (command == 'r') {
            if (strcmp(mines[col][row], " o ") == 0) {
                gameState = 0; // Przegrana, trafiono na minę
                break;
            }
            if (strcmp(display[col][row], "███") == 0) {
                strcpy(display[col][row], mines[col][row]);
                points++;
                correctSteps++;
            }
        }
        if (command == 'f') {
            if (strcmp(display[col][row], "███") == 0) {
                correctSteps++;
            }
        }
    }

    if (gameState == 2 && isWin(display, mines, columns, rows)) {
        gameState = 1; // Wygrana
    }

    printf("Poprawne kroki: %d\n", correctSteps);
    printf("Zdobyte punkty: %d\n", points);
    printf("Stan gry: %d\n", gameState);

    freeArray(display, columns, rows);
    freeArray(mines, columns, rows);
    fclose(file);
}