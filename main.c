#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sudoku.h"

// Glowna funkcja programu (menu, rozgrywka)
int main() {
    srand(time(NULL));
    int grid[MAX_SIZE][MAX_SIZE];
    int choice;

    while (true) {
        printf("\n=== MENU GLOWNE ===\n");
        printf("1) Nowa gra\n");
        printf("2) Wybierz rozmiar planszy\n");
        printf("3) Wybierz poziom trudnosci\n");
        printf("4) Instrukcja\n");
        printf("5) Wyjscie\n");
        printf("Wybierz opcje: ");
        scanf_s("%d", &choice);

        static int boardSize = 9;
        static int difficulty = 2;

        if (choice == 1) {
            int N = boardSize;
            int total = N * N;
            int k;

            switch (difficulty) {
            case 1: k = total / 4; break;
            case 2: k = total / 2; break;
            case 3: k = total * 3 / 4; break;
            default: k = total / 2;
            }

            sudokuGenerator(k, grid, N);

            for (int i = 0; i < N; i++)
                for (int j = 0; j < N; j++)
                    fixed[i][j] = (grid[i][j] != 0);

            while (true) {
                printf("\n--- %dx%d Sudoku (%s) ---\n", N, N,
                    difficulty == 1 ? "LATWY" : difficulty == 2 ? "SREDNI" : "TRUDNY");
                printGrid(grid, N);

                if (isSolved(grid, N)) {
                    printf("Gratulacje! Rozwiazales sudoku!\n");
                    break;
                }

                printf("\nOpcje:\n 1) Wstaw ruch\n 2) Zmien wartosc\n 3) Wyjdz\n Wybierz: ");
                int c;
                scanf_s("%d", &c);
                int row, col, num;

                if (c == 1) {
                    printf("Podaj wiersz, kolumne, liczbe: ");
                    scanf_s("%d %d %d", &row, &col, &num);
                    row--; col--;
                    if (makeMove(grid, row, col, num, N))
                        printf("Ruch zaakceptowany.\n");
                }
                else if (c == 2) {
                    printf("Podaj wiersz kolumne nowa_liczbe: ");
                    scanf_s("%d %d %d", &row, &col, &num);
                    row--; col--;
                    if (changeValue(grid, row, col, num, N))
                        printf("Wartosc zmieniona.\n");
                }
                else if (c == 3) {
                    break;
                }
                else {
                    printf("Nieznana opcja.\n");
                }
            }
        }
        else if (choice == 2) {
            boardSize = getBoardSize();
        }
        else if (choice == 3) {
            difficulty = getDifficulty();
        }
        else if (choice == 4) {
            showInstructions();
        }
        else if (choice == 5) {
            printf("Koniec programu.\n");
            break;
        }
        else {
            printf("Nieznana opcja.\n");
        }
    }

    return 0;
}