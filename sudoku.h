#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdbool.h>

#define MAX_SIZE 16

extern bool fixed[MAX_SIZE][MAX_SIZE];

int getBoardSize();
int getDifficulty();
void showInstructions();
void printGrid(int grid[MAX_SIZE][MAX_SIZE], int size);
void sudokuGenerator(int k, int grid[MAX_SIZE][MAX_SIZE], int N);
bool makeMove(int grid[MAX_SIZE][MAX_SIZE], int row, int col, int num, int N);
bool changeValue(int grid[MAX_SIZE][MAX_SIZE], int row, int col, int num, int N);
bool isSolved(int grid[MAX_SIZE][MAX_SIZE], int N);

#endif