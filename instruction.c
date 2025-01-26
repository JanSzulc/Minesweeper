#include "instruction.h"
#include <stdio.h>

void instruction() {

    printf("\n\n");
    printf("┌────────────────────────────────────────────────┐\n");
    printf("│                     INSTRUKCJA                 │\n");
    printf("└────────────────────────────────────────────────┘\n");
    printf("Witaj w grze SAPER! Oto jak należy grać:\n");
    printf("\n");
    printf("OPCJE URUCHOMIENIA GRY:\n");
    printf("  1. Wybierz poziom trudności (łatwy, średni, trudny) z menu głównego.\n");
    printf("  2. Możesz także podać własne wymiary planszy i liczbę min.\n");
    printf("  3. Alternatywnie uruchom program z flagą -f i podaj ścieżkę do pliku z zapisanym stanem gry.\n");
    printf("\n");
    printf("STEROWANIE PODCZAS GRY:\n");
    printf("  r <kolumna> <wiersz> - Odkrywa pole na planszy.\n");
    printf("  f <kolumna> <wiersz> - Oznacza pole flagą (lub usuwa flagę, jeśli już jest ustawiona).\n");
    printf("\n");
    printf("ZASADY GRY:\n");
    printf("  - Wygrywasz, gdy oznaczysz wszystkie miny flagami i odkryjesz pozostałe pola.\n");
    printf("  - Przegrywasz, gdy odkryjesz pole z miną.\n");
    printf("\n");
    printf("URUCHAMIANIE Z PLIKU:\n");
    printf("  Jeśli uruchamiasz grę z pliku, musi on zawierać:\n");
    printf("    1. Liczbę wierszy, kolumn i min w pierwszych liniach.\n");
    printf("    2. Pozycje min w formacie: 'mines_positions:' oraz współrzędne min.\n");
    printf("    3. Sekcję 'moves:' zawierającą ruchy w formacie: 'r <kolumna> <wiersz>' lub 'f <kolumna> <wiersz>'.\n");
    printf("\n");
    printf("PRZYKŁAD PLIKU:\n");
    printf("  wiersze: 9\n");
    printf("  kolumny: 9\n");
    printf("  miny: 10\n");
    printf("  pozycje_min:\n");
    printf("  2 3\n");
    printf("  5 5\n");
    printf("  ruchy:\n");
    printf("  r 1 1\n");
    printf("  f 2 3\n");
    printf("──────────────────────────────────────────────────\n");
    printf("\n\n");
}

