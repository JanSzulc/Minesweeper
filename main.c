#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Funkcja do generowania tablicy (ogólna)
char*** generateArray(int x, int y, const char* initialValue) {
    char*** array = (char***)malloc(x * sizeof(char**));
    if (array == NULL) {
        perror("Nie udało się przydzielić pamięci");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < x; i++) {
        array[i] = (char**)malloc(y * sizeof(char*));
        if (array[i] == NULL) {
            perror("Nie udało się przydzielić pamięci");
            exit(EXIT_FAILURE);
        }

        for (int j = 0; j < y; j++) {
            array[i][j] = (char*)malloc(4 * sizeof(char)); // "███" + \0
            if (array[i][j] == NULL) {
                perror("Nie udało się przydzielić pamięci");
                exit(EXIT_FAILURE);
            }
            strcpy(array[i][j], initialValue);
        }
    }

    return array;
}

// Funkcja do wypisywania tablicy z ramką
void printDisplay(char*** display, int x, int y, int minesCount, int flagsCount) {
    int maxRowDigits = snprintf(NULL, 0, "%d", y) + 1;
    int rowLength = (maxRowDigits + (x * 4) - 5) / 2;
    printf("\n%*s┌─────────┐\n", rowLength, "");
    for (int i = 0; i < rowLength; i++) {
        printf("─");
    }
    printf("┤  SAPER  ├");
    for (int i = 0; i < rowLength; i++) {
        printf("─");
    }
    printf("\n%*s└─────────┘\n", rowLength, "");
    printf("%*s", maxRowDigits, "");
    for (int j = 0; j < x; j++) {
        printf(" %3d", j + 1);
    }
    printf("\n%*s", maxRowDigits + 1, "");
    printf("┌");
    for (int j = 0; j < x - 1; j++) {
        printf("───┬");
    }
    printf("───┐\n");

    for (int i = 0; i < y; i++) {
        printf("%*d │", maxRowDigits, i + 1);
        for (int j = 0; j < x; j++) {
            printf("%s│", display[j][i]);
        }
        printf("\n");

        if (i < y - 1) {
            printf("%*s", maxRowDigits + 1, "");
            printf("├");
            for (int j = 0; j < x - 1; j++) {
                printf("───┼");
            }
            printf("───┤\n");
        }
    }

    printf("%*s", maxRowDigits + 1, "");
    printf("└");
    for (int j = 0; j < x - 1; j++) {
        printf("───┴");
    }
    printf("───┘\n");


    int m = minesCount - flagsCount;
    int mLength = snprintf(NULL, 0, "%d", m) + 2;
    printf("\n%*s┌────────┬", rowLength-mLength, "");
    for (int i = 0; i < mLength; i++) {
        printf("─");
    }
    printf("┐\n");
    for (int i = 0; i < rowLength-mLength; i++) {
        printf("─");
    }
    printf("┤  MINY  │ %d ├", m);
    for (int i = 0; i < rowLength+mLength-4; i++) {
        printf("─");
    }
    printf("\n%*s└────────┴", rowLength-mLength, "");
    for (int i = 0; i < mLength; i++) {
        printf("─");
    }
    printf("┘\n");
}

// Funkcja do generowania min
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

// Funkcja odkrywania komórek
void revealCell(char*** display, char*** mines, int x, int y, int col, int row) {
    if (col < 0 || col >= x || row < 0 || row >= y || strcmp(display[col][row], "███") != 0) {
        return;
    }

    strcpy(display[col][row], mines[col][row]);

    if (strcmp(mines[col][row], "   ") == 0) {
        for (int di = -1; di <= 1; di++) {
            for (int dj = -1; dj <= 1; dj++) {
                if (di != 0 || dj != 0) {
                    revealCell(display, mines, x, y, col + di, row + dj);
                }
            }
        }
    }
}

// Funkcja do obsługi flag
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

// Sprawdzenie poprawności komendy
int isValidCommand(char command, int x, int y, int col, int row) {
    if (command != 'r' && command != 'f') {
        printf("Nieznana komenda! Użyj 'r' do odkrycia pola lub 'f' do flagowania.\n");
        return 0;
    }
    if (col < 0 || col >= x || row < 0 || row >= y) {
        printf("Współrzędne poza zakresem planszy!\n");
        return 0;
    }
    return 1;
}

// Sprawdzenie poprawności formatu komendy
int parseCommand(char* input, char* command, int* col, int* row) {
    if (sscanf(input, " %c %d %d", command, col, row) != 3) {
        printf("Niepoprawny format komendy! Użyj: r [kolumna] [wiersz] lub f [kolumna] [wiersz].\n");
        return 0;
    }
    return 1;
}

// Sprawdzenie warunku wygranej
int isWin(char*** display, char*** mines, int x, int y) {
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            if (strcmp(display[i][j], "███") == 0 && strcmp(mines[i][j], " o ") != 0) {
                return 0;
            }
        }
    }
    return 1;
}

// Funkcja inicjalizująca grę
void runGame(int x, int y, int minesCount) {
    int moveX, moveY, moves = 0, flagsCount = 0;
    char command;
    char input[100];

    char*** display = generateArray(x, y, "███");
    char*** mines = NULL;

    while (1) {
        printDisplay(display, x, y, minesCount, flagsCount);

        printf("   Podaj ruch: ");
        fgets(input, sizeof(input), stdin);

        if (!parseCommand(input, &command, &moveX, &moveY)) {
            continue;
        }

        moveX--;
        moveY--;

        if (!isValidCommand(command, x, y, moveX, moveY)) {
            continue;
        }

        if (moves == 0) {
            mines = generateMines(x, y, minesCount, moveX, moveY);
        }

        if (command == 'r') {
            if (strcmp(display[moveX][moveY], " P ") == 0) {
                printf("Nie można odkryć zaflagowanego pola!\n");
                continue;
            }

            if (strcmp(mines[moveX][moveY], " o ") == 0) {
                printf("Przegrałeś! Trafiłeś na minę!\n");
                printDisplay(mines, x, y, minesCount, flagsCount);
                return;
            }

            revealCell(display, mines, x, y, moveX, moveY);
        } else if (command == 'f') {
            toggleFlag(display, x, y, moveX, moveY, &flagsCount);
        }

        moves++;

        if (isWin(display, mines, x, y)) {
            printf("Gratulacje! Wygrałeś!\n");
            printDisplay(mines, x, y, minesCount, flagsCount);
            return;
        }
    }
}

int main() {
    int x, y, minesCount;

    printf("Podaj wymiar x (kolumny): ");
    scanf("%d", &x);
    printf("Podaj wymiar y (wiersze): ");
    scanf("%d", &y);
    printf("Podaj liczbę min: ");
    scanf("%d", &minesCount);
    while (getchar() != '\n'); // Oczyszczanie bufora wejściowego

    runGame(x, y, minesCount);

    return 0;
}
