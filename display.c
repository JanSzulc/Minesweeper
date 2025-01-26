#include "display.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

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
    printf("\n%*s" YELLOW "┌─────────┐\n", rowLength, "");
    for (int i = 0; i < rowLength; i++) {
        printf("─");
    }
    printf("┤  " CYAN "SAPER" YELLOW "  ├");
    for (int i = 0; i < rowLength; i++) {
        printf("─");
    }
    printf("\n%*s└─────────┘\n" RESET, rowLength, "");
    printf("%*s", maxRowDigits, "");
    for (int j = 0; j < x; j++) {
        printf(" %3d", j + 1);
    }
    printf("\n%*s", maxRowDigits + 1, "");
    printf(YELLOW"┌");
    for (int j = 0; j < x - 1; j++) {
        printf("───┬");
    }
    printf("───┐\n");

    for (int i = 0; i < y; i++) {
        printf(RESET"%*d"YELLOW" │", maxRowDigits, i + 1);
        for (int j = 0; j < x; j++) {
            printf(GREEN"%s"YELLOW"│", display[j][i]);
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
    printf("───┘\n"RESET);

    int m = minesCount - flagsCount;
    int mLength = snprintf(NULL, 0, "%d", m) + 2;
    printf("\n%*s"YELLOW"┌────────┬", rowLength - mLength, "");
    for (int i = 0; i < mLength; i++) {
        printf("─");
    }
    printf("┐\n");
    for (int i = 0; i < rowLength - mLength; i++) {
        printf("─");
    }
    printf("┤"CYAN"  MINY  "YELLOW"│"CYAN" %d "YELLOW"├", m);
    for (int i = 0; i < rowLength + mLength - 4; i++) {
        printf("─");
    }
    printf("\n%*s└────────┴", rowLength - mLength, "");
    for (int i = 0; i < mLength; i++) {
        printf("─");
    }
    printf("┘"RESET"\n");
}