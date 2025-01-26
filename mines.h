#ifndef MINES_H
#define MINES_H

char*** generateMines(int x, int y, int minesCount, int moveX, int moveY);
void revealCell(char*** display, char*** mines, int x, int y, int col, int row, int* points, int countingPoints);
void toggleFlag(char*** display, int x, int y, int col, int row, int* flagsCount);
int isWin(char*** display, char*** mines, int x, int y);

#endif // MINES_H