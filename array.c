#include "array.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
    if (array == NULL) {
        return;
    }
    for (int i = 0; i < x; i++) {
        if (array[i] != NULL) {
            for (int j = 0; j < y; j++) {
                if (array[i][j] != NULL) {
                    free(array[i][j]);
                    array[i][j] = NULL; // Uniknięcie podwójnego zwolnienia
                }
            }
            free(array[i]);
            array[i] = NULL;
        }
    }
    free(array);
    array = NULL;
}


