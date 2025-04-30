#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include "sudoku.h"

#define MAX_SIZE 16

bool fixed[MAX_SIZE][MAX_SIZE]; // Tablica oznaczajaca pola, ktorych nie mozna zmieniac (podpowiedzi)

// Pobiera rozmiar planszy od uzytkownika
int getBoardSize() {
    int size;
    printf("Wybierz rozmiar planszy (4, 9 lub 16): ");
    while (scanf_s("%d", &size) == 1) {
        if (size == 4 || size == 9 || size == 16)
            return size;
        printf("Niepoprawny wybor. Wpisz 4, 9 lub 16: ");
    }
    exit(1);
}

// Pobiera poziom trudnosci od uzytkownika
int getDifficulty() {
    int d;
    printf("Wybierz poziom trudnosci (1 - latwy, 2 - sredni, 3 - trudny): ");
    while (scanf_s("%d", &d) == 1) {
        if (d >= 1 && d <= 3)
            return d;
        printf("Niepoprawny wybor. Wpisz 1, 2 lub 3: ");
    }
    exit(1);
}

// Sprawdza, czy liczba jest unikalna w danym boksie
bool unUsedInBox(int grid[MAX_SIZE][MAX_SIZE], int rowStart, int colStart, int num, int N) {
    int boxSize = (int)sqrt(N);
    for (int i = 0; i < boxSize; i++)
        for (int j = 0; j < boxSize; j++)
            if (grid[rowStart + i][colStart + j] == num)
                return false;
    return true;
}

// Wypelnia jeden boks losowymi unikalnymi liczbami
void fillBox(int grid[MAX_SIZE][MAX_SIZE], int row, int col, int N) {
    int num, boxSize = (int)sqrt(N);
    for (int i = 0; i < boxSize; i++) {
        for (int j = 0; j < boxSize; j++) {
            do {
                num = (rand() % N) + 1;
            } while (!unUsedInBox(grid, row, col, num, N));
            grid[row + i][col + j] = num;
        }
    }
}

// Sprawdza, czy liczba nie wystepuje w danym wierszu
bool unUsedInRow(int grid[MAX_SIZE][MAX_SIZE], int row, int num, int N) {
    for (int j = 0; j < N; j++)
        if (grid[row][j] == num)
            return false;
    return true;
}

// Sprawdza, czy liczba nie wystepuje w danej kolumnie
bool unUsedInCol(int grid[MAX_SIZE][MAX_SIZE], int col, int num, int N) {
    for (int i = 0; i < N; i++)
        if (grid[i][col] == num)
            return false;
    return true;
}

// Sprawdza, czy wstawienie liczby w danym miejscu jest bezpieczne
bool checkIfSafe(int grid[MAX_SIZE][MAX_SIZE], int i, int j, int num, int N) {
    return unUsedInRow(grid, i, num, N)
        && unUsedInCol(grid, j, num, N)
        && unUsedInBox(grid, i - i % (int)sqrt(N), j - j % (int)sqrt(N), num, N);
}

// Wypelnia przekatne boksy
void fillDiagonal(int grid[MAX_SIZE][MAX_SIZE], int N) {
    int boxSize = (int)sqrt(N);
    for (int i = 0; i < N; i += boxSize)
        fillBox(grid, i, i, N);
}

// Rekurencyjnie wypelnia pozostale pola
bool fillRemaining(int grid[MAX_SIZE][MAX_SIZE], int i, int j, int N) {
    if (i == N) return true;
    if (j == N) return fillRemaining(grid, i + 1, 0, N);
    if (grid[i][j] != 0) return fillRemaining(grid, i, j + 1, N);

    for (int num = 1; num <= N; num++) {
        if (checkIfSafe(grid, i, j, num, N)) {
            grid[i][j] = num;
            if (fillRemaining(grid, i, j + 1, N)) return true;
            grid[i][j] = 0;
        }
    }
    return false;
}

// Usuwa k losowych cyfr z planszy (tworzy "dziury")
void removeKDigits(int grid[MAX_SIZE][MAX_SIZE], int k, int N) {
    int total = N * N, limit;
    if (N == 4)       limit = total / 2;
    else if (N == 9)  limit = total * 60 / 100;
    else              limit = total * 70 / 100;
    if (k > limit) k = limit;

    while (k > 0) {
        int id = rand() % total;
        int r = id / N, c = id % N;
        if (grid[r][c] != 0) {
            grid[r][c] = 0;
            k--;
        }
    }
}

