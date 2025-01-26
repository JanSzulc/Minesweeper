#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#define CLEAR_SCREEN() system("cls")
#define SET_ENCODING() system("chcp 65001 > nul")
#else
#include <unistd.h>
#define CLEAR_SCREEN() system("clear")
#define SET_ENCODING() do {} while(0)	
#endif

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

void freeArray(char*** array, int x, int y) {
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            free(array[i][j]);
        }
        free(array[i]);
    }
    free(array);
}

// Funkcja do wypisywania tablicy z ramką
void printCommand(char* comm) {
    int len = 0;
    for (char* c = comm; *c != '\0'; c++) {
        len += (*c & 0xC0) != 0x80;
    }

    printf("\n┌");
    for (int i = 0; i < len + 2; i++) {
        printf("─");
    }
    printf("┐\n│ %s │\n└", comm);
    for (int i = 0; i < len + 2; i++) {
        printf("─");
    }
    printf("┘\n");
}

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
    printf("\n%*s┌────────┬", rowLength - mLength, "");
    for (int i = 0; i < mLength; i++) {
        printf("─");
    }
    printf("┐\n");
    for (int i = 0; i < rowLength - mLength; i++) {
        printf("─");
    }
    printf("┤  MINY  │ %d ├", m);
    for (int i = 0; i < rowLength + mLength - 4; i++) {
        printf("─");
    }
    printf("\n%*s└────────┴", rowLength - mLength, "");
    for (int i = 0; i < mLength; i++) {
        printf("─");
    }
    printf("┘\n");
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
//Funkcja do zapisywania wszystkich wynikow w pliku tekstowym 
void saveResult(int points) {
    char firstName[100], lastName[100];
    printf("Twój ostateczny wynik to: %d\n\n", points);
    printf("Podaj swoje imię: ");
    scanf("%99s", firstName); // Używamy %99s, aby zapobiec przepełnieniu bufora
    printf("Podaj swoje nazwisko: ");
    scanf("%99s", lastName);

    FILE *file = fopen("Wyniki.txt", "a");
    if (file == NULL) {
        perror("Nie można otworzyć pliku Wyniki.txt");
        return;
    }

    fprintf(file, "%s %s: %d\n", firstName, lastName, points);
    fclose(file);
}

