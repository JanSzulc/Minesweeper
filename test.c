#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Funkcja do generowania tablicy
char*** generateArray(int x, int y) {
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
            strcpy(array[i][j], "███");
        }
    }

    return array;
}

// Funkcja do wypisywania tablicy z ramką
void printArray(char*** array, int x, int y) {
    // Górna krawędź
    printf("    ");
    for (int j = 0; j < y; j++) {
        printf("%d   ", j + 1);
    }
    printf("\n");

    printf("  ┌");
    for (int j = 0; j < y - 1; j++) {
        printf("───┬");
    }
    printf("───┐\n");

    for (int i = 0; i < x; i++) {
        printf("%d │", i + 1);
        for (int j = 0; j < y; j++) {
            printf("%s│", array[i][j]);
        }
        printf("\n");

        if (i < x - 1) {
            printf("  ├");
            for (int j = 0; j < y - 1; j++) {
                printf("───┼");
            }
            printf("───┤\n");
        }
    }

    printf("  └");
    for (int j = 0; j < y - 1; j++) {
        printf("───┴");
    }
    printf("───┘\n");
}

// Funkcja do zwalniania pamięci
void freeArray(char*** array, int x, int y) {
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            free(array[i][j]);
        }
        free(array[i]);
    }
    free(array);
}

int main() {
    int x, y;

    // Pobranie wymiarów od użytkownika
    printf("Podaj wymiar x: ");
    scanf("%d", &x);
    printf("Podaj wymiar y: ");
    scanf("%d", &y);

    // Generowanie tablicy
    char*** array = generateArray(x, y);
    array[4][3] = " 2 ";
    printArray(array, x, y);

    // Zwolnienie pamięci
    freeArray(array, x, y);

    return 0;
}
