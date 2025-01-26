#include "mines.h"
#include "array.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

char*** generateMines(int x, int y, int minesCount, int moveX, int moveY) {
    char*** mines = generateArray(x, y, "   ");
    srand(time(NULL));
    int minesPlaced = 0;

    while (minesPlaced < minesCount) {
        int col = rand() % x;
        int row = rand() % y;

        int isAroundFirstMove = 0;
        for (int di = -1; di <= 1; di++) {
            for (int dj = -1; dj <= 1; dj++) {
                if (col == moveX + di && row == moveY + dj) {
                    isAroundFirstMove = 1;
                    break;
                }
            }
            if (isAroundFirstMove) break;
        }

        if (isAroundFirstMove || strcmp(mines[col][row], " o ") == 0) {
            continue;
        }

        strcpy(mines[col][row], " o ");
        minesPlaced++;
    }

    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            if (strcmp(mines[i][j], " o ") != 0) {
                int count = 0;
                for (int di = -1; di <= 1; di++) {
                    for (int dj = -1; dj <= 1; dj++) {
                        if (di == 0 && dj == 0) continue;
                        int ni = i + di;
                        int nj = j + dj;
                        if (ni >= 0 && ni < x && nj >= 0 && nj < y && strcmp(mines[ni][nj], " o ") == 0) {
                            count++;
                        }
                    }
                }
                if (count > 0) {
                    sprintf(mines[i][j], " %d ", count);
                }
            }
        }
    }

    return mines;
}

void revealCell(char*** display, char*** mines, int x, int y, int col, int row, int* points, int countingPoints) {
    if (col < 0 || col >= x || row < 0 || row >= y || strcmp(display[col][row], "███") != 0) {
        return;
    }

    strcpy(display[col][row], mines[col][row]);
    *points += countingPoints;

    if (strcmp(mines[col][row], "   ") == 0) {
        for (int di = -1; di <= 1; di++) {
            for (int dj = -1; dj <= 1; dj++) {
                if (di != 0 || dj != 0) {
                    revealCell(display, mines, x, y, col + di, row + dj, points, countingPoints);
                }
            }
        }
    }
}

void toggleFlag(char*** display, int x, int y, int col, int row, int* flagsCount) {
    if (col < 0 || col >= x || row < 0 || row >= y) {
        return;
    }

    if (strcmp(display[col][row], "███") == 0) {
        strcpy(display[col][row], " P ");
        (*flagsCount)++;
    } else if (strcmp(display[col][row], " P ") == 0) {
        strcpy(display[col][row], "███");
        (*flagsCount)--;
    }
}

int isWin(char*** display, char*** mines, int x, int y) {
    if (display == NULL || mines == NULL || x <= 0 || y <= 0) {
        return 0;
    }

    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            if (strcmp(display[i][j], "███") == 0 && strcmp(mines[i][j], " o ") != 0) {
                return 0; // Gra jeszcze się nie skończyła
            }
        }
    }
    return 1; // Wszystkie pola są odkryte
}

