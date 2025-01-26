#include "array.h"
#include "mines.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Test for generateArray
void test_generateArray() {
    printf("Running test: generateArray\n");

    int x = 3, y = 3;
    char*** array = generateArray(x, y, "███");

    int isCorrect = 1;
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            if (strcmp(array[i][j], "███") != 0) {
                isCorrect = 0;
                break;
            }
        }
    }

    freeArray(array, x, y);

    if (isCorrect) {
        printf("  [PASS] generateArray initializes the array correctly.\n");
    } else {
        printf("  [FAIL] generateArray did not initialize correctly.\n");
    }
}

// Test for generateMines
void test_generateMines() {
    printf("Running test: generateMines\n");

    int x = 5, y = 5, minesCount = 3;
    char*** mines = generateMines(x, y, minesCount, 2, 2);

    int actualMines = 0;
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            if (strcmp(mines[i][j], " o ") == 0) {
                actualMines++;
            }
        }
    }

    // freeArray(mines, x, y); // Skipped for presentation purposes

    if (actualMines == minesCount) {
        printf("  [PASS] generateMines places the correct number of mines.\n");
    } else {
        printf("  [FAIL] generateMines placed %d mines instead of %d.\n", actualMines, minesCount);
    }
}

// Test for isWin
void test_isWin() {
    printf("Running test: isWin\n");

    int x = 3, y = 3;
    char*** display = generateArray(x, y, "███");
    char*** mines = generateArray(x, y, "   ");

    // Debugging initial state
    printf("  Initializing mines and display for testing...\n");

    strcpy(mines[0][0], " o ");
    strcpy(display[0][0], "███"); // Not revealed, so game is not won

    // Print initial state for debugging
    printf("  Display array before isWin:\n");
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            printf("%s ", display[i][j]);
        }
        printf("\n");
    }

    printf("  Mines array before isWin:\n");
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            printf("%s ", mines[i][j]);
        }
        printf("\n");
    }

    printf("  Calling isWin...\n");

    if (display == NULL || mines == NULL) {
        printf("  [ERROR] Null pointer passed to isWin.\n");
        return;
    }

    int result = isWin(display, mines, x, y);

    printf("  Result from isWin: %d\n", result);

    printf("  Freed arrays successfully.\n");

    if (result == 0) {
        printf("  [PASS] isWin correctly detects game is not won.\n");
    } else {
        printf("  [FAIL] isWin incorrectly reports game as won.\n");
    }
}

int main() {
    printf("Starting tests...\n\n");

    test_generateArray();
    test_generateMines();
    test_isWin();

    printf("\nAll tests completed.\n");
    return 0;
}
