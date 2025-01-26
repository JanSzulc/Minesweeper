#include "scores.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void saveResult(int points) {
    char firstName[100], lastName[100];
    printf("Twój ostateczny wynik to: %d\n\n", points);
    printf("Podaj swoje imię: ");
    scanf("%99s", firstName);
    printf("Podaj swoje nazwisko: ");
    scanf("%99s", lastName);

    FILE *file = fopen("Wyniki.cfg", "a");
    if (file == NULL) {
        perror("Nie można otworzyć pliku Wyniki.cfg");
        return;
    }

    fprintf(file, "%s %s: %d\n", firstName, lastName, points);
    fclose(file);
}

void displayTopScores() {
    FILE *file = fopen("Wyniki.cfg", "r");
    if (file == NULL) {
        perror("Nie można otworzyć pliku Wyniki.cfg");
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