//Funkcja do sortowania najlepszych wynikow oraz wypisywania rankingu
void displayTopScores() {
    FILE *file = fopen("Wyniki.txt", "r");
    if (file == NULL) {
        perror("Nie można otworzyć pliku Wyniki.txt");
        return;
    }

    struct Score {
        char firstName[100];
        char lastName[100];
        int points;
    } scores[100], temp;

    int numScores = 0;
    while (fscanf(file, "%99s %99s %d", scores[numScores].firstName, scores[numScores].lastName, &scores[numScores].points) == 3) {
        numScores++;
    }
    fclose(file);

    // Sortowanie wyników
    for (int i = 0; i < numScores - 1; i++) {
        for (int j = 0; j < numScores - i - 1; j++) {
            if (scores[j].points < scores[j + 1].points) {
                temp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = temp;
            }
        }
    }

    // Wyświetlanie najlepszych wyników
    printf("\n\nTop 5 graczy:\n");
    int count = numScores < 5 ? numScores : 5;
    for (int i = 0; i < count; i++) {
        printf("%d. %s %s %d\n", i + 1, scores[i].firstName, scores[i].lastName, scores[i].points);
    }
    printf("\n\n");
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

void loadGameFromFile(const char* filePath) {
    FILE* file = fopen(filePath, "r");
    if (!file) {
        perror("Nie udało się otworzyć pliku");
        exit(EXIT_FAILURE);
    }

    int rows = 0, columns = 0, minesCount = 0;
    fscanf(file, "rows: %d\n", &rows);
    fscanf(file, "columns: %d\n", &columns);
    fscanf(file, "mines: %d\n", &minesCount);

    char*** display = generateArray(columns, rows, "███");
    char*** mines = generateArray(columns, rows, "   ");

    fscanf(file, "mines_positions:\n");
    for (int i = 0; i < minesCount; i++) {
        int x, y;
        fscanf(file, "%d %d\n", &x, &y);
        strcpy(mines[x - 1][y - 1], " o ");
    }

    fscanf(file, "moves:\n");
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



void runGame(int x, int y, int minesCount, int countingPoints) {
    int moveX, moveY, moves = 0, flagsCount = 0;
    int started = 0, points = 0;
    char command;
    char input[100];
    char*** display = generateArray(x, y, "███");
    char*** mines = NULL;

    while (1) {
        printDisplay(display, x, y, minesCount, flagsCount);
        printf("   Punkty: %d\n", points);
        printf("   Podaj ruch: ");
        fgets(input, sizeof(input), stdin);
        CLEAR_SCREEN();

        int parseResult = sscanf(input, " %c %d %d", &command, &moveX, &moveY);
        if (parseResult != 3) {
            printCommand("Niepoprawny format komendy! Użyj: r [kolumna] [wiersz] lub f [kolumna] [wiersz].");
            continue;
        }

        moveX--;
        moveY--;

        if (command != 'r' && command != 'f') {
            printCommand("Nieznana komenda! Użyj r do odkrycia pola lub f do flagowania.");
            continue;
        }
        if (moveX < 0 || moveX >= x || moveY < 0 || moveY >= y) {
            printCommand("Współrzędne poza zakresem planszy!");
            continue;
        }

        moves++;

        if (command == 'f') {
            toggleFlag(display, x, y, moveX, moveY, &flagsCount);
        }

        if (command == 'r') {
            if (strcmp(display[moveX][moveY], " P ") == 0) {
                printCommand("Nie można odkryć zaflagowanego pola!");
                continue;
            }
            if (started == 0) {
                mines = generateMines(x, y, minesCount, moveX, moveY);
                started = 1;
            } else {
                if (strcmp(mines[moveX][moveY], " o ") == 0) {
                    printCommand("Przegrałeś! Trafiłeś na minę!");
                    printDisplay(mines, x, y, minesCount, flagsCount);
		    saveResult(points);
		    displayTopScores();
                    freeArray(display, x, y);
                    freeArray(mines, x, y);
                    return;
                }
            }

            revealCell(display, mines, x, y, moveX, moveY, &points, countingPoints);

            if (isWin(display, mines, x, y)) {
                printCommand("Gratulacje! Wygrałeś!");
                printDisplay(mines, x, y, minesCount, flagsCount);
		saveResult(points);
		displayTopScores();
                freeArray(display, x, y);
                freeArray(mines, x, y);
                return;
            }
        }
    }
}
int main(int argc, char* argv[]) {
    SET_ENCODING();
    CLEAR_SCREEN();

    if (argc == 3 && strcmp(argv[1], "-f") == 0) {
        loadGameFromFile(argv[2]);
        return 0;
    }

    int x, y, minesCount, countingPoints = 0;
    int s = 0;

    while (s == 0) {
        printf("┌─────────────────────────────┐\n");
        printf("│   Wybierz poziom trudności  │\n");
        printf("├────────────┬───────┬────────┤\n");
        printf("│ 1 - Łatwy  │ 9x9   │ 10 min │\n");
        printf("│ 2 - Średni │ 16x16 │ 40 min │\n");
        printf("│ 3 - Trudny │ 16x30 │ 99 min │\n");
        printf("│ 4 - Własny │       │        │\n");
        printf("└────────────┴───────┴────────┘\n");
        printf("Wybierz: ");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                s = 1;
                x = 9;
                y = 9;
                minesCount = 10;
                countingPoints = 1;
                break;
            case 2:
                s = 1;
                x = 16;
                y = 16;
                minesCount = 40;
                countingPoints = 2;
                break;
            case 3:
                s = 1;
                x = 30;
                y = 16;
                minesCount = 99;
                countingPoints = 3;
                break;
            case 4:
                s = 1;
                printf("Podaj wymiar x (kolumny): ");
                scanf("%d", &x);
                if (x < 5) {
                    s = 0;
                    CLEAR_SCREEN();
                    printCommand("Za mało kolumn! Minimalnie 5!");
                    break;
                }
                printf("Podaj wymiar y (wiersze): ");
                scanf("%d", &y);
                if (y < 5) {
                    s = 0;
                    CLEAR_SCREEN();
                    printCommand("Za mało wierszy! Minimalnie 5!");
                    break;
                }
                printf("Podaj liczbę min: ");
                scanf("%d", &minesCount);
                if (minesCount / (x * y) > 0.8 || (x * y) - 9 < minesCount) {
                    s = 0;
                    CLEAR_SCREEN();
                    printCommand("Za dużo min!");
                    break;
                }
                countingPoints = 1;
                break;
            default:
                CLEAR_SCREEN();
                printCommand("Niepoprawny wybór!");
                break;
        }

        while (getchar() != '\n'); // Oczyszczanie bufora wejściowego
    }

    CLEAR_SCREEN();
    runGame(x, y, minesCount, countingPoints);

    return 0;
}