// Generuje pelna plansze Sudoku i usuwa z niej k liczb
void sudokuGenerator(int k, int grid[MAX_SIZE][MAX_SIZE], int N) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            grid[i][j] = 0;

    fillDiagonal(grid, N);
    fillRemaining(grid, 0, 0, N);
    removeKDigits(grid, k, N);
}

// Wyswietla plansze w konsoli
void printGrid(int grid[MAX_SIZE][MAX_SIZE], int size) {
    int boxSize;
    if (size == 4) boxSize = 2;
    else if (size == 9) boxSize = 3;
    else if (size == 16) boxSize = 4;
    else boxSize = 3;

    printf("    ");
    for (int j = 0; j < size; j++) {
        printf("%2d ", j + 1);
        if ((j + 1) % boxSize == 0 && j != size - 1)
            printf(" ");
    }
    printf("\n");

    printf("   +");
    for (int j = 0; j < size; j++) {
        printf("---");
        if ((j + 1) % boxSize == 0 && j != size - 1)
            printf("+");
    }
    printf("+\n");

    for (int i = 0; i < size; i++) {
        printf("%2d |", i + 1);
        for (int j = 0; j < size; j++) {
            if (grid[i][j] == 0)
                printf(" . ");
            else
                printf("%2d ", grid[i][j]);
            if ((j + 1) % boxSize == 0 && j != size - 1)
                printf("|");
        }
        printf("|\n");

        if ((i + 1) % boxSize == 0 && i != size - 1) {
            printf("   +");
            for (int j = 0; j < size; j++) {
                printf("---");
                if ((j + 1) % boxSize == 0 && j != size - 1)
                    printf("+");
            }
            printf("+\n");
        }
    }

    printf("   +");
    for (int j = 0; j < size; j++) {
        printf("---");
        if ((j + 1) % boxSize == 0 && j != size - 1)
            printf("+");
    }
    printf("+\n");
}

// Obsluguje wstawienie nowej liczby przez gracza
bool makeMove(int grid[MAX_SIZE][MAX_SIZE], int row, int col, int num, int N) {
    if (row < 0 || row >= N || col < 0 || col >= N) {
        printf("Niepoprawna pozycja!\n");
        return false;
    }
    if (num < 1 || num > N) {
        printf("Niepoprawna liczba! Podaj wartosc od 1 do %d.\n", N);
        return false;
    }
    if (fixed[row][col]) {
        printf("Nie mozna zmieniac podpowiedzi!\n");
        return false;
    }
    if (grid[row][col] != 0) {
        printf("Pole nie jest puste!\n");
        return false;
    }
    if (checkIfSafe(grid, row, col, num, N)) {
        grid[row][col] = num;
        return true;
    }
    printf("Niepoprawny ruch!\n");
    return false;
}

// Umozliwia graczowi zmiane istniejacej liczby
bool changeValue(int grid[MAX_SIZE][MAX_SIZE], int row, int col, int num, int N) {
    if (row < 0 || row >= N || col < 0 || col >= N) {
        printf("Niepoprawna pozycja!\n");
        return false;
    }
    if (num < 1 || num > N) {
        printf("Niepoprawna liczba! Podaj wartosc od 1 do %d.\n", N);
        return false;
    }
    if (fixed[row][col]) {
        printf("Nie mozna zmieniac podpowiedzi!\n");
        return false;
    }
    if (grid[row][col] == 0) {
        printf("Pole jest puste!\n");
        return false;
    }
    if (checkIfSafe(grid, row, col, num, N)) {
        grid[row][col] = num;
        return true;
    }
    printf("Niepoprawna zmiana!\n");
    return false;
}

// Sprawdza, czy plansza zostala calkowicie uzupelniona
bool isSolved(int grid[MAX_SIZE][MAX_SIZE], int N) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (grid[i][j] == 0)
                return false;
    return true;
}

// Wyswietla instrukcje gry
void showInstructions() {
    printf("\nInstrukcja gry Sudoku:\n");
    printf("1. Wybierz rozmiar planszy (4, 9 lub 16).\n");
    printf("2. Wybierz poziom trudnosci: Latwy, Sredni, Trudny.\n");
    printf("3. Wybierz opcje Nowa Gra w menu glownym.\n");
    printf("4. Uzupelniaj puste pola cyframi 1..N.\n");
    printf("5. Nie zmieniaj liczb oznaczonych jako podpowiedzi.\n");
    printf("6. Gra konczy sie, gdy wszystkie pola beda wypelnione poprawnie.\n\n");
}
