#include "utils.h"
#include "display.h"
#include "array.h"
#include "mines.h"
#include "scores.h"
#include "loader.h"
#include "instruction.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    char *filename = NULL;
    int showInstruction = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            showInstruction = 1;
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            filename = argv[++i];
        } else {
            fprintf(stderr, "Użycie: %s [-i] [-f plik]\n", argv[0]);
            return EXIT_FAILURE;
        }
    }

    if (showInstruction) {
        instruction();
        printf("Naciśnij Enter, aby kontynuować...\n");
        while (getchar() != '\n');
    }

    if (filename != NULL) {
        loadGameFromFile(filename);
        return 0;
    }

    if (argc == 3 && strcmp(argv[1], "-f") == 0) {
        loadGameFromFile(argv[2]);
        return 0;
    }


    if (argc == 3 && strcmp(argv[1], "-f") == 0) {
        loadGameFromFile(argv[2]);
        return 0;
    }

    int x, y, minesCount, countingPoints = 0;
    int s = 0;

    while (s == 0) {
        printf(YELLOW "┌─────────────────────────────┐\n" RESET);
        printf(YELLOW "│" RESET CYAN "  Wybierz poziom trudności   " RESET YELLOW "│\n" RESET);
        printf(YELLOW "├────────────┬───────┬────────┤\n" RESET);
        printf(YELLOW "│" RESET CYAN " 1 - Łatwy  " YELLOW "│" RESET CYAN " 9x9   " YELLOW "│" RESET CYAN " 10 min " RESET YELLOW "│\n" RESET);
        printf(YELLOW "│" RESET CYAN " 2 - Średni " YELLOW "│" RESET CYAN " 16x16 " YELLOW "│" RESET CYAN " 40 min " RESET YELLOW "│\n" RESET);
        printf(YELLOW "│" RESET CYAN " 3 - Trudny " YELLOW "│" RESET CYAN " 16x30 " YELLOW "│" RESET CYAN " 99 min " RESET YELLOW "│\n" RESET);
        printf(YELLOW "│" RESET CYAN " 4 - Własny " YELLOW "│" RESET CYAN "       " YELLOW "│" RESET CYAN "        " RESET YELLOW "│\n" RESET);
        printf(YELLOW "└────────────┴───────┴────────┘\n" RESET);
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
